#include <memory> // for std::shared_ptr
#include <vector>

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