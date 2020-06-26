// Copyright (c) 2020 Telos developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "smtputils.h"
#include <curl/curl.h>
#include "utiltime.h"
#include "tinyformat.h"
#include "util.h"

namespace {
    struct uploadStatus {
        int linesRead;
        std::string* payload;
    };

    size_t getPayload(void *ptr, size_t size, size_t nmemb, void *userp) {
        uploadStatus *upload_ctx = (uploadStatus *)userp;
        const char *data;

        if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1))
        {
            return 0;
        }

        data = upload_ctx->payload[upload_ctx->linesRead].c_str();

        if (data)
        {
            size_t len = strlen(data);
            memcpy(ptr, data, len);
            upload_ctx->linesRead++;

            return len;
        }

        return 0;
    }
}

bool smtp::sendEmail(const std::string &recipient, const std::string& subject, const std::string &body) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *recipients = NULL;
    uploadStatus upload_ctx;
    upload_ctx.linesRead = 0;

    /* value for envelope reverse-path */
    static const std::string from = "test.some.more.bob@gmail.com";

    const std::string payload[] = {
        tinyformat::format("From: %s\n", from),
        tinyformat::format("To: %s\n", recipient),
        tinyformat::format("Subject: %s\n", subject),
        body,
        "\0"
    };

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com");
        curl_easy_setopt(curl, CURLOPT_PORT, 465);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0); 
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0); 
        curl_easy_setopt(curl, CURLOPT_USERNAME, "test.some.more.bob@gmail.com");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "Fghbjhbn1!");
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from.c_str());
        recipients = curl_slist_append(recipients, recipient.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, getPayload);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);

        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK) {
            LogPrintf("Failed to send email %s\n", curl_easy_strerror(res));
        }

        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);

        return res == CURLE_OK;
    }
}