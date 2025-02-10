#include <iostream>
#include <cstdlib>
#include "only-is-prime.h"

int main(int argc, char **argv)
{

   int x = std::atoi(argv[1]);
   // Check if number is a prime number
   if (is_prime(x))
      std::cout << x << " is prime.\n";
   else
      std::cout << x << " is not prime.\n";

   return 0;
}
