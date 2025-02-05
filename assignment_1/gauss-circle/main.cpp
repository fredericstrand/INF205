#include <iostream>
#include <cstdlib>
#include "is-gauss.h"

int main(int argc, char **argv)
{
    // Parse the radius from the command-line argument
    int r = std::atoi(argv[1]);

    int count = num_points(r);

    std::cout << "Number of points for radius " << r << " -> " << count << std::endl;
}
