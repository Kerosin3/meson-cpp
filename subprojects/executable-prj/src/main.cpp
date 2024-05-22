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
process_command_line(int ac, char** av)
{
  try {
    po::options_description desc("Allowed options");
    desc.add_options()("help", "produce help message")(
        "dirs,d",
        po::value< std::vector< std::string > >()->multitoken()->required(),
        "directoris to scan")("minfile",
                              po::value< unsigned >()->default_value(1),
                              "set min filesize to index")(
        "idirs,i",
        po::value< std::vector< std::string > >()->multitoken(),
        "set ignore directories");

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);

    if (vm.count("help")) {
      cout << desc << "\n";
      return false;
    }
    po::notify(vm);

    if (vm.count("dirs")) {
      std::vector< std::string > kvps =
          vm["dirs"].as< std::vector< std::string > >();
      for (const auto& elem : kvps) {
        cout << elem << ".\n";
      }
    } else {
      cout << "Directories not set.\n";
    }
    if (vm.count("idirs")) {
      std::vector< std::string > kvps =
          vm["idirs"].as< std::vector< std::string > >();
      for (const auto& elem : kvps) {
        cout << elem << ".\n";
      }
    } else {
      cout << "ignore directories not set.\n";
    }

  } catch (std::exception& e) {
    cerr << "error: " << e.what() << "\n";
    return true;
  } catch (...) {
    cerr << "Exception of unknown type!\n";
  }

  return false;
}

int
main(int argc, char** argv)
{
  process_command_line(argc, argv);
  return EXIT_SUCCESS;
}
