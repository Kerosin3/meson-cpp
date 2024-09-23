#include <algorithm>
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
    ("idirs,i",po::value< std::vector< std::string > >()->multitoken(),"set exclude directories")
    ("level,l",po::value< bool >()->default_value(true),"recursive search")
    ("filter pattern,f",po::value< std::string >(),"set min filesize to index,bytes") // target files
    ("target files,t",po::value< std::vector< std::string > >()->multitoken(),"setup target files to analyze")
    ("blocksize,b",po::value< size_t >()->default_value(1024),"read block size,bytes");

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
    // setup Dirs
    app.setupDirs();
    // set ignore dirs
    if (vm.count("idirs")) {
      std::vector< std::string > idirs =
          vm["idirs"].as< std::vector< std::string > >();
      app.setExcludeDirs(std::move(idirs));
    } else {
      cout << "ignore directories are not set.\n";
    }
    // setup recursive search
    if (vm.count("level")) {
      auto rec_search = vm["level"].as< bool >();
      app.setRecursiveSearch(rec_search);
    }
    // setup min size and sort files
    auto minsize = vm["minfile"].as< size_t >();
    app.setMinSizeCounts(minsize);
    app.setupFilesPaths();
    app.filerBySize();
    auto blocksize = vm["blocksize"].as< size_t >();
    app.setupBlockSize(blocksize);
    // setup filterng
    if (vm.count("filter pattern")) {
      auto rec_search = vm["filter pattern"].as< std::string >();
      app.setupFilter(std::move(rec_search));
    } else {
      cout << "filtering is not set.\n"; //there is default !
    }
    //target files
    if (vm.count("target files")) {
      auto target_files = vm["target files"].as< std::vector< std::string > >();
      app.setupTargetFiles(std::move(target_files));
    } else {
      cout << "target files are not set!\n";
      //return false;
    }
    app.printDuplicates(app.getDuplicates());
    // app.printInfo();

  } catch (std::exception& e) {
    cerr << "error: " << e.what() << "\n";
    return true;
  } catch (...) {
    cerr << "Exception of unknown type!\n";
  }

  return false;
}