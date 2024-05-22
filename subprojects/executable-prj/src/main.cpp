#include <cstdlib>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

using std::cerr;
using std::cout;

bool
process_command_line(int, char**);

int
main(int argc, char** argv)
{
  try {
    process_command_line(argc, argv);
  } catch (std::exception& e) {
    cerr << "error: " << e.what() << "\n";
    return EXIT_FAILURE;
  } catch (...) {
    cerr << "Exception of unknown type!\n";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
