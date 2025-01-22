#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>
#include <sys/types.h>

#include "crc32.hpp"


// crc32 hash function
inline uint32_t
crc32_calc(uint8_t* p, uint64_t bytelength)
{
  uint32_t crc = 0xffffffff;
  while (bytelength-- != 0) {
    crc = crc32Table[((uint8_t)crc ^ *(p++))] ^ (crc >> 8);
  }
  return (crc ^ 0xffffffff);
}


// chunk reader for Holder
inline std::pair< std::istreambuf_iterator< char >, ssize_t >
readChunk(std::istreambuf_iterator< char >& curr,
          std::vector< char >& vec,
          ssize_t blocksize)
{
  vec.clear();
  ssize_t readed {0};
  for (readed = 0;
       curr != std::istreambuf_iterator< char >() && readed < blocksize;
       ++readed, ++curr)
  {
    vec.emplace_back(*curr);
  }
  return {curr, readed};
}
// wrapper for file has calc and read
class Holder
{
  std::string m_filename {};
  std::shared_ptr< std::vector< char > > m_buf {};
  int32_t m_current_block_hash {0};
  int32_t m_prev_block_hash {0};
  std::ifstream m_file;
  inline static ssize_t m_blocksize {1024};
  ssize_t m_serial {0};

public:
  bool m_eof_reached {false};
  ssize_t m_iterations {0};

  Holder() = delete;

private:
  Holder(std::string& filename,
         ssize_t p_blocksize,
         std::shared_ptr< std::vector< char > > ptr)
      : m_filename {filename}
      , m_buf {ptr}
  {

    Holder::m_blocksize = p_blocksize;
  }

public:
  explicit Holder(std::string& filename,
                  std::shared_ptr< std::vector< char > > ptr,
                  ssize_t p_blocksize,
                  ssize_t serial)
      : Holder {filename, p_blocksize, ptr}
  {
    m_serial = serial;
    m_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    m_file.open(m_filename, std::ios::binary);
  }

  int32_t calcBlockHash()
  {
    if (m_eof_reached)
      return 0;
    auto iter = std::istreambuf_iterator< char >(m_file);
    auto [iresult, readed] = readChunk(iter, *m_buf, Holder::m_blocksize);
    m_prev_block_hash = m_current_block_hash;
    if (!readed) {
      m_eof_reached = true;
      m_current_block_hash = 0;
      return 0;
    }
    m_iterations++;
    m_current_block_hash =
        crc32_calc(reinterpret_cast< uint8_t* >(m_buf->data()), readed);
    return m_current_block_hash;
  }

  int32_t getBlockHash() const
  {
    return m_current_block_hash;
  }

  int32_t getPrevBlockHash() const
  {
    return m_prev_block_hash;
  }

  void showFilename() const
  {
    std::cout << "filename: " << m_filename << "\n";
  }

  std::string getFilename() const
  {
    return m_filename;
  }

  friend bool operator==(const Holder& h1, const Holder& h2)
  {
    return ((h1.getBlockHash() == h2.getBlockHash())
            && (h1.getPrevBlockHash() == h2.getPrevBlockHash()));
  }
};

// file open factory
class FileFactory
{
  std::shared_ptr< std::vector< char > > factory_buf;
  bool m_initialized {false};
  inline static ssize_t m_blocksize {1024};
  inline static ssize_t m_opened_files {0};

  FileFactory() = delete;

public:
  FileFactory(std::shared_ptr< std::vector< char > > buf, ssize_t p_blocksize)
      : factory_buf {buf}
  {
    FileFactory::m_blocksize = p_blocksize;
  }

  bool initialize()
  {
    if (factory_buf->size() < FileFactory::m_blocksize) {
      return false;
    }
    m_initialized = true;
    return true;
  }

  Holder getInstance(std::string& fname)
  {
    if (!std::filesystem::exists(fname))
      throw std::runtime_error(fname + " not exists");
    return Holder {
        fname, factory_buf, FileFactory::m_blocksize, ++m_opened_files};
  }
};