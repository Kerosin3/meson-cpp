#include <chrono>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <sstream>
#include <thread>

#include "test1.hpp"
#include "libexample/example.hpp"

int
main(int argc, char* argv[])
{
  someFnction();
  libexample::someFnction();
  return EXIT_SUCCESS;
}
