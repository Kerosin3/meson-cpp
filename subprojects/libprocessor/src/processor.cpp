#include <stdexcept>

#include "processor/processor.hpp"

void
processor_ns::Processor::parseInput()
{
  if (!processor_ns::Processor::m_coutstream) {
    throw std::runtime_error(std::string("no output steam was assigned"));
  }
  while (std::getline(m_cinstream, TmpString)) {
    // deal with empty string
    if (TmpString.empty()) {
      continue;
    }
    this->parseLine();
  }
  // eof
  if (!m_Commands.empty() & !m_NOpenBracets) {
    executeCallbacks();
  }
}

void
processor_ns::Processor::parseLine()
{
  auto isOpBrace = [](char symbol) -> bool { return symbol == '{'; };
  auto isClBrace = [](char symbol) -> bool { return symbol == '}'; };
  if (isOpBrace(TmpString.at(0))) {
    if (m_NOpenBracets == 0 && !m_Commands.empty()) {
      executeCallbacks();
    }
    m_NOpenBracets++;
  } else if (isClBrace(TmpString.at(0))) {
    if (m_NOpenBracets > 0) {
      m_NCloseBracets++;
      if (m_NCloseBracets == m_NOpenBracets && !m_Commands.empty()) {
        executeCallbacks();
      }
      m_NCloseBracets = 0;
      m_NOpenBracets = 0;
    }
  } else {
    m_Commands.emplace_back(TmpString);
    if (m_Commands.size() == m_BlockSize && m_NOpenBracets == 0) {
      executeCallbacks();
    }
  }
}
