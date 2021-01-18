#include <cstdlib>
#include <iostream>
#include <string>

#include "HeatMap.hpp"
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

    return EXIT_SUCCESS;
}