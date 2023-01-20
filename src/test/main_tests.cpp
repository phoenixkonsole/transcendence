// Copyright (c) 2014 The Bitcoin Core developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2017-2019 The Transcendence developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/transaction.h"
#include "main.h"
#include "spork.h"
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

    for (int nHeight = 1578902; nHeight <= 1578902 + 100; nHeight += 1) {
        /* Masternode tiers system implemented */
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == 25 * COIN);
        nSum += nSubsidy;
    }

    long int startHeight = (SPORK_21_SUPERBLOCK_START_DEFAULT / SPORK_21_SUPERBLOCK_PERIOD_DEFAULT + 1) * SPORK_21_SUPERBLOCK_PERIOD_DEFAULT;
    for (int nHeight = startHeight; nHeight <= startHeight + SPORK_21_SUPERBLOCK_PERIOD_DEFAULT*10; nHeight += SPORK_21_SUPERBLOCK_PERIOD_DEFAULT) {
        /* Masternode tiers system implemented */
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == 300000 * COIN);
        nSum += nSubsidy;
    }
}

BOOST_AUTO_TEST_CASE(halving_test)
{
    double standartReward = 25;

    BOOST_CHECK(GetHalvingReward(0) == standartReward);
    BOOST_CHECK(GetHalvingReward(1578902) == standartReward);
    BOOST_CHECK(GetHalvingReward(1578902 + 525600) == (standartReward / 2));
    BOOST_CHECK(GetHalvingReward(1578902 + 525600 + 525600 + 525600 + 525600) == (standartReward / 5));
}

BOOST_AUTO_TEST_CASE(superblock_halving_test)
{
    double standartReward = 300000;

    BOOST_CHECK(GetSuperblockHalvingReward(0) == standartReward);
    BOOST_CHECK(GetSuperblockHalvingReward( (SPORK_21_SUPERBLOCK_START_DEFAULT / SPORK_21_SUPERBLOCK_PERIOD_DEFAULT + 1) * SPORK_21_SUPERBLOCK_PERIOD_DEFAULT) == standartReward);
    BOOST_CHECK(GetSuperblockHalvingReward(((SPORK_21_SUPERBLOCK_START_DEFAULT + 525600) / SPORK_21_SUPERBLOCK_PERIOD_DEFAULT + 1) * SPORK_21_SUPERBLOCK_PERIOD_DEFAULT) == (standartReward / 2));
    BOOST_CHECK(GetSuperblockHalvingReward(((SPORK_21_SUPERBLOCK_START_DEFAULT + 525600 * 4) / SPORK_21_SUPERBLOCK_PERIOD_DEFAULT + 1) * SPORK_21_SUPERBLOCK_PERIOD_DEFAULT) == (standartReward / 5));
}

BOOST_AUTO_TEST_CASE(masternode_payment_test)
{
    const int nMasternodeCount = 5
    int64_t blockValue = 200
    BOOST_CHECK(GetMasternodePayment(100, blockValue, nMasternodeCount) == 0);
    BOOST_CHECK(GetMasternodePayment(SPORK_17_MASTERNODE_PAYMENT_CHECK_DEFAULT, blockValue, nMasternodeCount) == blockValue / 100 * 90);
    BOOST_CHECK(GetMasternodePayment(SPORK_19_LOWERED_MASTERNODE_PAYMENT_DEFAULT - 1, blockValue, nMasternodeCount) == blockValue / 100 * 80);
    BOOST_CHECK(GetMasternodePayment(SPORK_21_SUPERBLOCK_START_DEFAULT - 1, blockValue, nMasternodeCount) == blockValue / 100 * 70);
    BOOST_CHECK(GetMasternodePayment(SPORK_21_SUPERBLOCK_START_DEFAULT, blockValue, nMasternodeCount) == 0);
}

BOOST_AUTO_TEST_SUITE_END()
