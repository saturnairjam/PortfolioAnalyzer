#include "Portfolio.hpp"

const std::string& Portfolio::GetName()
{
    return mName;
}

Date Portfolio::GetStartDate()
{
    return mStartDate;
}

int Portfolio::GetDuration()
{
    return mDurationInMonths;
}

RebalancingStrategy Portfolio::GetRebalancingStrategy()
{
    return mRebalancingStrategy;
}

int Portfolio::GetRebalancingPeriod()
{
    return mRebalancingPeriodInMonths;
}

int Portfolio::GetRebalancingThreshold()
{
    return mRebalancingThreshold;
}

const std::map<std::shared_ptr<AssetClass>, float>& Portfolio::GetAssetClassProportions()
{
    return mAssetClassProportionsMap;
}