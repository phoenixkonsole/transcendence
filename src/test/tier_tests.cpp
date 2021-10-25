// Copyright (c) 2017-2019 The Transcendence developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/test/unit_test.hpp>

#include <vector>
#include <cstdlib>

#include "masternode-tiers.h"
#include "primitives/transaction.h"
#include "spork.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(tier_tests)

namespace
{
    void TierFromOutputTest(unsigned int* tiers, bool* isMastermodeOutput, int blockHeight)
    {
        int nextLockTime = 0;
        int nInput = 0;
            
        for (auto i = 0; i < MasternodeTiers::TIER_NONE; i++) {
            CMutableTransaction tx;
            tx.nLockTime = nextLockTime++;
            tx.vout.resize(nInput + 1);
            tx.vout[nInput].nValue = MASTERNODE_TIER_COINS[i] * COIN;

            BOOST_CHECK_EQUAL(IsMasternodeOutput(tx.vout[nInput].nValue, blockHeight), isMastermodeOutput[i]);
            BOOST_CHECK_EQUAL(GetMasternodeTierFromOutput(tx.vout[nInput].nValue, blockHeight), tiers[i]);
        }
        CMutableTransaction txNotMn;
        txNotMn.nLockTime = nextLockTime++;
        txNotMn.vout.resize(nInput + 1);
        txNotMn.vout[nInput].nValue = 1234 * COIN;

        BOOST_CHECK(!IsMasternodeOutput(txNotMn.vout[nInput].nValue, blockHeight));
        BOOST_CHECK_EQUAL(GetMasternodeTierFromOutput(txNotMn.vout[nInput].nValue, blockHeight), MasternodeTiers::TIER_NONE);
    }
}

BOOST_AUTO_TEST_CASE(test_tier_from_outputs_old_tiers)
{
    unsigned int tiers[MasternodeTiers::TIER_NONE] = {MasternodeTiers::TIER_1K, MasternodeTiers::TIER_3K, MasternodeTiers::TIER_10K,
                                                      MasternodeTiers::TIER_30K, MasternodeTiers::TIER_100K, MasternodeTiers::TIER_NONE,
                                                      MasternodeTiers::TIER_NONE, MasternodeTiers::TIER_NONE, MasternodeTiers::TIER_NONE};
    bool isMastermodeOutput[MasternodeTiers::TIER_NONE] = {true, true, true, true, true, false, false, false, false};
    TierFromOutputTest(tiers, isMastermodeOutput, TIER_BLOCK_HEIGHT);
}

BOOST_AUTO_TEST_CASE(test_tier_from_outputs_new_tiers)
{
    unsigned int tiers[MasternodeTiers::TIER_NONE] = {MasternodeTiers::TIER_NONE, MasternodeTiers::TIER_NONE, MasternodeTiers::TIER_NONE,
                                                      MasternodeTiers::TIER_NONE, MasternodeTiers::TIER_100K, MasternodeTiers::TIER_300K,
                                                      MasternodeTiers::TIER_1KK, MasternodeTiers::TIER_3KK, MasternodeTiers::TIER_10KK};
    bool isMastermodeOutput[MasternodeTiers::TIER_NONE] = {false, false, false, false, true, true, true, true, true};
    TierFromOutputTest(tiers, isMastermodeOutput, SPORK_17_MASTERNODE_PAYMENT_CHECK_DEFAULT);
}

BOOST_AUTO_TEST_CASE(test_tier_from_outputs_truncated_tiers)
{
    unsigned int tiers[MasternodeTiers::TIER_NONE] = {MasternodeTiers::TIER_1K,
                                                      MasternodeTiers::TIER_3K, MasternodeTiers::TIER_10K, MasternodeTiers::TIER_30K,
                                                      MasternodeTiers::TIER_100K, MasternodeTiers::TIER_300K, MasternodeTiers::TIER_1KK,
                                                      MasternodeTiers::TIER_3K, MasternodeTiers::TIER_10KK};
    bool isMastermodeOutput[MasternodeTiers::TIER_NONE] = {false, false, true, true, true, true, true, false, false};
    TierFromOutputTest(tiers, isMastermodeOutput, SPORK_40_SUPERBLOCK_START_DEFAULT);
}

