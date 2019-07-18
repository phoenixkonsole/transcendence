// Copyright (c) 2017-2019 The Transcendence developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/test/unit_test.hpp>

#include <vector>
#include <cstdlib>

#include "masternode-tiers.h"
#include "primitives/transaction.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(tier_tests)

BOOST_AUTO_TEST_CASE(test_tier_from_outputs)
{
    int nextLockTime = 0;
    int nInput = 0;
    unsigned int tiers[MasternodeTiers::TIER_NONE] = {MasternodeTiers::TIER_1K, MasternodeTiers::TIER_3K, MasternodeTiers::TIER_10K, MasternodeTiers::TIER_30K, MasternodeTiers::TIER_100K};

    for (auto i = 0; i < MasternodeTiers::TIER_NONE; i++) {
        CMutableTransaction tx;
        tx.nLockTime = nextLockTime++;
        tx.vout.resize(nInput + 1);
        tx.vout[nInput].nValue = MASTERNODE_TIER_COINS[i] * COIN;

        BOOST_CHECK(IsMasternodeOutput(tx.vout[nInput].nValue, TIER_BLOCK_HEIGHT));
        BOOST_CHECK_EQUAL(GetMasternodeTierFromOutput(tx.vout[nInput].nValue, TIER_BLOCK_HEIGHT), tiers[i]);
    }
    CMutableTransaction txNotMn;
    txNotMn.nLockTime = nextLockTime++;
    txNotMn.vout.resize(nInput + 1);
    txNotMn.vout[nInput].nValue = 1234 * COIN;

    BOOST_CHECK(!IsMasternodeOutput(txNotMn.vout[nInput].nValue, TIER_BLOCK_HEIGHT));
    BOOST_CHECK_EQUAL(GetMasternodeTierFromOutput(txNotMn.vout[nInput].nValue, TIER_BLOCK_HEIGHT), MasternodeTiers::TIER_NONE);
}

BOOST_AUTO_TEST_CASE(test_masternode_coins)
{
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_1K), 1000);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_3K), 3000);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_10K), 10000);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_30K), 30000);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_100K), 100000);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_NONE), 0);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(-2), 0);
}

BOOST_AUTO_TEST_CASE(test_obfuscation_value)
{
    BOOST_TEST(GetObfuscationValueForTier(MasternodeTiers::TIER_1K) == 999.99, boost::test_tools::tolerance(0.0001));
    BOOST_TEST(GetObfuscationValueForTier(MasternodeTiers::TIER_3K) == 2999.99, boost::test_tools::tolerance(0.0001));
    BOOST_TEST(GetObfuscationValueForTier(MasternodeTiers::TIER_10K) == 9999.99, boost::test_tools::tolerance(0.0001));
    BOOST_TEST(GetObfuscationValueForTier(MasternodeTiers::TIER_30K) == 29999.99, boost::test_tools::tolerance(0.0001));
    BOOST_TEST(GetObfuscationValueForTier(MasternodeTiers::TIER_100K) == 99999.99, boost::test_tools::tolerance(0.0001));
    BOOST_TEST(GetObfuscationValueForTier(MasternodeTiers::TIER_NONE) == 0, boost::test_tools::tolerance(0.0001));
    BOOST_TEST(GetObfuscationValueForTier(-2) == 0, boost::test_tools::tolerance(0.0001));
}

BOOST_AUTO_TEST_CASE(test_intial_distribution)
{
    std::vector<size_t> vecTierSizes;
    std::vector<int> vecBlockNumbers = {0, 1, 3, 4, 13, 14, 43, 44, 143, 144};
    std::vector<unsigned int> vecTiers = {MasternodeTiers::TIER_1K, MasternodeTiers::TIER_3K, MasternodeTiers::TIER_3K,
                                          MasternodeTiers::TIER_10K, MasternodeTiers::TIER_10K, MasternodeTiers::TIER_30K,
                                          MasternodeTiers::TIER_30K, MasternodeTiers::TIER_100K, MasternodeTiers::TIER_100K, MasternodeTiers::TIER_1K};

    for (auto i = 0; i < MasternodeTiers::TIER_NONE; i++) {
        vecTierSizes.push_back(1);
    }

    for (auto j = 0; j < vecBlockNumbers.size(); j++) {
        uint256 tmpHash(vecBlockNumbers[j]);
        auto nTier = CalculateWinningTier(vecTierSizes, tmpHash);
        BOOST_CHECK_EQUAL(nTier, vecTiers[j]);
    }
}

