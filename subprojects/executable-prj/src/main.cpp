#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <thread>

#include "libexample/example.hpp"
#include "test1.hpp"

int
main(int argc, char* argv[])
{
  Myspace::Container Container1 {10};
  Container1.computeC(10);
  std::cout << Container1 << std::endl;
  auto Add = Container1 + 5;
  std::cout << Add << std::endl;
  return EXIT_SUCCESS;
}
