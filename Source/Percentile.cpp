#include <algorithm> // for std::sort()
#include <cmath> // for std::ceil() & std::floor()
#include <iostream>
#include <vector>

int Percentile(std::vector<float> array, float percentile, float& percentileValue)
{
    if ((percentile < 0) || (percentile > 1))
    {
        std::cerr << "invalid percentile: " << percentile << "; must be in the range 0-1\n";

        return EXIT_FAILURE;
    }

    std::sort(array.begin(), array.end());

    float rank = percentile * (array.size() - 1);

    int indexA = static_cast<int>(std::floor(rank));
    int indexB = static_cast<int>(std::ceil(rank));

    percentileValue = array[indexA] + ((rank - std::floor(rank)) * (array[indexB] - array[indexA]));

    return EXIT_SUCCESS;
}