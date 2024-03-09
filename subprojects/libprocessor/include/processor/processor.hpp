#pragma once
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <format>
#include <functional>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <utility>
#include <vector>

namespace processor_ns
{

// class to store comands and associated print string and date datasets
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
  [[nodiscard]] std::string getTimestamp() const
  {
    return std::format("{}", m_Timestamp);
  }

private:
  std::string m_Cmd;
  time_t m_Timestamp;
};

class Processor
{
  // pointer to function
  using func_t = std::function<void(const std::vector<Commands>)>;

public:
  Processor() = delete;
  ~Processor() = default;
  Processor(size_t BlockSize, std::istream& input_source)
      : m_cinstream(input_source)
      , m_BlockSize(BlockSize)
  {
  }
  void parseInput();
  void parseLine();
  //
  void addCallback(const func_t& a_callback)
  {
    m_CmdBuffer.push_back(a_callback);
  }
  //
  void executeCallbacks()
  {
    for (auto& cb : m_CmdBuffer) {
      cb(m_Commands);
    }
    clearCommands();
  }
  //
  void clearCommands() { m_Commands.clear(); }

private:
  std::istream& m_cinstream;
  static inline std::string TmpString {};
  size_t m_BlockSize {1};
  size_t m_NOpenBracets {0};
  size_t m_NCloseBracets {0};
  // callbacks functions
  std::vector<func_t> m_CmdBuffer {};
  // commands
  std::vector<Commands> m_Commands;
  static inline std::ostream* m_coutstream {nullptr};

public:
  static void s_SetOutputStream(std::ostream* outStream)
  {
    Processor::m_coutstream = outStream;
  }
  static std::ostream* s_GetOutputStream() { return Processor::m_coutstream; }
};

}  // namespace processor_ns