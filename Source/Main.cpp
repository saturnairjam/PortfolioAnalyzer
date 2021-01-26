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

    // construct portfolio object using builder class

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

    // compute & print baseline 10-year return

    float baselineReturn;

    if (const auto result = Percentile(rollingReturns[9], 0.15f, baselineReturn); result != EXIT_SUCCESS)
    {
        return result;
    }

    std::cout << "Baseline 10-year Return: " << baselineReturn << " %\n";

    // compute & print drawdown stats

    auto [drawdowns, deepestDrawdown, longestDrawdown, ulcerIndex] = Drawdowns(heatMap[0]);

    std::cout << "Deepest Drawdown: " << deepestDrawdown << " %\n";
    std::cout << "Longest Drawdown: " << longestDrawdown << " months\n";
    std::cout << "Ulcer Index: " << ulcerIndex << "\n";

    // write drawdowns to CSV file

    if (const auto result = WriteDrawdownsToCSV(portfolio, drawdowns, "drawdowns.csv"); result != EXIT_SUCCESS)
    {
        return result;
    }

    return EXIT_SUCCESS;
}