#include <fstream>
#include <iostream>
#include <memory> // for std::shared_ptr
#include <vector>

#include "Portfolio.hpp"

std::shared_ptr<std::vector<std::vector<float>>>
RollingReturns(const std::shared_ptr<std::vector<std::vector<float>>> heatMap)
{
    // declare rolling returns matrix

    std::vector<std::vector<float>> rollingReturns;

    // iterate over each row in heat map (corresponds to a given start date)

    for (const auto& heatMapRow : *heatMap)
    {
        // access yearly heat map returns

        for (int monthsHeld = 12; monthsHeld <= heatMapRow.size(); monthsHeld += 12)
        {
            if (rollingReturns.size() < (monthsHeld / 12))
            {
                rollingReturns.push_back({});
            }

            rollingReturns[(monthsHeld / 12) - 1].push_back(heatMapRow[monthsHeld - 1]);
        }
    }

    return std::make_shared<std::vector<std::vector<float>>>(rollingReturns);
}

int WriteRollingReturnsToCSV(
    const std::shared_ptr<Portfolio> portfolio,
    const std::shared_ptr<std::vector<std::vector<float>>> rollingReturns,
    const std::string& filename)
{
    std::ofstream outputFile(filename);

    if (!outputFile.is_open())
    {
        std::cerr << "failed to open output file\n";

        return EXIT_FAILURE;
    }

    outputFile << ", Start Date\n";

    outputFile << "Years Held";

    int portfolioStartDate = (portfolio->GetStartDate().Year * 12) + (portfolio->GetStartDate().Month - 1);

    for (int ii = 0; ii < rollingReturns->at(0).size(); ii++, portfolioStartDate++)
    {
        outputFile << ", " << (portfolioStartDate / 12) << "-" << ((portfolioStartDate % 12) + 1);
    }

    outputFile << "\n";

    int yearsHeld = 1;

    for (const auto& row : *rollingReturns)
    {
        outputFile << yearsHeld++;

        for (const auto& entry : row)
        {
            outputFile << ", " << entry;
        }

        outputFile << "\n";
    }

    outputFile.close();

    return EXIT_SUCCESS;
}