#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <memory>
#include <ranges>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "finder.hpp"

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/regex.hpp>
#include <sys/types.h>

namespace fs = boost::filesystem;

template< typename T >
void
exclude_common(std::vector< T >& a, std::vector< T >& b)
{
  std::unordered_multiset< T > st;
  st.insert(a.begin(), a.end());
  st.insert(b.begin(), b.end());
  auto predicate = [&st](const T& k) { return st.count(k) > 1; };
  a.erase(std::remove_if(a.begin(), a.end(), predicate), a.end());
  b.erase(std::remove_if(b.begin(), b.end(), predicate), b.end());
}

void
finder::Finder::setExcludeDirs(vector< string >&& e_dirs)
{
  m_excluded_directories = std::move(e_dirs);
  std::vector< std::string > files_to_exclude {};
  using boost::adaptors::filtered;
  auto is_reg_file = [](auto& dir) { return dir.is_regular_file(); };
  auto ret_name = [](auto& dir) { return dir.path().native(); };
  for (const auto& a_dir : m_excluded_directories) {
    boost::transform(boost::filesystem::recursive_directory_iterator(a_dir)
                         | filtered(is_reg_file),
                     std::back_inserter(files_to_exclude),
                     ret_name);
  }
  exclude_common(m_filespaths, files_to_exclude);
}

void
finder::Finder::setupDirs()
{
  using boost::adaptors::filtered;
  auto is_dir = [](auto& dir) { return dir.is_directory(); };
  auto ret_name = [](auto& dir) { return dir.path().native(); };
  std::vector< std::string > directories_list {};
  for (const auto& a_dir : m_direstories) {

    if (!m_recursive_search) {
      directories_list.push_back(a_dir);
    } else {
      boost::transform(boost::filesystem::recursive_directory_iterator(a_dir)
                           | filtered(is_dir),
                       std::back_inserter(directories_list),
                       ret_name);
    }
  }
  m_direstories = std::move(directories_list);
}

void

finder::Finder::setMinSizeCounts(size_t size)
{
  m_min_elem_size = size;
}

void
finder::Finder::printInfo()
{
  cout << "INFO:\n";
  cout << "directories:\n";
  for (size_t i = 0; const auto& elem : m_direstories) {
    auto fmt = std::format("elem {} = {}\n", i, elem);
    cout << fmt;
    i++;
  }
  cout << "exclude directories:\n";
  for (size_t i = 0; const auto& elem : m_excluded_directories) {
    auto fmt = std::format("elem {} = {}\n", i, elem);
    cout << fmt;
    i++;
  }
  cout << "files in directories:\n";
  for (const auto& elem : m_filespaths) {
    cout << "your filename is " << elem << "\n";
  }
}

void
finder::Finder::setupFilesPaths()
{
  using boost::adaptors::filtered;
  auto is_reg_file = [](auto& dir) { return dir.is_regular_file(); };
  auto ret_name = [](auto& dir) { return dir.path().native(); };
  for (const auto& a_dir : m_direstories) {
    if (!m_recursive_search) {
      boost::transform(
          boost::filesystem::directory_iterator(a_dir) | filtered(is_reg_file),
          std::back_inserter(m_filespaths),
          ret_name);
    } else {
      boost::transform(boost::filesystem::recursive_directory_iterator(a_dir)
                           | filtered(is_reg_file),
                       std::back_inserter(m_filespaths),
                       ret_name);
    }
  }
}

void
finder::Finder::filerBySize()
{
  cout << "enter!\n";
  std::erase_if(
      m_filespaths,
      [this](const auto& fpath)
      { return std::filesystem::file_size(fpath) < m_min_elem_size; });
}

constexpr auto
ToUpperCase(std::string msv) noexcept
{
  std::transform(msv.begin(),
                 msv.end(),
                 msv.begin(),
                 [](unsigned char c) -> unsigned char
                 { return std::toupper(c); });
  return msv;
}

constexpr auto
ToLowerCase(std::string msv) noexcept
{
  std::transform(msv.begin(),
                 msv.end(),
                 msv.begin(),
                 [](unsigned char c) -> unsigned char
                 { return std::tolower(c); });
  return msv;
}

void
finder::Finder::filterFilenames(std::string&& filter_str)
{
  const auto& upper = ToUpperCase(filter_str);
  const auto& lower = ToLowerCase(filter_str);
  std::erase_if(m_filespaths,
                [&](const auto& fpath)
                {
                  return (fpath.contains(filter_str) || fpath.contains(upper)
                          || fpath.contains(lower));
                });
}

void
finder::Finder::setupTargetFiles(std::vector< std::string >&& v_strings)
{
  for (const auto& file_to_analyze : v_strings) {
    // if regex there is no need for upper and lower
    std::erase_if(m_filespaths,
                  [&](const auto& fpath)
                  {
                    // boost::regex pattern {file_to_analyze};
                    // return
                    // !boost::regex_match(fs::path(fpath).filename().string(),pattern);
                    return !(fs::path(fpath).filename().string().contains(
                        file_to_analyze));
                  });
  }
}

