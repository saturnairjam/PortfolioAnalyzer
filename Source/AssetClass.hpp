#pragma once

#include <string>
#include <vector>

#include "Types.hpp"

class AssetClassBuilder; // forward declaration

class AssetClass
{
public:
    ~AssetClass() = default;

    const std::string& GetName();

    Date GetStartDate();
    int GetDuration();

    const std::vector<float>& GetMonthEndNavs();
    const std::vector<float>& GetMonthOnMonthGrowth();

    friend class AssetClassBuilder;

private:
    AssetClass() = default;

    std::string mName;

    Date mStartDate;
    int mDurationInMonths;

    std::vector<float> mMonthEndNavs;
    std::vector<float> mMonthOnMonthGrowth;
};