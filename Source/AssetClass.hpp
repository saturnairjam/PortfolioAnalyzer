#pragma once

#include <string>
#include <vector>

#include "Types.hpp"

class AssetClass
{
public:
    std::string GetName() { return mName; }

    Date GetStartDate() { return mStartDate; }
    int GetDuration() { return mDurationInMonths; }

    const std::vector<float>& GetMonthEndNavs() { return mMonthEndNavs; }
    const std::vector<float>& GetMonthOnMonthGrowth() { return mMonthOnMonthGrowth; }

private:
    std::string mName;

    Date mStartDate;
    int mDurationInMonths;

    std::vector<float> mMonthEndNavs;
    std::vector<float> mMonthOnMonthGrowth;
};