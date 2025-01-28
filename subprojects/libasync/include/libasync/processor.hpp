#pragma once
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <format>
#include <functional>
#include <iostream>
#include <istream>
#include <semaphore>
#include <sstream>
#include <streambuf>
#include <string>
#include <thread>
#include <vector>

namespace Processor
{

class DataProcessor;

static inline std::binary_semaphore input_readed {false};

class Commands
{
public:
  Commands() = delete;
  ~Commands() = default;
  explicit Commands(std::string cmd)
      : m_Cmd {std::move(cmd)}
      , m_Timestamp {std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now())}
  {
  }
  [[nodiscard]] std::string getCmd() const { return std::format("{}", m_Cmd); }
  [[nodiscard]] std::string getTimestamp()
  {
    return std::format("{}{}", m_Timestamp, serial.fetch_add(1));
  }

private:
  std::string m_Cmd;
  time_t m_Timestamp;
  static inline std::atomic<long> serial {0};
};

class InputProcessorParser
{
  using data_t = std::vector<std::string>;
  std::reference_wrapper<data_t> data_to_write;
  size_t m_NOpenBracets {0};
  size_t m_NCloseBracets {0};
  size_t m_BlockSize {};
  std::vector<Commands> m_Commands;

public:
  InputProcessorParser(std::reference_wrapper<data_t> data, size_t blocksize)
      : data_to_write(data)
      , m_BlockSize(blocksize)
  {
  }
  void readInput(std::istringstream& ss);
};

class DataProcessor
{
  size_t m_bulk_size {};
  std::jthread input_thread;
  std::vector<std::string> readed_data;
  InputProcessorParser input_parser {std::ref(readed_data), m_bulk_size};

public:
  DataProcessor() = delete;
  explicit DataProcessor(size_t size)
      : m_bulk_size(size)
  {
  }
  // recv data
  void receive_input(std::string& string)
  {
    std::istringstream ss(string);
    input_parser.readInput(ss);
  }
  void revc_cin()
  {
    std::istringstream ss;
    ss.basic_ios::rdbuf(std::cin.rdbuf());
    input_parser.readInput(ss);
  }
  ~DataProcessor()
  {
    for (auto& str : readed_data) {
      std::cout << "your string is " << str << "\n";
    }
  }
};

}  // namespace Processor