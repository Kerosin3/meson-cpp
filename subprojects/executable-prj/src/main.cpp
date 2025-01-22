#include <iostream>
#include <boost/program_options.hpp>
#include <fcntl.h>
#include <sys/resource.h>

namespace po = boost::program_options;

using std::cerr;
using std::cout;

bool setupFinder(int ac, char** av);

int
main(int argc, char** argv)
{
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
