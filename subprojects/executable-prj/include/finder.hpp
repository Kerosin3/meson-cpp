#pragma once

#include <cstddef>
#include <format>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>
#include <ranges>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/regex.hpp>
#include <sys/types.h>
#include <boost/range/iterator_range.hpp>
#include "analyzer.hpp"

#include <boost/filesystem.hpp>

using std::cout;
using std::string;
using std::string_view;
using std::vector;

using namespace std::string_literals;
using namespace std::string_view_literals;


namespace finder
{
  class Finder
  {
  private:
    vector< string > m_direstories;
    vector< string > m_excluded_directories;
    size_t m_min_elem_size {1};
    size_t m_blocksize {1024};
    bool m_recursive_search {false};
    std::vector< string >  m_filespaths {};


  public:
    Finder() = delete;

    explicit Finder(vector< string > dirs)
        : m_direstories {std::move(dirs)} {};

    void setExcludeDirs(vector< string >&& dirs);

    void setMinSizeCounts(size_t size);

    void setupBlockSize(size_t size)
    {
      m_blocksize = size;
    }

    void setRecursiveSearch(bool arg)
    {
      m_recursive_search = arg;
    }
    void setupDirs();

    void setupFilesPaths();

    void setupTargetFiles(std::vector<std::string>&&);

    void setupFilter(std::string&&);

    void filterFilenames(std::string&&);

    void filerBySize();

    void printInfo();

    void printDuplicates(std::multimap<int32_t,std::string>&&);

    std::multimap<int32_t,std::string> getDuplicates();
  };
}  // namespace finder
