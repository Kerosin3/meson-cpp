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
      boost::transform(boost::filesystem::directory_iterator(a_dir) | filtered(is_dir),std::back_inserter(directories_list),ret_name);
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
std::unordered_multimap<u_int32_t,std::string>
finder::Finder::getDuplicates(){
  for (auto& fpath : m_filespaths) {
    cout << "analyze file " << fpath << "\n";
  };
  cout << "ANALYZING DUPLICATES\n";
  std::unordered_multimap<u_int32_t,std::string> mapfilter {};
  // holders
  constexpr ssize_t BUF_SIZE = 4096;
  std::map<std::string,std::unique_ptr< Holder<BUF_SIZE>>> f_holders{};
  //buffer
  std::vector<char> buffer(BUF_SIZE);
  auto ptr = std::make_shared<std::vector<char>>(buffer);
  //factory
  FileFactory<BUF_SIZE> factory{ptr};
  //initialization
  for (auto& fpath : m_filespaths) {
    auto x = std::make_unique<Holder<BUF_SIZE>> (factory.getInstance(fpath));
  }
  if (!factory.initialize())
    throw 1;
  std::vector<std::string> store {};
  //initialization
  for (auto& fpath : m_filespaths) {
    f_holders[fpath] = (std::make_unique<Holder<BUF_SIZE>> (factory.getInstance(fpath)));
  };
    auto all_eof = [&]() {return std::all_of(f_holders.begin(),
                          f_holders.end(),
                          [&](auto& elem)
                          { return elem.second->getBlockHash() == 0; });};

    auto eof = [](auto& hash) { return hash.second->getBlockHash() != 0; };
    auto ieof = [](auto& hash) { return hash.second->getBlockHash() == 0; };
    do {

      std::ranges::for_each(
          f_holders | std::views::filter(eof),
          [&store, &mapfilter](auto& elem)
          {
            elem.second->calcBlockHash();
            auto prev_hash = elem.second->getPrevBlockHash();
            auto current_hash = elem.second->getBlockHash();
            auto& current_file = elem.first;
            printf("filename is %s, current hash is 0x%X, prev: 0x%X\n",
                   current_file.c_str(),
                   current_hash,
                   prev_hash);
            if (prev_hash != 0 && current_hash == 0) {
              mapfilter.insert({prev_hash, current_file});
            }
          });
      cout << "CYCLE\n";
    } while (!all_eof());

    std::erase_if(mapfilter,
                  [&](auto& elem) { return mapfilter.count(elem.first) == 1; });
    return mapfilter;
}

void
finder::Finder::printDuplicates(
    std::unordered_multimap< u_int32_t, std::string >&& duplicates )
{
  unsigned prev = 0xFFFF;
  ssize_t index {1};

  auto [begin, end] {duplicates.equal_range(0)};
  if (begin!=end){
    cout << "less than blocksize files:\n";
    std::for_each(
      begin, end, [](auto& x) { cout << "path:" << x.second << "\n"; });
  }
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