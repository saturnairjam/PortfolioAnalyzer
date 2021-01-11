#pragma once

#include <memory>

#include "Portfolio.hpp"

class PortfolioBuilder
{
public:
    PortfolioBuilder(char* jsonFile);
    ~PortfolioBuilder() = default;

    std::shared_ptr<Portfolio> GetPortfolio();

private:
    std::shared_ptr<Portfolio> mPortfolio;

    bool mConstructedSuccessfully {false};
};