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
std::vector<std::string>
finder::Finder::getDuplicates(){
  cout << "DUPLICATES\n";
  std::vector<std::string> duplicates{};
  std::unordered_multimap<u_int32_t,std::string> mapfilter {};
  // holders
  std::map<std::string,std::unique_ptr< Holder<10>>> f_holders{};
  //buffer
  std::vector<char> buffer(100);
  auto ptr = std::make_shared<std::vector<char>>(buffer);
  //factory
  FileFactory<10> factory{ptr};
  //initialization
  for (auto& fpath : m_filespaths) {
    auto x = std::make_unique<Holder<10>> (factory.getInstance(fpath));
  }
  if (!factory.initialize())
    throw 1;
  //initialization
  for (auto& fpath : m_filespaths) {
    f_holders[fpath] = (std::make_unique<Holder<10>> (factory.getInstance(fpath)));
    f_holders[fpath].get()->showFilename();
    auto hash = f_holders[fpath].get()->calcBlockHash();
    printf("crc32 is 0x%X\n", hash);
    mapfilter.insert({hash,fpath});
  };

  std::erase_if(mapfilter, [&] (auto& elem)
  {
    return mapfilter.count(elem.first) == 1;
  }
  );
  for (const auto& [key,value]: mapfilter) {
    cout << "--key is "<< key << " value is " << value << "\n";
  }
  for (auto& file : mapfilter) {
    duplicates.emplace_back(file.second);
  }
  return duplicates;
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