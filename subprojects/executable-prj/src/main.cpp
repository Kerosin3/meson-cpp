#include <any>
#include <array>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <expected>
#include <format>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <map>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

using std::bad_any_cast;
using std::cerr;
using std::cout;

bool setupFinder(int ac, char** av);

int
main(int argc, char** argv)
{
  cout << "Start program!\n";
  setupFinder(argc,argv);
  return EXIT_SUCCESS;
}
