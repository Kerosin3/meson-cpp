#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <ranges>
#include <filesystem>

#include "finder.hpp"

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/regex.hpp>
#include <sys/types.h>

namespace fs = boost::filesystem;

void
finder::Finder::setExcludeDirs(vector< string >&& e_dirs)
{
  m_excluded_directories = std::move(e_dirs);
  std::erase_if(m_direstories, [this](const auto& dpath) {
    for (const auto& exclude_dir: m_excluded_directories) {
      if (dpath == exclude_dir)
        return true;
      continue;
    }
    return false;
  });
}

void
finder::Finder::setupDirs(){
  using boost::adaptors::filtered;
  auto is_dir = [](auto& dir) { return dir.is_directory(); };
  auto ret_name = [](auto& dir){ return dir.path().native();};
  std::vector<std::string> directories_list {};
  for (const auto& a_dir : m_direstories) {

    if (!m_recursive_search) {
      directories_list.push_back(a_dir);
    } else {
      boost::transform(boost::filesystem::recursive_directory_iterator(a_dir) | filtered(is_dir),std::back_inserter(directories_list),ret_name);
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
  auto ret_name = [](auto& dir){ return dir.path().native();};
  for (const auto& a_dir : m_direstories) {
    if (!m_recursive_search) {
      boost::transform(boost::filesystem::directory_iterator(a_dir) | filtered(is_reg_file),std::back_inserter(m_filespaths),ret_name);
    } else {
      boost::transform(boost::filesystem::recursive_directory_iterator(a_dir) | filtered(is_reg_file),std::back_inserter(m_filespaths),ret_name);
    }
  }
   for (const auto& elem : m_filespaths) {
    cout << "Xyour filename is " << elem << "\n";
  }
}

void
finder::Finder::filerBySize()
{
  cout << "enter!\n";
  std::erase_if(m_filespaths, [this](const auto& fpath) {
   return std::filesystem::file_size(fpath) < m_min_elem_size;
  });
}

constexpr auto ToUpperCase(std::string msv) noexcept {
    std::transform(msv.begin(), msv.end(), msv.begin(), [](unsigned char c) -> unsigned char { return std::toupper(c); });
    return msv;
}
constexpr auto ToLowerCase(std::string msv) noexcept {
    std::transform(msv.begin(), msv.end(), msv.begin(), [](unsigned char c) -> unsigned char { return std::tolower(c); });
    return msv;
}

void
finder::Finder::filterFilenames(std::string&& filter_str){
  const auto& upper = ToUpperCase(filter_str);
  const auto& lower = ToLowerCase(filter_str);
  std::erase_if(m_filespaths, [&](const auto& fpath) {
      return (fpath.contains(filter_str) || fpath.contains(upper) || fpath.contains(lower) );
  });
}



void
finder::Finder::setupTargetFiles(std::vector<std::string>&& v_strings)
{
  for (const auto& file_to_analyze : v_strings) {
    // if regex there is no need for upper and lower
    std::erase_if(m_filespaths, [&](const auto& fpath) {
      // boost::regex pattern {file_to_analyze};
      // return !boost::regex_match(fs::path(fpath).filename().string(),pattern);
      return !(fs::path(fpath).filename().string().contains(file_to_analyze));
  });
  }
}

std::multimap< int32_t, std::string >
finder::Finder::getDuplicates()
{
  ssize_t BUF_SIZE = m_blocksize;
  for (auto& fpath : m_filespaths) {
    cout << "analyze file " << fpath << "\n";
  };
  cout << "ANALYZING DUPLICATES\n";
  std::multimap< int32_t, std::string > mapfilter {};
  // holders
  std::map< std::string, std::unique_ptr< Holder > > f_holders {};
  // buffer
  std::vector< char > buffer(BUF_SIZE);
  auto ptr = std::make_shared< std::vector< char > >(buffer);
  // factory
  FileFactory factory {ptr,BUF_SIZE};
  // initialization
  for (auto& fpath : m_filespaths) {
    auto x = std::make_unique< Holder>(factory.getInstance(fpath));
  }
  if (!factory.initialize())
    throw 1;
  std::vector< std::string > store {};
  // initialization
  for (auto& fpath : m_filespaths) {
    f_holders[fpath] =
        (std::make_unique< Holder >(factory.getInstance(fpath)));
  };
  auto all_eof = [&]()
  {
    return std::all_of(f_holders.begin(),
                       f_holders.end(),
                       [&](auto& elem)
                       {return elem.second->m_eof_reached ; });
  };
  auto files_tested = [&](auto& fname) {
     auto ranges { f_holders.equal_range(fname)};
     return std::all_of(ranges.first,ranges.second,[&](auto& elem){
      return elem.second->getBlockHash() == 0;
     });
  };
  auto eof = [](auto& hash) { return ! hash.second->m_eof_reached ; };
  do {
        mapfilter.clear();
    std::ranges::for_each(
        f_holders | std::views::filter(eof),
        [&mapfilter](auto& elem)
        {
          auto prev_hash = elem.second->getPrevBlockHash();
          elem.second->calcBlockHash();
          auto current_hash = elem.second->getBlockHash();
          auto& current_file = elem.first;
                   printf("filename is %s, current hash is 0x%X, prev: 0x%X\n",
                 current_file.c_str(),
                 current_hash,
                 prev_hash);
          // insert element
          if (current_hash != 0) {
            mapfilter.insert({current_hash, current_file});
          }
        });
        // erase single elements
        std::erase_if(mapfilter,
                [&](auto& elem) {
                   auto single_element = mapfilter.count(elem.first) == 1;
                   //remove from files list
                   if (single_element)
                     f_holders.erase(elem.second);
                   return single_element;
                   });
      //
        //iterate over >2 hashes
        for (auto it = mapfilter.begin(), end = mapfilter.end(); it != end;
             it = mapfilter.upper_bound(it->first))
        {
          printf("your hash is 0X%X\n", it->first);
          // get filenames per hash
          auto files {mapfilter.equal_range(it->first)};
          // iterage over simular files
          std::multimap< u_int32_t, std::string > d_filter {};
          // for each file
          for (auto same_file = files.first; same_file != files.second;
               ++same_file) {
            cout << "files are " << same_file->second << "\n";
            auto current_hash =
                f_holders.find(same_file->second)->second->calcBlockHash();
            //  insert hash
            d_filter.insert({current_hash, same_file->second});
          }
          std::erase_if(d_filter,
                        [&](auto& elem)
                        {
                          cout << "test\n";
                          auto single_element = d_filter.count(elem.first) == 1;
                          // auto is_zero = d_filter.find(0);
                          // remove from files list
                          if (single_element) {
                            f_holders.erase(elem.second);
                          }
                          return single_element;
                        });

          d_filter.clear();
        };

        cout << "CYCLE! " << all_eof() << " \n";
  } while (!all_eof());
  cout << "xxxxxxxxxxxxxxxx\n";
  std::erase_if(f_holders,
                [&](auto& elem) {
                  return (!elem.second->getBlockHash() && !elem.second->getPrevBlockHash());
                   });
  for (auto& rest : f_holders){
    cout << "left are " << rest.first << " prev hash is " << rest.second->getPrevBlockHash() << "current hash " << rest.second->getBlockHash() <<"\n";
  }
  return mapfilter;
}

void
finder::Finder::printDuplicates(
    std::multimap< int32_t, std::string >&& duplicates )
{
  unsigned prev = 0xFFFF;
  ssize_t index {1};

  for (const auto& [key, value] : duplicates) {
  if (key == prev || key == 0)
      continue;
  auto [begin, end] {duplicates.equal_range(key)};
  cout << index << "\'th duplicate:[" << key << "]\n";
  std::for_each(
      begin, end, [](auto& x) { cout << "path:" << x.second << "\n"; });
  prev = key;
  index++;
  }
}

void
finder::Finder::dropFile(const std::string& file){
  std::erase_if(m_filespaths, [&](const auto& fpath) {
    return fpath == file;
  });

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