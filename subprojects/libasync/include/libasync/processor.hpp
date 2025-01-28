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
#include <fstream>


namespace Processor
{
using rsema_t = std::reference_wrapper<std::binary_semaphore>;
using dsema_t = std::reference_wrapper<std::binary_semaphore>;
using psema_t = std::reference_wrapper<std::counting_semaphore<1>>;

class DataProcessor;

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
  rsema_t readed_sem;
  psema_t proc_sem;
  dsema_t done_sem;
  std::thread thr;

public:
  InputProcessorParser(std::reference_wrapper<data_t> data,
                       size_t blocksize,
                       rsema_t rsem,
                       psema_t psem,
                       dsema_t dsem)
      : data_to_write(data)
      , m_BlockSize(blocksize)
      , readed_sem(rsem)
      , proc_sem(psem)
      , done_sem(dsem)
  {
  }
  void readInput(std::istringstream& ss);

  ~InputProcessorParser() { std::cout << "input done\n"; }
};

class Printer
{
  std::vector<std::string>& cmds;
  std::jthread out_thread;
  rsema_t readed_sem;
  psema_t proc_sem;

  Printer() = delete;

public:
  Printer(std::vector<std::string>& commands,
          rsema_t readed_sema,
          psema_t proc_sema)
      : cmds(commands)
      , readed_sem(readed_sema)
      , proc_sem(proc_sema)
  {
  }
  void initialize()
  {
    auto thr = std::jthread(
        [&]()
        {
          std::ofstream ofs("fout", std::ios_base::out | std::ios_base::trunc);
          auto writex = [&]()
          {
            for (auto& elem : cmds) {
              ofs << elem << " ";
            }
            ofs << "\n";
            ofs.flush();
          };
          for (;;) {
            readed_sem.get().acquire();
            // if (cmds.size() == 3) {
            std::cout << "block:";
            for (auto& elem : cmds) {
              std::cout << elem << " ";
              // }
              std::cout << "\n";
            }
            writex();
            cmds.clear();
            proc_sem.get().release();
          }
        });
  }
};

class DataProcessor
{
  size_t m_bulk_size {};
  std::counting_semaphore<1> processed {1};
  std::binary_semaphore input_readed {false};
  std::binary_semaphore input_done {false};
  std::vector<std::string> readed_data;
  InputProcessorParser input_parser {std::ref(readed_data),
                                     m_bulk_size,
                                     std::ref(input_readed),
                                     std::ref(processed),
                                     std::ref(input_done)};
  Printer printer1 {readed_data, input_readed, processed};
  std::thread input_thread;

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
    printer1.initialize();
    std::cout << "PROCEED\n";
    input_done.acquire();
  }
  void revc_cin()
  {
    std::istringstream ss;
    ss.basic_ios::rdbuf(std::cin.rdbuf());
    input_parser.readInput(ss);
    printer1.initialize();
    input_done.acquire();
  }

  ~DataProcessor()
  {
    std::cout << "destroy\n";
    // input_thread.join();
    // for (auto& str : readed_data) {
    // std::cout << "your string is " << str << "\n";
    // }
  }
};

}  // namespace Processor