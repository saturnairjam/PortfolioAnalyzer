#include "AssetClass.hpp"

const std::string& AssetClass::GetName()
{
    return mName;
}

Date AssetClass::GetStartDate()
{
    return mStartDate;
}

int AssetClass::GetDuration()
{
    return mDurationInMonths;
}

const std::vector<float>& AssetClass::GetMonthEndNavs()
{
    return mMonthEndNavs;
}

const std::vector<float>& AssetClass::GetMonthOnMonthGrowth()
{
    return mMonthOnMonthGrowth;
}