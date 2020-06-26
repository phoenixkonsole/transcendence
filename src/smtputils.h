// Copyright (c) 2020 Telos developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef TRANSCENDENCE_SMTPUTILS_H
#define TRANSCENDENCE_SMTPUTILS_H

#include <string>

namespace smtp {
    bool sendEmail(const std::string& recipient, const std::string& subject, const std::string& body);
}

#endif //TRANSCENDENCE_SMTPUTILS_H