// Copyright (c) 2020 TBP developers
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
        const std::string* payload;
    };

    size_t getPayload(void *ptr, size_t size, size_t nmemb, void *userp) {
        uploadStatus *upload_ctx = (uploadStatus *)userp;
        const char *data;
        LogPrintf("email: getPayload\n");

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

            LogPrintf("email: read %u bytes for upload\n", len);
            return len;
        }

        return 0;
    }
}

bool smtp::sendEmail(const std::string& sender,
                     const std::string& recipient,
                     const std::string& smtpUrl,
                     const std::string& smtpUser,
                     const std::string& smtpPass,
                     const std::string& subject,
                     const std::string& body) {           
    CURL *curl;
    CURLcode res;
    struct curl_slist *recipients = NULL;
    uploadStatus upload_ctx;
    upload_ctx.linesRead = 0;

    const std::string payload[] = {
        tinyformat::format("From: %s\r\n", sender),
        tinyformat::format("To: %s\r\n", recipient),
        tinyformat::format("Subject: %s\r\n\r\n", subject),
        body,
        ""
    };
    upload_ctx.payload = payload;
    LogPrintf("email: Preparing to send email: sender - %s, recipient - %s, url - %s, username - %s\n", sender, recipient, smtpUrl, smtpUser);

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, smtpUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0); 
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0); 
        curl_easy_setopt(curl, CURLOPT_USERNAME, smtpUser.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, smtpPass.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, sender.c_str());
        recipients = curl_slist_append(recipients, recipient.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        curl_easy_setopt(curl, CURLOPT_READFUNCTION, getPayload);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        LogPrintf("email: Preparing to send email\n");
        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK) {
            LogPrintf("email: Failed to send email %s\n", curl_easy_strerror(res));
        }

        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);

        return res == CURLE_OK;
    }
}