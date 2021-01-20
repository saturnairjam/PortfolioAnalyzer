#include <cmath>
#include <fstream>
#include <iostream>

#include "Drawdowns.hpp"

std::tuple<std::vector<float>, float, int, float> Drawdowns(const std::vector<float>& returns)
{
    float peakPortfolioValue = 1;

    float deepestDrawdown = 0;
    float sumOfDrawdownSquares = 0;
    int longestDrawdown = 0;

    int durationCounter = 0;
    int drawdownDuration = 0;

    std::vector<float> drawdowns;

    for (const auto& returnsEntry : returns)
    {
        float portfolioValue = static_cast<float>(std::pow(1 + (returnsEntry / 100), (durationCounter + 1) / 12.0f));

        if (portfolioValue < peakPortfolioValue)
        {
            float drawdown = ((portfolioValue / peakPortfolioValue) - 1) * 100;

            drawdowns.push_back(drawdown);

            if (drawdown < deepestDrawdown)
            {
                deepestDrawdown = drawdown;
            }

            sumOfDrawdownSquares += drawdown * drawdown;

            drawdownDuration++;

            if (drawdownDuration > longestDrawdown)
            {
                longestDrawdown = drawdownDuration;
            }
        }
        else
        {
            drawdowns.push_back(0.0f);

            peakPortfolioValue = portfolioValue;

            drawdownDuration = 0;
        }

        durationCounter++;
    }

    auto ulcerIndex = std::sqrt(sumOfDrawdownSquares / durationCounter);

    return std::make_tuple(drawdowns, deepestDrawdown, longestDrawdown, ulcerIndex);
}

int WriteDrawdownsToCSV(
    const std::shared_ptr<Portfolio> portfolio,
    const std::vector<float>& drawdowns,
    const std::string& filename)
{
    std::ofstream outputFile(filename);

    if (!outputFile.is_open())
    {
        std::cerr << "failed to open output file\n";

        return EXIT_FAILURE;
    }

    outputFile << "Date, Drawdown %\n";

    int date = (portfolio->GetStartDate().Year * 12) + (portfolio->GetStartDate().Month - 1) + 1;

    for (const auto& drawdown : drawdowns)
    {
        outputFile << (date / 12) << "-" << ((date % 12) + 1) << ", " << drawdown << "\n";

        date++;
    }

    outputFile.close();

    return EXIT_SUCCESS;
}