#include <cstdlib>
#include <iostream>
#include <string>

#include "PortfolioBuilder.hpp"

void HeatMap(std::shared_ptr<Portfolio> portfolio);

int main(int argc, char** argv)
{
    // validate command-line argument count

    if (argc != 3)
    {
        std::cerr << "error: incorrect number of arguments!\n\n";

        std::cout << "Usage: " << std::string(argv[0]).substr(std::string(argv[0]).find_last_of("/\\") + 1)
                  << " [ASSET CLASS PATH] [PORTFOLIO JSON FILE]\n";

        return EXIT_FAILURE;
    }

    auto portfolioBuilder = std::make_shared<PortfolioBuilder>(std::string(argv[1]), std::string(argv[2]));

    auto portfolio = portfolioBuilder->GetPortfolio();

    if (!portfolio)
    {
        return EXIT_FAILURE;
    }

    HeatMap(portfolio);

    return EXIT_SUCCESS;
}