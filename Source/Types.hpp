#pragma once

struct Date
{
    int Year {0};
    int Month {1};
};

enum class RebalancingStrategy
{
    None,
    Periodic,
    Threshold
};