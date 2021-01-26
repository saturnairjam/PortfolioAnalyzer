#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "Portfolio.hpp"

std::tuple<std::vector<float>, float, int, float> Drawdowns(const std::vector<float>& returns);

int WriteDrawdownsToCSV(
    const std::shared_ptr<Portfolio>& portfolio,
    const std::vector<float>& drawdowns,
    const std::string& filename);