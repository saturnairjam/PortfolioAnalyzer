#include <cmath> // for std::fabs(), std::pow()
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

#include "CagrHeatMap.hpp"
#include "Portfolio.hpp"

std::vector<std::vector<float>> CagrHeatMap(const std::shared_ptr<Portfolio>& portfolio)
{
    // compute portfolio start date

    int portfolioStartDate = (portfolio->GetStartDate().Year * 12) + (portfolio->GetStartDate().Month - 1);

    // populate vector of tuples containing info for each asset class

    std::vector<std::tuple<std::shared_ptr<AssetClass>, float, float, int>> assetClasses;

    auto& assetClassProportionsMap = portfolio->GetAssetClassProportions();

    for (const auto& [assetClass, proportion] : assetClassProportionsMap)
    {
        // compute asset class start date offset

        int assetClassStartDate = (assetClass->GetStartDate().Year * 12) + (assetClass->GetStartDate().Month - 1);

        int assetClassStartDateOffset = portfolioStartDate - assetClassStartDate;

        assetClasses.push_back(std::make_tuple(assetClass, proportion, proportion, assetClassStartDateOffset));
    }

    // compute inflation start date & offset

    auto inflation = portfolio->GetInflation();

    int inflationStartDate = 0, inflationStartDateOffset = 0;

    if (inflation)
    {
        inflationStartDate = (inflation->GetStartDate().Year * 12) + (inflation->GetStartDate().Month - 1);

        inflationStartDateOffset = portfolioStartDate - inflationStartDate;
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
        // std::cout << (portfolioStartDate + portfolioStartDateOffset) / 12 << "-"
        //          << ((portfolioStartDate + portfolioStartDateOffset) % 12) + 1 << ":";

        std::vector<float> heatMapRow;

        float normalizedPortfolioValue = 1;

        int monthCounter = 1;

        for (auto [runningOffset, rebalancingCounter] = std::tuple {portfolioStartDateOffset, 0};
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

            normalizedPortfolioValue *= portfolioGrowth;

            // recompute asset class proportions

            bool thresholdExceeded = false;

            for (auto& [assetClass, runningProportion, originalProportion, assetClassStartDateOffset] : assetClasses)
            {
                int offset = assetClassStartDateOffset + runningOffset;

                auto assetClassGrowth = assetClass->GetMonthOnMonthGrowth()[offset];

                runningProportion *= (1 + assetClassGrowth) / portfolioGrowth;

                // check if rebalancing threshold is exceeded

                if (std::fabs(runningProportion - originalProportion) > rebalancingThreshold)
                {
                    thresholdExceeded = true;

                    break;
                }
            }

            // determine whether to rebalance

            if (((rebalancingStrategy == RebalancingStrategy::Periodic) && (runningOffset > portfolioStartDateOffset) &&
                 ((rebalancingCounter % rebalancingPeriod) == 0)) ||
                ((rebalancingStrategy == RebalancingStrategy::Threshold) && thresholdExceeded))
            {
                // std::cout << " " << (portfolioStartDate + runningOffset) / 12 << "-"
                //          << ((portfolioStartDate + runningOffset) % 12) + 1;

                // reset asset class proportions to original values

                for (auto& [assetClass, runningProportion, originalProportion, offset] : assetClasses)
                {
                    runningProportion = originalProportion;
                }

                // reset rebalancing counter

                rebalancingCounter = 0;
            }

            // adjust for inflation

            if (inflation)
            {
                int offset = inflationStartDateOffset + runningOffset;

                auto inflationValue = inflation->GetMonthEndNavs()[offset];

                normalizedPortfolioValue /= portfolioGrowth; // reset

                portfolioGrowth /= (1 + inflationValue);

                normalizedPortfolioValue *= portfolioGrowth;
            }

            // compute CAGR

            float cagr = std::pow(normalizedPortfolioValue, 12.0f / monthCounter++) - 1;

            // save CAGR to heat map row vector

            heatMapRow.push_back(cagr * 100);
        }

        // save heat map row to heat map matrix

        heatMap.push_back(heatMapRow);

        portfolioStartDateOffset++;

        // std::cout << "\n";
    }

    return heatMap;
}

int WriteHeatMapToCSV(
    const std::shared_ptr<Portfolio>& portfolio,
    const std::vector<std::vector<float>>& heatMap,
    const std::string& filename)
{
    std::ofstream outputFile(filename);

    if (!outputFile.is_open())
    {
        std::cerr << "failed to open output file\n";

        return EXIT_FAILURE;
    }

    int portfolioStartDate = (portfolio->GetStartDate().Year * 12) + (portfolio->GetStartDate().Month - 1);

    int monthsHeld = portfolio->GetDuration() - 1;

    outputFile << ", Years Held\n";

    outputFile << "Start Date";

    for (int ii = 1; ii <= monthsHeld; ii++)
    {
        outputFile << ", " << ii;
    }

    outputFile << "\n";

    for (const auto& row : heatMap)
    {
        outputFile << (portfolioStartDate / 12) << "-" << ((portfolioStartDate % 12) + 1);

        for (const auto& entry : row)
        {
            outputFile << ", " << entry << "%";
        }

        portfolioStartDate++;

        outputFile << "\n";
    }

    outputFile.close();

    return EXIT_SUCCESS;
}