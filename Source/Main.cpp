#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

#include "Drawdowns.hpp"
#include "HeatMap.hpp"
#include "Percentile.hpp"
#include "PortfolioBuilder.hpp"
#include "RollingReturns.hpp"

int main(int argc, char** argv)
{
    // validate command-line argument count

    if (argc != 3)
    {
        std::cerr << "error: incorrect number of arguments!\n\n";

        std::cout << "Usage: " << std::string(argv[0]).substr(std::string(argv[0]).find_last_of("/\\") + 1)
                  << " [ASSET CLASS PATH] [PORTFOLIO JSON FILE]\n";

        return EXIT_FAILURE;
    }

    auto portfolioBuilder = std::make_shared<PortfolioBuilder>(std::string(argv[1]), std::string(argv[2]));

    auto portfolio = portfolioBuilder->GetPortfolio();

    if (!portfolio)
    {
        return EXIT_FAILURE;
    }

    // compute heat map

    auto heatMap = HeatMap(portfolio);

    // write heat map to CSV file

    if (const auto result = WriteHeatMapToCSV(portfolio, heatMap, "heatmap.csv"); result != EXIT_SUCCESS)
    {
        return result;
    }

    // compute rolling returns

    auto rollingReturns = RollingReturns(heatMap);

    // write rolling returns to CSV file

    if (const auto result = WriteRollingReturnsToCSV(portfolio, rollingReturns, "rollingreturns.csv");
        result != EXIT_SUCCESS)
    {
        return result;
    }

    // compute drawdowns

    auto [drawdowns, deepestDrawdown, longestDrawdown, ulcerIndex] = Drawdowns(heatMap[0]);

    std::cout << "Deepest Drawdown: " << deepestDrawdown << " %\n";
    std::cout << "Longest Drawdown: " << longestDrawdown << " months\n";
    std::cout << "Ulcer Index: " << ulcerIndex << "\n";

    // write drawdowns to CSV file

    if (const auto result = WriteDrawdownsToCSV(portfolio, drawdowns, "drawdowns.csv"); result != EXIT_SUCCESS)
    {
        return result;
    }

    // print stats of 10-year rolling returns

    {
        auto tenYearRollingReturns = rollingReturns[9];

        std::sort(tenYearRollingReturns.begin(), tenYearRollingReturns.end());

        auto min = tenYearRollingReturns[0];
        auto max = tenYearRollingReturns[tenYearRollingReturns.size() - 1];

        float baseline, median, stretch;

        if (const auto result = Percentile(tenYearRollingReturns, 0.15f, baseline); result != EXIT_SUCCESS)
        {
            return result;
        }

        if (const auto result = Percentile(tenYearRollingReturns, 0.50f, median); result != EXIT_SUCCESS)
        {
            return result;
        }

        if (const auto result = Percentile(tenYearRollingReturns, 0.85f, stretch); result != EXIT_SUCCESS)
        {
            return result;
        }

        std::cout << "10-year rolling returns: " << min << ", " << baseline << ", " << median << ", " << stretch << ", "
                  << max << "\n";
    }

    return EXIT_SUCCESS;
}