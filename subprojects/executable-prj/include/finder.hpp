#pragma once

#include <cstddef>
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
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
    bool m_recursive_search {false};
    std::vector< string >  m_filespaths {};


  public:
    Finder() = delete;

    void dropFile(const std::string&);
    explicit Finder(vector< string > dirs)
        : m_direstories {std::move(dirs)} {};

    void setExcludeDirs(vector< string >&& dirs);

    void setMinSizeCounts(size_t size);

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

    void printDuplicates(std::unordered_multimap<u_int32_t,std::string>&&);

    void execute();

    std::unordered_multimap<u_int32_t,std::string> getDuplicates();
  };
}  // namespace finder
