#pragma once

#include <cstddef>
#include <format>
#include <iostream>
#include <string>
#include <vector>

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

  public:
    Finder() = delete;
    explicit Finder(vector< string > dirs)
        : m_direstories {std::move(dirs)} {};
    void setExcludeDirs(vector< string > dirs);
    void setMinSizeCounts(size_t size);
    void printInfo() const;
  };
}  // namespace finder