std::multimap< int32_t, std::string >
finder::Finder::getDuplicates()
{
  ssize_t BUF_SIZE = m_blocksize;
  cout << "ANALYZING DUPLICATESx\n";
  std::multimap< int32_t, std::string > mapfilter {};
  // holders
  std::map< std::string, std::unique_ptr< Holder > > f_holders {};
  // buffer
  std::vector< char > buffer(BUF_SIZE);
  auto ptr = std::make_shared< std::vector< char > >(buffer);
  // factory
  FileFactory factory {ptr, BUF_SIZE};
  if (!factory.initialize())
    throw 1;
  std::vector< std::string > store {};
  // initialization
  for (auto& fpath : m_filespaths) {
    try {
      auto an_instance = std::make_unique< Holder >(factory.getInstance(fpath));
      f_holders[fpath] = std::move(an_instance);
    } catch (const std::ios_base::failure& e) {
      std::cerr << e.code().message() << "\n";
      std::cerr << "error: " << std::strerror(errno) << "\n";
    } catch (...) {
      std::cerr << "catched unknown exception\n";
    }
  };
  auto all_eof = [&]()
  {
    return std::all_of(f_holders.begin(),
                       f_holders.end(),
                       [&](auto& elem) { return elem.second->m_eof_reached; });
  };
  auto eof = [](auto& hash) { return !hash.second->m_eof_reached; };
  do {
    mapfilter.clear();
    std::ranges::for_each(f_holders | std::views::filter(eof),
                          [&mapfilter](auto& elem)
                          {
                            elem.second->calcBlockHash();
                            auto current_hash = elem.second->getBlockHash();
                            auto& current_file = elem.first;
                            // insert non empty files hash
                            if (current_hash != 0) {
                              mapfilter.insert({current_hash, current_file});
                            }
                          });
    // erase unique files
    std::erase_if(mapfilter,
                  [&](auto& elem)
                  {
                    auto single_element = mapfilter.count(elem.first) == 1;
                    if (single_element)
                      f_holders.erase(elem.second);
                    return single_element;
                  });
    // iterate over same hashes block files
    for (auto it = mapfilter.begin(), end = mapfilter.end(); it != end;
         it = mapfilter.upper_bound(it->first))
    {
      auto files {mapfilter.equal_range(it->first)};
      std::multimap< u_int32_t, std::string > d_filter {};
      for (auto same_file = files.first; same_file != files.second; ++same_file)
      {
        // calc next hash block
        auto current_hash =
            f_holders.find(same_file->second)->second->calcBlockHash();
        d_filter.insert({current_hash, same_file->second});
      }
      // remove unique
      std::for_each(d_filter.begin(),
                    d_filter.end(),
                    [&](auto& elem)
                    {
                      bool erase_elem = (d_filter.count(elem.first) == 1);
                      if (erase_elem) {
                        f_holders.erase(elem.second);
                      }
                    });

      d_filter.clear();
    };

    cout << "CYCLE! " << all_eof() << " \n";
  } while (!all_eof());
  mapfilter.clear();
  for (auto& rest : f_holders) {
    cout << "left are " << rest.first << " prev hash is "
         << rest.second->getPrevBlockHash() << "current hash "
         << rest.second->getBlockHash() << "count "
         << f_holders.count(rest.first) << "\n";
    mapfilter.insert(
        {rest.second->getPrevBlockHash() + rest.second->m_iterations,
         rest.first});
  }
  for (auto& [k, v] : mapfilter) {
    cout << "ker is " << k << " value is " << v << "\n";
  }
  //  std::erase_if(mapfilter,
  // [&](auto& elem) {
  // return ( mapfilter.count(elem.first) <= 1);
  //  });

  std::cout << "OUT!!!!\n";
  return mapfilter;
}

void
finder::Finder::printDuplicates(
    std::multimap< int32_t, std::string >&& duplicates)
{
  cout << "duplicates are:\n";
  unsigned prev = 0xFFFF;
  ssize_t index {1};

  for (const auto& [key, value] : duplicates) {
    if (key == prev)
      continue;
    auto [begin, end] {duplicates.equal_range(key)};
    if (duplicates.count(key) == 1)
      continue;
    cout << index << "\'th duplicate:[" << key << "]\n";
    std::for_each(
        begin, end, [](auto& x) { cout << "path:" << x.second << "\n"; });
    prev = key;
    index++;
  }
}

void
finder::Finder::dropFile(const std::string& file)
{
  std::erase_if(m_filespaths, [&](const auto& fpath) { return fpath == file; });
}

void
finder::Finder::setupFilter(std::string&& filter_str)
{
  if (!filter_str.empty()) {
    filterFilenames(std::move(filter_str));
  }
}

void
finder::Finder::execute()
{
  // getFilenames(this->m_direstories);
}