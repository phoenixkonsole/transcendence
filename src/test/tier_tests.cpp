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
    unsigned int tiers[MasternodeTiers::TIER_NONE] = {MasternodeTiers::TIER_10K,
                                                      MasternodeTiers::TIER_30K, MasternodeTiers::TIER_100K, MasternodeTiers::TIER_NONE,
                                                      MasternodeTiers::TIER_NONE};
    bool isMastermodeOutput[MasternodeTiers::TIER_NONE] = {true, true, true, false, false};
    TierFromOutputTest(tiers, isMastermodeOutput, TIER_BLOCK_HEIGHT);
}

BOOST_AUTO_TEST_CASE(test_tier_from_outputs_new_tiers)
{
    unsigned int tiers[MasternodeTiers::TIER_NONE] = {MasternodeTiers::TIER_NONE,
                                                      MasternodeTiers::TIER_NONE, MasternodeTiers::TIER_100K, MasternodeTiers::TIER_300K,
                                                      MasternodeTiers::TIER_1KK};
    bool isMastermodeOutput[MasternodeTiers::TIER_NONE] = {false, false, true, true, true};
    TierFromOutputTest(tiers, isMastermodeOutput, SPORK_17_MASTERNODE_PAYMENT_CHECK_DEFAULT);
}

BOOST_AUTO_TEST_CASE(test_masternode_coins)
{
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_10K), 10000);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_30K), 30000);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_100K), 100000);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_300K), 300000);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_1KK), 1000000);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(MasternodeTiers::TIER_NONE), 0);
    BOOST_CHECK_EQUAL(GetMastenodeTierCoins(-2), 0);
}

BOOST_AUTO_TEST_CASE(test_obfuscation_value)
{
    BOOST_TEST(GetObfuscationValueForTier(MasternodeTiers::TIER_10K) == 9999.99, boost::test_tools::tolerance(0.0001));
    BOOST_TEST(GetObfuscationValueForTier(MasternodeTiers::TIER_30K) == 29999.99, boost::test_tools::tolerance(0.0001));
    BOOST_TEST(GetObfuscationValueForTier(MasternodeTiers::TIER_100K) == 99999.99, boost::test_tools::tolerance(0.0001));
    BOOST_TEST(GetObfuscationValueForTier(MasternodeTiers::TIER_300K) == 299999.99, boost::test_tools::tolerance(0.0001));
    BOOST_TEST(GetObfuscationValueForTier(MasternodeTiers::TIER_1KK) == 999999.99, boost::test_tools::tolerance(0.0001));
    BOOST_TEST(GetObfuscationValueForTier(MasternodeTiers::TIER_NONE) == 0, boost::test_tools::tolerance(0.0001));
    BOOST_TEST(GetObfuscationValueForTier(-2) == 0, boost::test_tools::tolerance(0.0001));
}

BOOST_AUTO_TEST_CASE(test_intial_distribution)
{
    std::vector<size_t> vecTierSizes;
    std::vector<int> vecBlockNumbers = {3, 9, 10, 39, 40, 139, 140, 439, 440, 1423, 1439};
    std::vector<unsigned int> vecTiers = {
                                          MasternodeTiers::TIER_10K, MasternodeTiers::TIER_10K, MasternodeTiers::TIER_30K,
                                          MasternodeTiers::TIER_30K, MasternodeTiers::TIER_100K, MasternodeTiers::TIER_100K,
                                          MasternodeTiers::TIER_300K, MasternodeTiers::TIER_300K, MasternodeTiers::TIER_1KK,
                                          MasternodeTiers::TIER_1KK, MasternodeTiers::TIER_1KK};

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
    std::vector<int> vecBlockNumbers = {3, 4, 13, 14, 43, 44, 143, 144, 312, 322, 1024, 1234, 3003, 8888};
    unsigned int tiers[MasternodeTiers::TIER_NONE] = {MasternodeTiers::TIER_10K,
                                                      MasternodeTiers::TIER_30K, 
                                                      MasternodeTiers::TIER_100K, 
                                                      MasternodeTiers::TIER_300K,
                                                      MasternodeTiers::TIER_1KK};

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
    std::vector<size_t> vecTierSizes = {5, 2, 1, 0, 0};
    std::vector<int> vecBlockNumbers = {0, 31, 32, 70, 71, 131, 139, 140};
    std::vector<unsigned int> vecTiers = { MasternodeTiers::TIER_10K, MasternodeTiers::TIER_10K,
                                           MasternodeTiers::TIER_10K, MasternodeTiers::TIER_30K, MasternodeTiers::TIER_30K,
                                           MasternodeTiers::TIER_100K, MasternodeTiers::TIER_100K, MasternodeTiers::TIER_10K};
    WinningTierTest(vecTierSizes, vecBlockNumbers, vecTiers);
}

BOOST_AUTO_TEST_CASE(test_winning_tier2)
{
    std::vector<size_t> vecTierSizes = {5, 2, 0, 0, 0};
    std::vector<int> vecBlockNumbers = {0, 16, 17, 37, 38, 40};
    std::vector<unsigned int> vecTiers = {MasternodeTiers::TIER_10K, MasternodeTiers::TIER_10K, 
                                          MasternodeTiers::TIER_10K, MasternodeTiers::TIER_30K, 
                                          MasternodeTiers::TIER_30K, MasternodeTiers::TIER_10K,};
    WinningTierTest(vecTierSizes, vecBlockNumbers, vecTiers);
}

BOOST_AUTO_TEST_CASE(test_winning_tier3)
{
    std::vector<size_t> vecTierSizes = {5, 0, 1, 0, 0};
    std::vector<int> vecBlockNumbers = {20, 21, 34, 35, 61, 71, 109, 110};
    std::vector<unsigned int> vecTiers = {MasternodeTiers::TIER_10K, MasternodeTiers::TIER_10K, 
                                          MasternodeTiers::TIER_10K, MasternodeTiers::TIER_10K,
                                          MasternodeTiers::TIER_100K, MasternodeTiers::TIER_100K,
                                          MasternodeTiers::TIER_100K, MasternodeTiers::TIER_10K};
    WinningTierTest(vecTierSizes, vecBlockNumbers, vecTiers);
}

BOOST_AUTO_TEST_CASE(test_winning_tier_new_tiers)
{
    std::vector<size_t> vecTierSizes = {0, 0, 0, 20, 10};
    std::vector<int> vecBlockNumbers = {353, 387, 487, 898, 1153, 1238, 1289};
    std::vector<unsigned int> vecTiers = {MasternodeTiers::TIER_300K, MasternodeTiers::TIER_300K,  MasternodeTiers::TIER_300K,
                                           MasternodeTiers::TIER_1KK, MasternodeTiers::TIER_1KK, MasternodeTiers::TIER_1KK
                                           , MasternodeTiers::TIER_1KK};
    WinningTierTest(vecTierSizes, vecBlockNumbers, vecTiers);
}


BOOST_AUTO_TEST_SUITE_END()
