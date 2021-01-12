#include <filesystem>
#include <fstream>
#include <iostream>

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

#include "AssetClassBuilder.hpp"
#include "PortfolioBuilder.hpp"

PortfolioBuilder::PortfolioBuilder(const std::string& assetClassPath, const std::string& portfolioJsonFilename)
{
    // verify that asset class path is a directory

    if (!std::filesystem::is_directory(assetClassPath))
    {
        std::cerr << "not a directory: " << assetClassPath << "\n";

        return;
    }

    std::map<std::string, rapidjson::Document> assetClassDomMap;

    // iterate over all JSON files in asset class path

    for (const auto& file : std::filesystem::directory_iterator(assetClassPath))
    {
        std::cout << file.path() << "\n";

        // attempt to open asset class JSON file

        std::ifstream assetClassJsonFile(file.path());

        if (!assetClassJsonFile.is_open())
        {
            std::cerr << "failed to open: " << file.path() << "\n";

            return;
        }

        // load asset class JSON file into string object

        std::string assetClassJSON(
            (std::istreambuf_iterator<char>(assetClassJsonFile)), (std::istreambuf_iterator<char>()));

        // parse asset class JSON into DOM

        rapidjson::Document assetClassDOM;

        {
            rapidjson::ParseResult result = assetClassDOM.Parse(assetClassJSON.c_str());

            if (result.IsError())
            {
                std::cerr << "asset class JSON invalid: " << rapidjson::GetParseError_En(result.Code()) << "\n";

                return;
            }
        }

        // add (move) asset class DOM to map indexed by asset class name

        assetClassDomMap[assetClassDOM["Name"].GetString()] = std::move(assetClassDOM);
    }

    // attempt to open portfolio JSON file

    std::ifstream portfolioJsonFile(portfolioJsonFilename);

    if (!portfolioJsonFile.is_open())
    {
        std::cerr << "failed to open: " << portfolioJsonFilename << "\n";

        return;
    }

    // load portfolio JSON file into string object

    std::string portfolioJSON((std::istreambuf_iterator<char>(portfolioJsonFile)), (std::istreambuf_iterator<char>()));

    // parse portfolio JSON into DOM

    rapidjson::Document portfolioDOM;

    {
        rapidjson::ParseResult result = portfolioDOM.Parse(portfolioJSON.c_str());

        if (result.IsError())
        {
            std::cerr << "portfolio JSON invalid: " << rapidjson::GetParseError_En(result.Code()) << "\n";

            return;
        }
    }

    // create portfolio object

    mPortfolio = std::shared_ptr<Portfolio>(new Portfolio);

    // parse portfolio parameters from DOM

    mPortfolio->mName = portfolioDOM["Name"].GetString();

    std::cout << "Name: " << mPortfolio->mName << "\n";

    int totalWeight = 0;

    {
        const rapidjson::Value& assetClassWeightsArray = portfolioDOM["AssetClassWeights"];

        for (auto& assetClassEntry : assetClassWeightsArray.GetArray())
        {
            std::cout << "AssetClass: Name: " << assetClassEntry["Name"].GetString()
                      << ", Weight: " << assetClassEntry["Weight"].GetInt() << "\n";

            // add asset class weight to total

            totalWeight += assetClassEntry["Weight"].GetInt();

            // check if asset class exists in map

            auto it = assetClassDomMap.find(assetClassEntry["Name"].GetString());

            if (it == assetClassDomMap.end())
            {
                std::cerr << "non-existent asset class: " << assetClassEntry["Name"].GetString() << "\n";

                return;
            }

            // create asset class object

            AssetClassBuilder assetClassBuilder(it->second);

            auto assetClass = assetClassBuilder.GetAssetClass();

            if (!assetClass)
            {
                return;
            }

            // add asset class object to map along with weight

            mPortfolio->mAssetClassProportionsMap[assetClass] = assetClassEntry["Weight"].GetFloat();

            // recompute portfolio start date & duration based on asset class start date & duration

            auto portfolioStartDate = (mPortfolio->mStartDate.Year * 12) + (mPortfolio->mStartDate.Month - 1);
            auto portfolioEndDate = portfolioStartDate + mPortfolio->mDurationInMonths;

            auto assetClassStartDate = (assetClass->GetStartDate().Year * 12) + (assetClass->GetStartDate().Month - 1);
            auto assetClassEndDate = assetClassStartDate + assetClass->GetDuration();

            if ((assetClassEndDate < portfolioStartDate) || (portfolioEndDate < assetClassStartDate))
            {
                std::cerr << "non-overlapping date range\n";

                return;
            }

            if (assetClassStartDate > portfolioStartDate)
            {
                portfolioStartDate = assetClassStartDate;
            }

            if (assetClassEndDate < portfolioEndDate)
            {
                portfolioEndDate = assetClassEndDate;
            }

            mPortfolio->mStartDate.Year = portfolioStartDate / 12;
            mPortfolio->mStartDate.Month = (portfolioStartDate % 12) + 1;
            mPortfolio->mDurationInMonths = portfolioEndDate - portfolioStartDate;
        }

        // iterate through asset class map and replace weights with proportions

        for (auto& [object, weight] : mPortfolio->mAssetClassProportionsMap)
        {
            weight = weight / totalWeight;
        }
    }

    std::string rebalancingStrategy(portfolioDOM["RebalancingStrategy"].GetString());

    if (rebalancingStrategy == "Periodic")
    {
        mPortfolio->mRebalancingStrategy = RebalancingStrategy::Periodic;
    }
    else if (rebalancingStrategy == "Threshold")
    {
        mPortfolio->mRebalancingStrategy = RebalancingStrategy::Threshold;
    }
    else
    {
        std::cerr << "invalid rebalancing strategy: " << rebalancingStrategy << "\n";

        return;
    }

    std::cout << "RebalancingStrategy: " << rebalancingStrategy << "\n";

    mPortfolio->mRebalancingPeriodInMonths = portfolioDOM["RebalancingPeriodInMonths"].GetInt();

    if (mPortfolio->mRebalancingPeriodInMonths < 0)
    {
        std::cerr << "invalid rebalancing period: " << mPortfolio->mRebalancingPeriodInMonths << "\n";

        return;
    }

    std::cout << "RebalancingPeriodInMonths: " << mPortfolio->mRebalancingPeriodInMonths << "\n";

    mPortfolio->mRebalancingThreshold = portfolioDOM["RebalancingThreshold"].GetInt();

    if (mPortfolio->mRebalancingThreshold < 0)
    {
        std::cerr << "invalid rebalancing threshold: " << mPortfolio->mRebalancingThreshold << "\n";

        return;
    }

    std::cout << "RebalancingThreshold: " << mPortfolio->mRebalancingThreshold << "\n";

    mConstructedSuccessfully = true;
}

std::shared_ptr<Portfolio> PortfolioBuilder::GetPortfolio()
{
    if (mConstructedSuccessfully)
    {
        return mPortfolio;
    }
    else
    {
        return nullptr;
    }
}