BOOST_AUTO_TEST_CASE(test_masternode_coins)
{
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_1K), 1000);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_3K), 3000);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_10K), 10000);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_30K), 30000);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_100K), 100000);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_300K), 300000);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_1KK), 1000000);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_3KK), 3000000);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_10KK), 10000000);
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
    BOOST_TEST(GetObfuscationValueForTier(MasternodeTiers::TIER_300K) == 299999.99, boost::test_tools::tolerance(0.0001));
    BOOST_TEST(GetObfuscationValueForTier(MasternodeTiers::TIER_1KK) == 999999.99, boost::test_tools::tolerance(0.0001));
    BOOST_TEST(GetObfuscationValueForTier(MasternodeTiers::TIER_3KK) == 2999999.99, boost::test_tools::tolerance(0.0001));
    BOOST_TEST(GetObfuscationValueForTier(MasternodeTiers::TIER_10KK) == 9999999.99, boost::test_tools::tolerance(0.0001));
    BOOST_TEST(GetObfuscationValueForTier(MasternodeTiers::TIER_NONE) == 0, boost::test_tools::tolerance(0.0001));
    BOOST_TEST(GetObfuscationValueForTier(-2) == 0, boost::test_tools::tolerance(0.0001));
}

BOOST_AUTO_TEST_CASE(test_intial_distribution)
{
    std::vector<size_t> vecTierSizes;
    std::vector<int> vecBlockNumbers = {0, 1, 3, 4, 13, 14, 43, 44, 143, 144, 443, 444, 1443, 1444, 4443, 4444, 14443, 14444};
    std::vector<unsigned int> vecTiers = {MasternodeTiers::TIER_1K, MasternodeTiers::TIER_3K, MasternodeTiers::TIER_3K,
                                          MasternodeTiers::TIER_10K, MasternodeTiers::TIER_10K, MasternodeTiers::TIER_30K,
                                          MasternodeTiers::TIER_30K, MasternodeTiers::TIER_100K, MasternodeTiers::TIER_100K,
                                          MasternodeTiers::TIER_300K, MasternodeTiers::TIER_300K, MasternodeTiers::TIER_1KK,
                                          MasternodeTiers::TIER_1KK, MasternodeTiers::TIER_3KK, MasternodeTiers::TIER_3KK,
                                          MasternodeTiers::TIER_10KK, MasternodeTiers::TIER_10KK};

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
    std::vector<int> vecBlockNumbers = {0, 1, 3, 4, 13, 14, 43, 44, 143, 144, 312, 322, 1024, 1234, 3003, 8888, 10321, 11111};
    unsigned int tiers[MasternodeTiers::TIER_NONE] = {MasternodeTiers::TIER_1K, MasternodeTiers::TIER_3K, MasternodeTiers::TIER_10K,
                                                      MasternodeTiers::TIER_30K, MasternodeTiers::TIER_100K, MasternodeTiers::TIER_300K,
                                                      MasternodeTiers::TIER_1KK, MasternodeTiers::TIER_3KK, MasternodeTiers::TIER_10KK};

    for (auto tier = 0; tier < MasternodeTiers::TIER_NONE; tier++) {
        std::vector<size_t> vecTierSizes(MasternodeTiers::TIER_NONE, 0);
        vecTierSizes[tier] = 10;

        for (auto i = 0; i < vecBlockNumbers.size(); i++) {
            uint256 tmpHash(vecBlockNumbers[i]);
            BOOST_CHECK_EQUAL(CalculateWinningTier(vecTierSizes, tmpHash), tiers[tier]);
        }
    }
}

void WinningTierTest(const std::vector<size_t>& vecTierSizes,
                     const std::vector<int>& vecBlockNumbers,
                     const std::vector<unsigned int>& vecTiers)
{
    for (auto j = 0; j < vecBlockNumbers.size(); j++) {
        uint256 tmpHash(vecBlockNumbers[j]);
        auto nTier = CalculateWinningTier(vecTierSizes, tmpHash);
        BOOST_CHECK_EQUAL(nTier, vecTiers[j]);
    }
}

