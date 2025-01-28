#include <fstream>

#include "processor.hpp"

namespace Processor
{
/*
void
DataProcessor()
{
std::string input {};
while (std::cin >> input) {
  std::cout << "your data is " << input << "\n";
}
}
*/

void
InputProcessorParser::readInput(std::istringstream& ss)
{
  auto thr = std::jthread(
      [&]()
      {
        std::string TmpString {};
        auto printx = [&]()
        {
          for (auto& elem : m_Commands) {
            std::cout << "d: " << elem.getCmd() << "\n";
          }
        };
        std::ofstream ofs("fout", std::ios_base::out | std::ios_base::trunc);
        auto writex = [&]()
        {
          for (auto& elem : m_Commands) {
            ofs << elem.getCmd() << " " << elem.getTimestamp() << "\n";
            ofs.flush();
          }
        };
        while (std::getline(ss, TmpString)) {
          std::cout << "input: " << TmpString << "\n";
          if (TmpString.empty()) {
            continue;
          }

          auto isOpBrace = [](char symbol) -> bool { return symbol == '{'; };
          auto isClBrace = [](char symbol) -> bool { return symbol == '}'; };

          if (isOpBrace(TmpString.at(0))) {
            if (m_NOpenBracets == 0 && !m_Commands.empty()) {
              // executeCallbacks();
              std::cout << "CALLBACK1\n";
              printx();
              writex();
              m_Commands.clear();
            }
            m_NOpenBracets++;
          } else if (isClBrace(TmpString.at(0))) {
            if (m_NOpenBracets > 0) {
              m_NCloseBracets++;
              if (m_NCloseBracets == m_NOpenBracets && !m_Commands.empty()) {
                //   executeCallbacks();
                std::cout << "CALLBACK2\n";
                printx();
                writex();
                m_Commands.clear();
              }
              m_NCloseBracets = 0;
              m_NOpenBracets = 0;
            }
          } else {
            m_Commands.emplace_back(TmpString);
            if (m_Commands.size() == m_BlockSize && m_NOpenBracets == 0) {
              // executeCallbacks();
              std::cout << "CALLBACK3\n";
              writex();
              printx();
            }
          }

          data_to_write.get().push_back(TmpString);
        }
        if (!m_Commands.empty() & !m_NOpenBracets) {
          // executeCallbacks();
          // std::cout << "CALLBACK4\n";
          // writex();
          // printx();
        }
      });
}
}  // namespace Processor