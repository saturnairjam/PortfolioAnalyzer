#include <fstream>
#include <iostream>

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

#include "PortfolioBuilder.hpp"

PortfolioBuilder::PortfolioBuilder(char* jsonFile)
{
    // attempt to open JSON file

    std::ifstream portfolioJsonFile(jsonFile);

    if (!portfolioJsonFile.is_open())
    {
        std::cerr << "failed to open: " << jsonFile << "\n";

        return;
    }

    // load JSON file into string object

    std::string portfolioJSON((std::istreambuf_iterator<char>(portfolioJsonFile)), (std::istreambuf_iterator<char>()));

    // parse JSON into DOM

    rapidjson::Document portfolioDOM;

    {
        rapidjson::ParseResult result = portfolioDOM.Parse(portfolioJSON.c_str());

        if (result.IsError())
        {
            std::cerr << "portfolio JSON invalid: " << rapidjson::GetParseError_En(result.Code()) << "\n";

            return;
        }
    }

    // TODO:
    // - load schema JSON into DOM
    // - validate DOM against schema

    // create portfolio object

    mPortfolio = std::shared_ptr<Portfolio>(new Portfolio);

    // parse portfolio parameters from DOM

    mPortfolio->mName = portfolioDOM["Name"].GetString();

    std::cout << "Name: " << mPortfolio->mName << "\n";

    {
        const rapidjson::Value& assetClassWeightsArray = portfolioDOM["AssetClassWeights"];

        for (auto& assetClass : assetClassWeightsArray.GetArray())
        {
            std::cout << "AssetClass: Name: " << assetClass["Name"].GetString()
                      << ", Weight: " << assetClass["Weight"].GetInt() << "\n";
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