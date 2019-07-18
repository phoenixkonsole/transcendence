// Copyright (c) 2014 The Bitcoin Core developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2017-2019 The Transcendence developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/transaction.h"
#include "main.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(main_tests)

BOOST_AUTO_TEST_CASE(subsidy_limit_test)
{
    CAmount nSum = 0;
    CAmount nSubsidy = 0;

    nSubsidy = GetBlockValue(0);
    BOOST_CHECK(nSubsidy == 0.1 * COIN);
    nSum += nSubsidy;

    /* premine in block 1 (3,000,000 TRANSCENDENCE) */
    nSubsidy = GetBlockValue(1);
    BOOST_CHECK(nSubsidy == 3000000 * COIN);
    nSum += nSubsidy;

    nSubsidy = GetBlockValue(0);
    BOOST_CHECK(nSubsidy == 0.1 * COIN);
    nSum += nSubsidy;

    for (int nHeight = 3; nHeight <= 291; nHeight += 1) {
        /* PoW Phase */
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == 3 * COIN);
        nSum += nSubsidy;
    }

    for (int nHeight = 292; nHeight <= 1728; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == 2 * COIN);
        nSum += nSubsidy;
    }

    for (int nHeight = 1729; nHeight <= 4608; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == 5 * COIN);
        nSum += nSubsidy;
    }

    for (int nHeight = 4609; nHeight <= 8928; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == 10 * COIN);
        nSum += nSubsidy;
    }

    for (int nHeight = 8929; nHeight <= 11808; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == 25 * COIN);
        nSum += nSubsidy;
    }

    for (int nHeight = 11809; nHeight <= 16128; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == 70 * COIN);
        nSum += nSubsidy;
    }

    for (int nHeight = 16129; nHeight <= 19008; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == 100 * COIN);
        nSum += nSubsidy;
    }
    //Until TIER_BLOCK_HEIGHT
    for (int nHeight = 19009; nHeight <= 524500; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == 200 * COIN);
        nSum += nSubsidy;
    }

    for (int nHeight = 524501; nHeight <= 524500 + 100; nHeight += 1) {
        /* Masternode tiers system implemented */
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == 100 * COIN);
        nSum += nSubsidy;
    }
}

BOOST_AUTO_TEST_SUITE_END()
