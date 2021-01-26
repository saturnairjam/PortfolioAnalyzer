#pragma once

#include <limits>
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

    const std::shared_ptr<AssetClass> GetInflation();

    friend class PortfolioBuilder;

private:
    Portfolio() = default;

    std::string mName;

    Date mStartDate;
    int mDurationInMonths {std::numeric_limits<int>::max()};

    RebalancingStrategy mRebalancingStrategy {RebalancingStrategy::Periodic};

    int mRebalancingPeriodInMonths {12};
    int mRebalancingThreshold {0};

    std::map<std::shared_ptr<AssetClass>, float> mAssetClassProportionsMap;

    std::shared_ptr<AssetClass> mInflation;
};