BOOST_AUTO_TEST_CASE(test_single_tier)
{
    std::vector<int> vecBlockNumbers = {0, 1, 3, 4, 13, 14, 43, 44, 143, 144};
    unsigned int tiers[MasternodeTiers::TIER_NONE] = {MasternodeTiers::TIER_1K, MasternodeTiers::TIER_3K, MasternodeTiers::TIER_10K, MasternodeTiers::TIER_30K, MasternodeTiers::TIER_100K};

    for (auto tier = 0; tier < MasternodeTiers::TIER_NONE; tier++) {
        std::vector<size_t> vecTierSizes = {0, 0, 0, 0, 0};
        vecTierSizes[tier] = 10;

        for (auto i = 0; i < vecBlockNumbers.size(); i++) {
            uint256 tmpHash(vecBlockNumbers[i]);
            BOOST_CHECK_EQUAL(CalculateWinningTier(vecTierSizes, tmpHash), tiers[tier]);
        }
    }
}

BOOST_AUTO_TEST_CASE(test_winning_tier)
{
    std::vector<size_t> vecTierSizes1 = {100, 20, 10, 5, 2};
    std::vector<int> vecBlockNumbers1 = {0, 23, 24, 37, 39, 61, 62, 96, 97, 143, 144};
    std::vector<unsigned int> vecTiers1 = {MasternodeTiers::TIER_1K, MasternodeTiers::TIER_1K, MasternodeTiers::TIER_3K,
                                           MasternodeTiers::TIER_3K, MasternodeTiers::TIER_10K, MasternodeTiers::TIER_10K,
                                           MasternodeTiers::TIER_30K, MasternodeTiers::TIER_30K, MasternodeTiers::TIER_100K,
                                           MasternodeTiers::TIER_100K, MasternodeTiers::TIER_1K};
    for (auto j = 0; j < vecBlockNumbers1.size(); j++) {
        uint256 tmpHash(vecBlockNumbers1[j]);
        auto nTier = CalculateWinningTier(vecTierSizes1, tmpHash);
        BOOST_CHECK_EQUAL(nTier, vecTiers1[j]);
    }

    std::vector<size_t> vecTierSizes2 = {100, 20, 10, 5, 0};
    std::vector<int> vecBlockNumbers2 = {0, 10, 11, 16, 17, 27, 28, 43, 44};
    std::vector<unsigned int> vecTiers2 = {MasternodeTiers::TIER_1K, MasternodeTiers::TIER_1K, MasternodeTiers::TIER_3K,
                                           MasternodeTiers::TIER_3K, MasternodeTiers::TIER_10K, MasternodeTiers::TIER_10K,
                                           MasternodeTiers::TIER_30K, MasternodeTiers::TIER_30K, MasternodeTiers::TIER_1K};
    for (auto j = 0; j < vecBlockNumbers2.size(); j++) {
        uint256 tmpHash(vecBlockNumbers2[j]);
        auto nTier = CalculateWinningTier(vecTierSizes2, tmpHash);
        BOOST_CHECK_EQUAL(nTier, vecTiers2[j]);
    }

    std::vector<size_t> vecTierSizes3 = {100, 40, 0, 5, 1};
    std::vector<int> vecBlockNumbers3 = {0, 28, 29, 62, 63, 105, 106, 133, 134};
    std::vector<unsigned int> vecTiers3 = {MasternodeTiers::TIER_1K, MasternodeTiers::TIER_1K, MasternodeTiers::TIER_3K,
                                           MasternodeTiers::TIER_3K, MasternodeTiers::TIER_30K, MasternodeTiers::TIER_30K,
                                           MasternodeTiers::TIER_100K, MasternodeTiers::TIER_100K, MasternodeTiers::TIER_1K};
    for (auto j = 0; j < vecBlockNumbers3.size(); j++) {
        uint256 tmpHash(vecBlockNumbers3[j]);
        auto nTier = CalculateWinningTier(vecTierSizes3, tmpHash);
        BOOST_CHECK_EQUAL(nTier, vecTiers3[j]);
    }
}


BOOST_AUTO_TEST_SUITE_END()
