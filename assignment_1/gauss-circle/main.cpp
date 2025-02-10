#include <iostream>
#include <cstdlib>
#include "is-gauss.h"

int main(int argc, char **argv)
{
    // Parse the radius from the command-line argument
    double r = std::atoi(argv[1]);

    long long count = num_points(r);

    std::cout << "Number of points for radius " << r << " -> " << count << std::endl;
}
