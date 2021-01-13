#include <cmath> // for std::fabs()
#include <iostream>
#include <tuple>
#include <vector>

#include "Portfolio.hpp"

void HeatMap(std::shared_ptr<Portfolio> portfolio)
{
    // compute portfolio start date

    int portfolioStartDate = (portfolio->GetStartDate().Year * 12) + (portfolio->GetStartDate().Month - 1);

    // populate vector of tuples containing info for each asset class

    std::vector<std::tuple<std::shared_ptr<AssetClass>, float, float, int>> assetClasses;

    auto assetClassProportionsMap = portfolio->GetAssetClassProportions();

    for (const auto& [assetClass, proportion] : assetClassProportionsMap)
    {
        // compute asset class start date offset

        int assetClassStartDate = (assetClass->GetStartDate().Year * 12) + (assetClass->GetStartDate().Month - 1);

        int assetClassStartDateOffset = portfolioStartDate - assetClassStartDate;

        assetClasses.push_back(std::make_tuple(assetClass, proportion, proportion, assetClassStartDateOffset));
    }

    // compute number of months held

    int portfolioDuration = portfolio->GetDuration();

    int monthsHeld = portfolioDuration - 1;

    // declare heat map matrix

    std::vector<std::vector<float>> heatMap;

    // query & save rebalancing parameters

    auto rebalancingStrategy = portfolio->GetRebalancingStrategy();
    auto rebalancingPeriod = portfolio->GetRebalancingPeriod();
    auto rebalancingThreshold = static_cast<float>(portfolio->GetRebalancingThreshold()) / 100;

    // loop over number of months held in descending order

    int portfolioStartDateOffset = 0;

    while (monthsHeld-- > 0)
    {
        std::vector<float> heatMapRow;

        for (auto [runningOffset, rebalancingCounter] = std::tuple {portfolioStartDateOffset, 1};
             runningOffset < (portfolioDuration - 1);
             runningOffset++, rebalancingCounter++)
        {
            // iterate over asset classes to compute portfolio growth

            float portfolioGrowth = 0;

            for (const auto& [assetClass, runningProportion, originalProportion, assetClassStartDateOffset] :
                 assetClasses)
            {
                int offset = assetClassStartDateOffset + runningOffset;

                auto assetClassGrowth = assetClass->GetMonthOnMonthGrowth()[offset];

                portfolioGrowth += runningProportion * (1 + assetClassGrowth);
            }

            // recompute asset class proportions

            bool thresholdExceeded = false;

            for (auto& [assetClass, runningProportion, originalProportion, assetClassStartDateOffset] : assetClasses)
            {
                int offset = assetClassStartDateOffset + runningOffset;

                auto assetClassGrowth = assetClass->GetMonthOnMonthGrowth()[offset];

                runningProportion *= (1 + assetClassGrowth) / portfolioGrowth;

                // check if rebalancing threshold is exceeded

                if (std::fabs((runningProportion / originalProportion) - 1) > rebalancingThreshold)
                {
                    thresholdExceeded = true;
                }
            }

            // determine whether to rebalance

            if (((rebalancingStrategy == RebalancingStrategy::Periodic) &&
                 ((rebalancingCounter % rebalancingPeriod) == 0)) ||
                ((rebalancingStrategy == RebalancingStrategy::Threshold) && thresholdExceeded))
            {
                // reset asset class proportions to original values

                for (auto& [assetClass, runningProportion, originalProportion, offset] : assetClasses)
                {
                    runningProportion = originalProportion;
                }

                // reset rebalancing counter

                rebalancingCounter = 1;
            }

            // save portfolio growth to heat map row vector

            heatMapRow.push_back(portfolioGrowth - 1);
        }

        // save heat map row to heat map matrix

        heatMap.push_back(heatMapRow);

        portfolioStartDateOffset++;
    }
}