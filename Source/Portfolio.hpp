#pragma once

#include <map>
#include <string>

#include "AssetClass.hpp"
#include "Types.hpp"

class Portfolio
{
public:
    std::string GetName() { return mName; }

    Date GetStartDate() { return mStartDate; }
    int GetDuration() { return mDurationInMonths; }

    RebalancingStrategy GetRebalancingStrategy() { return mRebalancingStrategy; }
    int GetRebalancingPeriod() { return mRebalancingPeriodInMonths; }
    int GetRebalancingThreshold() { return mRebalancingThreshold; }

    const std::map<std::shared_ptr<AssetClass>, float>& GetAssetClassProportions() { return mAssetClassProportionsMap; }

private:
    std::string mName;

    Date mStartDate;
    int mDurationInMonths;

    RebalancingStrategy mRebalancingStrategy;

    int mRebalancingPeriodInMonths {12};
    int mRebalancingThreshold {0};

    std::map<std::shared_ptr<AssetClass>, float> mAssetClassProportionsMap;
};