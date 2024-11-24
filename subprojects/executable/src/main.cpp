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

#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <boost/program_options.hpp>

#include "libasync/async.hpp"

int
main(int argc, char* argv[])
{

  async::func1();
  async::func2();
  return EXIT_SUCCESS;
}
