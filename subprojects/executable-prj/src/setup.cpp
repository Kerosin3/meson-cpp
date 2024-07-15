#include <string>
#include "setup.hpp"
#include "finder.hpp"

bool
setupFinder(int ac, char** av)
{
  try {
    po::options_description desc("Allowed options");
    desc.add_options()("help", "produce help message")
    ("dirs,d",po::value< std::vector< std::string > >()->multitoken()->required(),"directoris to scan")
    ("minfile,m",po::value< size_t >()->default_value(1),"set min filesize to index,bytes")
    ("idirs,i",po::value< std::vector< std::string > >()->multitoken(),"set exclude directories");

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);

    if (vm.count("help")) {
      cout << desc << "\n";
      return false;
    }
    po::notify(vm);

    std::vector< std::string > dirs {};
    // set dirs
    if (vm.count("dirs")) {
      dirs = vm["dirs"].as< std::vector< std::string > >();
    } else {
      cout << "Directories not set.\n";
    }
    // setup Finder
    auto app {Finder {dirs}};

    app.printInfo();
    // set ignore dirs
    if (vm.count("idirs")) {
      std::vector< std::string > idirs =
          vm["idirs"].as< std::vector< std::string > >();
      app.setExcludeDirs(idirs);
    } else {
      cout << "ignore directories are not set.\n";
    }
    // setup min size
    if (vm.count("minfile")) {
      auto minsize = vm["minfile"].as< size_t >();
      app.setMinSizeCounts(minsize);
    } else {
      cout << "ignore directories are not set.\n";
    }

  } catch (std::exception& e) {
    cerr << "error: " << e.what() << "\n";
    return true;
  } catch (...) {
    cerr << "Exception of unknown type!\n";
  }

  return false;
}