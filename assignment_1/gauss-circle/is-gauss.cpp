#include "is-gauss.h"
#include <iostream>

int num_points(double r)
{

    long long count = 0;
    // Loop through area
    for (long x = 0; x <= r; x++)
    {
        for (long y = 0; y <= r; y++)
        {
            if (x * x + y * y <= r * r)
            {
                ++count;
            }
        }
    }
    // Multiplying by four for each quadrant
    count = 4 * count;

    // Adjust for points on the axes
    count -= ((4 * r) + 3);

    return count;
}