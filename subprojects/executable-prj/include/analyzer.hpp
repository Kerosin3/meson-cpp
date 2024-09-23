#pragma once

#include <cstddef>
#include <cstdint>
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <fstream>
#include <memory>
#include <filesystem>
#include <sys/types.h>
#include "crc32.hpp"



inline void DumpHex(const void* data, size_t size) {
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			printf(" ");
			if ((i+1) % 16 == 0) {
				printf("|  %s \n", ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					printf("   ");
				}
				printf("|  %s \n", ascii);
			}
		}
	}
}

inline uint32_t crc32_calc(uint8_t *p, uint64_t bytelength)
{
    uint32_t crc = 0xffffffff;
    while (bytelength-- != 0)
    {
        crc = crc32Table[((uint8_t)crc ^ *(p++))] ^ (crc >> 8);
    }
    return (crc ^ 0xffffffff);
}
inline std::pair<std::istreambuf_iterator<char>,ssize_t> readChunk(std::istreambuf_iterator<char>& curr, std::vector<char>& vec,ssize_t blocksize) {
    vec.clear();
    ssize_t readed {0};
    for (readed = 0; curr != std::istreambuf_iterator<char>() && readed < blocksize; ++readed, ++curr) {
        // std::cout << "data is " << *curr << "\n";
        vec.emplace_back(*curr);
    }
    // DumpHex(vec.data(), 10);
    return {curr,readed};
}

template<std::size_t BLOCKSIZE>
class Holder{
  std::ifstream m_file {};
  std::shared_ptr<std::vector<char>> m_buf;
  uint32_t m_current_block_hash {UINT32_MAX};
  uint32_t m_prev_block_hash {};
  bool m_is_valid {false};
  std::string m_filename{};

  public:

  Holder() = delete;

  Holder(std::shared_ptr<std::vector<char>> ptr) : m_buf{ptr}, m_is_valid{false}{}

  Holder(std::string& fname,std::shared_ptr<std::vector<char>> ptr) : m_file{fname}, m_buf{ptr}, m_is_valid{true}, m_filename{fname}{}

  uint32_t calcBlockHash()
  {
    auto iter = std::istreambuf_iterator<char> (m_file);
    auto [iresult,readed] = readChunk(iter, *m_buf, BLOCKSIZE);
    m_prev_block_hash = m_current_block_hash;
    if (!readed){
      m_current_block_hash = 0;
      return 0;
    }
	  m_current_block_hash =
        crc32_calc(reinterpret_cast< uint8_t* >(m_buf->data()), readed);
    return m_current_block_hash;
  }

  uint32_t getBlockHash()
  {
    return m_current_block_hash;
  }
  uint32_t getPrevBlockHash()
  {
    return m_prev_block_hash;
  }
  void showFilename() const {
    std::cout << "filename: " << m_filename << "\n";
  }
  std::string getFilename() const {
    return m_filename;
  }
};


template <size_t BLOCKSIZE>
class FileFactory{
	std::shared_ptr<std::vector<char>> factory_buf;
    bool m_initialized {false};

    FileFactory() = delete;
    public:
    FileFactory(std::shared_ptr<std::vector<char>> buf) : factory_buf{buf}{}

    bool initialize() {
      if (factory_buf->size() < BLOCKSIZE) {
        return false;
      }
      m_initialized = true;
      return true;
    }

    Holder<BLOCKSIZE> getInstance(std::string& fname){
      if(!std::filesystem::exists(fname))
        throw std::runtime_error(fname + " not exists" );
      if (std::filesystem::is_empty(fname))
          return Holder<BLOCKSIZE>{factory_buf};
      return Holder<BLOCKSIZE>{fname,factory_buf};
    }

};