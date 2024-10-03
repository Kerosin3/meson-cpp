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
#include <fcntl.h>
#include <sys/resource.h>

namespace po = boost::program_options;

using std::bad_any_cast;
using std::cerr;
using std::cout;

bool setupFinder(int ac, char** av);

int
main(int argc, char** argv)
{
  cout << "Start program!\n";
  struct rlimit rlim;
  int err;
  err = getrlimit(RLIMIT_NOFILE, &rlim);
  if (err < 0) {
    perror("getrlimit");
    exit(1);
  }
  rlim.rlim_cur = 65535;
  err = setrlimit(RLIMIT_NOFILE,&rlim);
  if (err < 0) {
    perror("setrlimit");
    exit(1);
  }
  setupFinder(argc,argv);
  return EXIT_SUCCESS;
}
