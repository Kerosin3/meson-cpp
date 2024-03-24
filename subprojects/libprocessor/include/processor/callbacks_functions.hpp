#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <ios>
#include <iostream>
#include <ostream>
#include <string_view>
#include <vector>

#include <bits/types/time_t.h>

#include "processor/processor.hpp"

using namespace std::string_literals;
using namespace processor_ns;

static inline void
write_to_file(const std::string& str, const std::string& filename)
{
  std::ofstream OutFile {filename, std::ios_base::out | std::ios_base::app};
  OutFile << str << std::endl;
}

static inline void
print_to_stdout(const std::vector<Commands>& cmds)
{
  using namespace processor_ns;
  std::ostream* OUT_STREAM = Processor::s_GetOutputStream();
  *OUT_STREAM << "bulk: ";
  for (unsigned index = 0; const auto& obj : cmds) {
    if (!index) {
      *OUT_STREAM << std::format("{}", obj.getCmd());
    } else {
      *OUT_STREAM << std::format(", {}", obj.getCmd());
    }
    index++;
  }
  *OUT_STREAM << std::endl;
}

static inline void
write_to_log(const std::vector<Commands>& cmds)
{
  auto Timestamp =
      std::format("{}{}", "bulk", cmds.front().getTimestamp().append(".log"));
  for (const auto& obj : cmds) {
    write_to_file(
        std::format(
            "comand:{}, timestamp:{}", obj.getCmd(), obj.getTimestamp()),
        Timestamp);
  }
}
