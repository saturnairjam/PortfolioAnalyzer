#pragma once

#include <memory>

#include "Portfolio.hpp"

class PortfolioBuilder
{
public:
    PortfolioBuilder(const std::string& assetClassPath, const std::string& portfolioJsonFilename);

    ~PortfolioBuilder() = default;

    std::shared_ptr<Portfolio> GetPortfolio();

private:
    std::shared_ptr<Portfolio> mPortfolio;

    bool mConstructedSuccessfully {false};
};