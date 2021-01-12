#include <filesystem>

#include "AssetClassBuilder.hpp"

AssetClassBuilder::AssetClassBuilder(const rapidjson::Document& dom)
{
    // create asset class object

    mAssetClass = std::shared_ptr<AssetClass>(new AssetClass);

    // parse asset class parameters from DOM

    mAssetClass->mName = dom["Name"].GetString();
    mAssetClass->mStartDate.Year = dom["StartYear"].GetInt();
    mAssetClass->mStartDate.Month = dom["StartMonth"].GetInt();
    mAssetClass->mDurationInMonths = dom["NetAssetValueArray"].Size();

    // load NAV array & compute month-on-month growth

    float navPrev = 0;

    for (const auto& navEntry : dom["NetAssetValueArray"].GetArray())
    {
        auto navCurr = navEntry.GetFloat();

        mAssetClass->mMonthEndNavs.push_back(navCurr);

        if (navPrev)
        {
            mAssetClass->mMonthOnMonthGrowth.push_back((navCurr / navPrev) - 1);
        }

        navPrev = navCurr;
    }

    mConstructedSuccessfully = true;
}

std::shared_ptr<AssetClass> AssetClassBuilder::GetAssetClass()
{
    if (mConstructedSuccessfully)
    {
        return mAssetClass;
    }
    else
    {
        return nullptr;
    }
}