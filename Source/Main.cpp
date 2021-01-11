#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    // validate command-line argument count

    if (argc != 2)
    {
        std::cerr << "error: incorrect number of arguments!\n\n";

        std::cout << "Usage: " << std::string(argv[0]).substr(std::string(argv[0]).find_last_of("/\\") + 1)
                  << " [PORTFOLIO JSON FILE]\n";

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}