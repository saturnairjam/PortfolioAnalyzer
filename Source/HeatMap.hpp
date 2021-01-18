#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Portfolio.hpp"

std::shared_ptr<std::vector<std::vector<float>>> HeatMap(std::shared_ptr<Portfolio> portfolio);

int WriteHeatMapToCSV(
    const std::shared_ptr<Portfolio> portfolio,
    std::shared_ptr<std::vector<std::vector<float>>> heatMap,
    const std::string& filename);