BOOST_AUTO_TEST_CASE(test_winning_tier)
{
    std::vector<size_t> vecTierSizes = {100, 20, 10, 5, 2, 0, 0, 0, 0};
    std::vector<int> vecBlockNumbers = {0, 23, 24, 37, 39, 61, 62, 96, 97, 143, 144};
    std::vector<unsigned int> vecTiers = {MasternodeTiers::TIER_1K, MasternodeTiers::TIER_1K, MasternodeTiers::TIER_3K,
                                           MasternodeTiers::TIER_3K, MasternodeTiers::TIER_10K, MasternodeTiers::TIER_10K,
                                           MasternodeTiers::TIER_30K, MasternodeTiers::TIER_30K, MasternodeTiers::TIER_100K,
                                           MasternodeTiers::TIER_100K, MasternodeTiers::TIER_1K};
    WinningTierTest(vecTierSizes, vecBlockNumbers, vecTiers);
}

BOOST_AUTO_TEST_CASE(test_winning_tier2)
{
    std::vector<size_t> vecTierSizes = {100, 20, 10, 5, 0, 0, 0, 0, 0};
    std::vector<int> vecBlockNumbers = {0, 10, 11, 16, 17, 27, 28, 43, 44};
    std::vector<unsigned int> vecTiers = {MasternodeTiers::TIER_1K, MasternodeTiers::TIER_1K, MasternodeTiers::TIER_3K,
                                           MasternodeTiers::TIER_3K, MasternodeTiers::TIER_10K, MasternodeTiers::TIER_10K,
                                           MasternodeTiers::TIER_30K, MasternodeTiers::TIER_30K, MasternodeTiers::TIER_1K};
    WinningTierTest(vecTierSizes, vecBlockNumbers, vecTiers);
}

BOOST_AUTO_TEST_CASE(test_winning_tier3)
{
    std::vector<size_t> vecTierSizes = {100, 40, 0, 5, 1, 0, 0, 0, 0};
    std::vector<int> vecBlockNumbers = {0, 28, 29, 62, 63, 105, 106, 133, 134};
    std::vector<unsigned int> vecTiers = {MasternodeTiers::TIER_1K, MasternodeTiers::TIER_1K, MasternodeTiers::TIER_3K,
                                           MasternodeTiers::TIER_3K, MasternodeTiers::TIER_30K, MasternodeTiers::TIER_30K,
                                           MasternodeTiers::TIER_100K, MasternodeTiers::TIER_100K, MasternodeTiers::TIER_1K};
    WinningTierTest(vecTierSizes, vecBlockNumbers, vecTiers);
}

BOOST_AUTO_TEST_CASE(test_winning_tier_new_tiers)
{
    std::vector<size_t> vecTierSizes = {0, 0, 0, 0, 100, 20, 10, 5, 1};
    std::vector<int> vecBlockNumbers = {328, 329, 3462, 3463, 5105, 5106, 11033, 11034, 26123, 26321};
    std::vector<unsigned int> vecTiers = {MasternodeTiers::TIER_100K, MasternodeTiers::TIER_100K,  MasternodeTiers::TIER_300K,
                                           MasternodeTiers::TIER_300K, MasternodeTiers::TIER_1KK, MasternodeTiers::TIER_1KK,
                                           MasternodeTiers::TIER_3KK, MasternodeTiers::TIER_3KK, MasternodeTiers::TIER_10KK,
                                           MasternodeTiers::TIER_10KK};
    WinningTierTest(vecTierSizes, vecBlockNumbers, vecTiers);
}

BOOST_AUTO_TEST_CASE(test_winning_tier_truncated_tiers)
{
    std::vector<size_t> vecTierSizes = {0, 0, 100, 40, 20, 10, 5, 0, 0}; 
    std::vector<int> vecBlockNumbers = {115, 116, 256, 257, 491, 492, 845, 846, 1437, 1438};
    std::vector<unsigned int> vecTiers = {MasternodeTiers::TIER_10K, MasternodeTiers::TIER_10K,  MasternodeTiers::TIER_30K,
                                           MasternodeTiers::TIER_30K, MasternodeTiers::TIER_100K, MasternodeTiers::TIER_100K,
                                           MasternodeTiers::TIER_300K, MasternodeTiers::TIER_300K, MasternodeTiers::TIER_1KK,
                                           MasternodeTiers::TIER_1KK};
    WinningTierTest(vecTierSizes, vecBlockNumbers, vecTiers);
}

BOOST_AUTO_TEST_SUITE_END()