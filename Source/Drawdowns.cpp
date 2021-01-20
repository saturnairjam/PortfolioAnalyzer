#include <cmath>

#include "Drawdowns.hpp"

std::tuple<float, int, float> Drawdowns(const std::vector<float>& returns)
{
    float peakPortfolioValue = 1;

    float deepestDrawdown = 0;
    float sumOfDrawdownSquares = 0;
    int longestDrawdown = 0;

    int durationCounter = 0;
    int drawdownDuration = 0;

    for (const auto& entry : returns)
    {
        float portfolioValue = static_cast<float>(std::pow(1 + (entry / 100), (durationCounter + 1) / 12.0f));

        if (portfolioValue < peakPortfolioValue)
        {
            float drawdown = ((portfolioValue / peakPortfolioValue) - 1) * 100;

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
            peakPortfolioValue = portfolioValue;

            drawdownDuration = 0;
        }

        durationCounter++;
    }

    auto ulcerIndex = std::sqrt(sumOfDrawdownSquares / durationCounter);

    return std::make_tuple(deepestDrawdown, longestDrawdown, ulcerIndex);
}