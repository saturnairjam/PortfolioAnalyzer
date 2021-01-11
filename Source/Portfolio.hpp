#pragma once

#include <map>
#include <memory>
#include <string>

#include "AssetClass.hpp"
#include "Types.hpp"

class PortfolioBuilder; // forward declaration

class Portfolio
{
public:
    ~Portfolio() = default;

    const std::string& GetName();

    Date GetStartDate();
    int GetDuration();

    RebalancingStrategy GetRebalancingStrategy();
    int GetRebalancingPeriod();
    int GetRebalancingThreshold();

    const std::map<std::shared_ptr<AssetClass>, float>& GetAssetClassProportions();

    friend class PortfolioBuilder;

private:
    Portfolio() = default;

    std::string mName;

    Date mStartDate;
    int mDurationInMonths {0};

    RebalancingStrategy mRebalancingStrategy {RebalancingStrategy::Periodic};

    int mRebalancingPeriodInMonths {12};
    int mRebalancingThreshold {0};

    std::map<std::string, int> mAssetClassWeightsMap;

    std::map<std::shared_ptr<AssetClass>, float> mAssetClassProportionsMap;
};