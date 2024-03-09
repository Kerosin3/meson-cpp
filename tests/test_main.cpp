#include <chrono>
#include <memory>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include "processor/callbacks_functions.hpp"
#include "processor/processor.hpp"

class ProcessorFixtured : public ::testing::Test
{
public:
  ProcessorFixtured() {}
  processor_ns::Processor getProcessor()
  {
    return processor_ns::Processor {3, m_InputEmulatedStream};
  }
  void SetUp(std::string& input, std::string& output)
  {
    auto write = [](std::stringstream& os, std::string& content)
    { os << content; };
    m_input = input;
    m_Expected = output;
    write(m_InputEmulatedStream, m_input);
    processor_ns::Processor::s_SetOutputStream(&m_OutputCapturedOutput);
  }
  void TearDown() override
  {
    ASSERT_EQ(m_OutputCapturedOutput.str(), m_Expected);
  }
  std::string m_input {};
  std::stringstream m_InputEmulatedStream;
  std::ostringstream m_OutputCapturedOutput;
  std::string m_Expected;
};

TEST_F(ProcessorFixtured, test_simple)
{
  std::string TestInput = "cmd1\n{\n";
  std::string Expected = "bulk: cmd1\n";
  SetUp(TestInput, Expected);
  auto Proc = ProcessorFixtured::getProcessor();
  Proc.addCallback(&print_to_stdout);
  Proc.parseInput();
}

TEST_F(ProcessorFixtured, test2)
{
  std::string TestInput = "cmd1\ncmd2\ncmd3\n";
  std::string Expected = "bulk: cmd1, cmd2, cmd3\n";
  SetUp(TestInput, Expected);
  auto Proc = ProcessorFixtured::getProcessor();
  Proc.addCallback(&print_to_stdout);
  Proc.parseInput();
}

TEST_F(ProcessorFixtured, test3)
{
  std::string TestInput = "cmd1\ncmd2\n{";
  std::string Expected = "bulk: cmd1, cmd2\n";
  SetUp(TestInput, Expected);
  auto Proc = ProcessorFixtured::getProcessor();
  Proc.addCallback(&print_to_stdout);
  Proc.parseInput();
}

TEST_F(ProcessorFixtured, testBLOCKS1)
{
  std::string TestInput = "cmd1\ncmd2\ncmd3\ncmd4\ncmd5\n";
  std::string Expected =
      "bulk: cmd1, cmd2, cmd3\n"
      "bulk: cmd4, cmd5\n";
  SetUp(TestInput, Expected);
  auto Proc = ProcessorFixtured::getProcessor();
  Proc.addCallback(&print_to_stdout);
  Proc.parseInput();
}
TEST_F(ProcessorFixtured, testBLOCKS2)
{
  std::string TestInput =
      "cmd1\ncmd2\n"
      "{\ncmd3\ncmd4\n}\n"
      "{\ncmd5\ncmd6\n{\ncmd7\ncmd8\n}\ncmd9\n}"
      "\n{\ncmd10\ncmd11"
      "\n";
  std::string Expected =
      "bulk: cmd1, cmd2\n"
      "bulk: cmd3, cmd4\n"
      "bulk: cmd5, cmd6, cmd7, cmd8, cmd9\n";
  SetUp(TestInput, Expected);
  auto Proc = ProcessorFixtured::getProcessor();
  Proc.addCallback(&print_to_stdout);
  Proc.parseInput();
}
