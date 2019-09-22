// Copyright (c) 2017-2019 The Transcendence developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "masternode-tiers.h"
#include <cmath>

bool IsMasternodeOutput(CAmount nValue, int blockHeight)
{
    return (GetMasternodeTierFromOutput(nValue, blockHeight) != MasternodeTiers::TIER_NONE);
}

unsigned int GetMasternodeTierFromOutput(CAmount nValue, int blockHeight)
{
    unsigned int tierRet = MasternodeTiers::TIER_NONE;

    if (blockHeight < TIER_BLOCK_HEIGHT) {
        if (nValue == MASTERNODE_TIER_COINS[MasternodeTiers::TIER_1K] * COIN) {
            tierRet = MasternodeTiers::TIER_1K;
        }
    }
    else {
        for (unsigned int tier = MasternodeTiers::TIER_1K; tier != MasternodeTiers::TIER_NONE; tier++) {
            if (nValue == MASTERNODE_TIER_COINS[tier] * COIN) {
                tierRet = tier;
                break;
            }
        }
    }
    return tierRet;
}

unsigned int GetMastenodeTierCoins(unsigned int nTier)
{
    if (nTier >= MasternodeTiers::TIER_NONE || nTier < MasternodeTiers::TIER_1K) {
        return 0;
    }
    else {
        return MASTERNODE_TIER_COINS[nTier];
    }
}

double GetObfuscationValueForTier(unsigned int nTier)
{
    if (nTier >= MasternodeTiers::TIER_NONE || nTier < MasternodeTiers::TIER_1K) {
        return 0;
    }
    else {
        return TIER_OBFUSCATION_VALUES[nTier];
    }
}

static unsigned int CalculateWeightedDistribution(unsigned int tierSize, unsigned int tierDistr, unsigned int nMod, double nDenominator)
{
    //Calculate weighted distribution: WD = modulus * (distribution*count) / ( Summ(distribution[i]*count[i]) )
    unsigned int weightedValue = round(tierSize * tierDistr * nMod * 1.0 / nDenominator);
    if (weightedValue == 0) {
        weightedValue = 1;
    }

    return weightedValue;
}

static void CalculateWeightedDistribution(std::vector<std::pair<size_t, unsigned int>>& weightedDistribution, const std::vector<size_t>& vecTierSizes,
                                          const unsigned int distribution[], unsigned int nMod, double nDenominator)
{
    unsigned int nPreviousWeight = 0;
    for (auto j = 0; j < weightedDistribution.size() - 1; j++) {
        auto curTier = weightedDistribution[j].first;

        //Calculate weighted distribution: WD = modulus * (distribution*count) / ( Summ(distribution[i]*count[i]) )
        unsigned int weightedValue = round(vecTierSizes[curTier] * distribution[curTier] * nMod * 1.0 / nDenominator);
        if (weightedValue == 0) {
            weightedValue = 1;
        }
        //Convert weighted distribution to the percent of the modulus
        weightedDistribution[j].second = nPreviousWeight;
        weightedDistribution[j].second += weightedValue;
        nPreviousWeight = weightedDistribution[j].second;
    }
    weightedDistribution[weightedDistribution.size() - 1].second = nMod;
}

static unsigned int TierByHash(std::vector<std::pair<size_t, unsigned int>>& weightedDistribution, uint256 blockHash, unsigned int nMod)
{
    int nWinningTier = MasternodeTiers::TIER_NONE;
    unsigned int nCheckNumber = blockHash.Get64() % nMod;
    for (auto &el : weightedDistribution) {
        if (nCheckNumber < el.second) {
            nWinningTier = el.first;
            break;
        }
    }
    return nWinningTier;
}

unsigned int CalculateWinningTier(const std::vector<size_t>& vecTierSizes, uint256 blockHash)
{
    const unsigned int distribution[MasternodeTiers::TIER_NONE] = {1, 3, 10, 30, 100};
    double nDenominator = 0; // Summ( distribution[i]*count[i] )
    unsigned int nMod = 0; // modulus = Summ( distribution[i] )

    //Contains pairs <tier number, weighted value>
    std::vector<std::pair<size_t, unsigned int>> weightedDistribution;

    if (vecTierSizes.size() < MasternodeTiers::TIER_NONE) {
        return MasternodeTiers::TIER_NONE;
    }

    //Select tiers which contain nodes
    for (auto i = 0; i < MasternodeTiers::TIER_NONE; i++) {
        if (vecTierSizes[i] > 0) {
            nMod += distribution[i];
            nDenominator += distribution[i] * vecTierSizes[i];
            weightedDistribution.push_back(std::make_pair(i, 0));
        }
    }

    //Stop calculation if there are no nodes or the only single tier is presented in the network
    if (weightedDistribution.empty()) {
        return MasternodeTiers::TIER_NONE;
    }
    else if (weightedDistribution.size() == 1) {
        return weightedDistribution[0].first;
    }

    CalculateWeightedDistribution(weightedDistribution, vecTierSizes, distribution, nMod, nDenominator);
    //Now distribution is converted from values [1,3,10,30,100] to the weighted percents, e.g. [1, 4, 14, 44, 144] for modulus = 144

    return TierByHash(weightedDistribution, blockHash, nMod);
}
