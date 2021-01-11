#pragma once

#include <memory>

#include "Portfolio.hpp"

class PortfolioBuilder
{
public:
    PortfolioBuilder() = default;
    ~PortfolioBuilder() = default;

    std::shared_ptr<Portfolio> GetPortfolio() { return mPortfolio; }

private:
    std::shared_ptr<Portfolio> mPortfolio;
};