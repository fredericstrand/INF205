#include <iostream>
#include <cstdlib>
#include "only-is-prime.h"

int main(int argc, char **argv)
/*
   Arguments:
   -  argc: is the count of command-line arguments
   -  argv: is an array of strings representing the arguments
   -  argv[0] is the program name, while argv[1] is the actual argument
*/
{
   // Check if the user has provided a number
   if (argc < 2)
   {
      std::cerr << "Usage: " << argv[0] << " <number>\n";
      return 1;
   }
   // Convert input to an int
   int x = std::atoi(argv[1]);
   if (x == 0 && argv[1][0] != '0')
   {
      std::cerr << "Invalid number: " << argv[1] << "\n";
      return 1;
   }
   // Check if number is a prime number
   if (is_prime(x))
      std::cout << x << " is prime.\n";
   else
      std::cout << x << " is not prime.\n";

   return 0;
}
