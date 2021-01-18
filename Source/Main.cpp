#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "PortfolioBuilder.hpp"

std::shared_ptr<std::vector<std::vector<float>>> HeatMap(std::shared_ptr<Portfolio> portfolio);
std::shared_ptr<std::vector<std::vector<float>>>
RollingReturns(const std::shared_ptr<std::vector<std::vector<float>>> heatMap);

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

    std::ofstream outputFile("heatmap.csv");

    if (!outputFile.is_open())
    {
        std::cerr << "failed to open output file\n";

        return EXIT_FAILURE;
    }

    int portfolioStartDate = (portfolio->GetStartDate().Year * 12) + (portfolio->GetStartDate().Month - 1);

    int monthsHeld = portfolio->GetDuration() - 1;

    outputFile << "Start Date";

    for (int ii = 1; ii <= monthsHeld; ii++)
    {
        outputFile << ", " << ii;
    }

    outputFile << "\n";

    for (const auto& row : *heatMap)
    {
        int year = portfolioStartDate / 12;
        int month = (portfolioStartDate % 12) + 1;

        outputFile << year << "-" << month;

        for (const auto& entry : row)
        {
            outputFile << ", " << entry << "%";
        }

        portfolioStartDate++;

        outputFile << "\n";
    }

    outputFile.close();

    // compute rolling returns

    auto rollingReturns = RollingReturns(heatMap);

    return EXIT_SUCCESS;
}