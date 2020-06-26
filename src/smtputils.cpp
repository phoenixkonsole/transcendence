// Copyright (c) 2020 Telos developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "smtputils.h"
#include <string>
#include <curl/curl.h>
#include "utiltime.h"
#include "tinyformat.h"
#include "util.h"

namespace {
    std::function<void *, size_t, size_t, void *> getPayloadSource(const std::string *payload) {
        return [payload] (void *ptr, size_t size, size_t nmemb, void *userp) => {
            struct upload_status *upload_ctx = (struct upload_status *)userp;
            const char *data;

            if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1))
            {
                return 0;
            }

            data = payload[upload_ctx->lines_read]->c_str();

            if (data)
            {
                size_t len = strlen(data);
                memcpy(ptr, data, len);
                upload_ctx->lines_read++;

                return len;
            }

            return 0;
        }
    }
}

bool smtp::sendEmail(const std::string &recipient, const std::string& subject, const std::string &body) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *recipients = NULL;
    struct upload_status upload_ctx;

    upload_ctx.lines_read = 0;

    /* value for envelope reverse-path */
    static const std::string from = "password_backup@teloscoin.org";

    const std::string timestamp = DateTimeStrFormat("%Y-%m-%d %H:%M:%S", GetTime());
    const std::string payload[] = {
        format("Date: %s\n", timestamp),
        format("From: %s\n", from),
        format("To: %s\n", recipient),
        format("Subject: %s\n", subject),
        body,
        "\0"
    };

    curl = curl_easy_init();
    if (curl) {
        /* This is the URL for your mailserver */
        curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_USERNAME, "test.some.more.bob@gmail.com");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "Fghbjhbn1!");
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from);
        recipients = curl_slist_append(recipients, recipient.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, getPayloadSource(payload));
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK) {
            LogPrintf("Using OpenSSL version %s\n", SSLeay_version(SSLEAY_VERSION));
        }

        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);

        return res == CURLE_OK
    }
}