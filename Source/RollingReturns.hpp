#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Portfolio.hpp"

std::shared_ptr<std::vector<std::vector<float>>>
RollingReturns(const std::shared_ptr<std::vector<std::vector<float>>> heatMap);

int WriteRollingReturnsToCSV(
    const std::shared_ptr<Portfolio> portfolio,
    const std::shared_ptr<std::vector<std::vector<float>>> rollingReturns,
    const std::string& filename);