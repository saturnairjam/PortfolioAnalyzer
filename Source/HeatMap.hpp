#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Portfolio.hpp"

std::vector<std::vector<float>> HeatMap(const std::shared_ptr<Portfolio>& portfolio);

int WriteHeatMapToCSV(
    const std::shared_ptr<Portfolio>& portfolio,
    const std::vector<std::vector<float>>& heatMap,
    const std::string& filename);