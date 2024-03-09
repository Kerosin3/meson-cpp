#include <chrono>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <sstream>
#include <thread>

#include "processor/callbacks_functions.hpp"
#include "processor/processor.hpp"

int
main(int argc, char* argv[])
{
  using namespace std::chrono_literals;
  constexpr bool IF_DEBUG = false;

  if (!IF_DEBUG) {
    if (argc < 2) {
      fprintf(stderr, "usage: %s <size>\n", argv[0]);
      return 1;
    }

    int blockSize = atoi(argv[1]);
    if (blockSize <= 0) {
      fprintf(stderr, "block size mush be a positive and greater than zero\n");
      return EXIT_FAILURE;
    }
    Processor MyProc {static_cast<size_t>(blockSize), std::cin};
    std::ostream& StreamCout = std::cout;
    Processor::s_SetOutputStream(&StreamCout);
    MyProc.addCallback(&print_to_stdout);
    MyProc.addCallback(&write_to_log);
    MyProc.parseInput();
    return EXIT_SUCCESS;
  } else {

    constexpr size_t BLOCKSIZE = 3;
    std::string TestInput =
        "cmd1\ncmd2\n"
        "{\ncmd3\ncmd4\n}\n"
        "{\ncmd5\ncmd6\n{\ncmd7\ncmd8\n}\ncmd9\n}"
        "\n{\ncmd10\ncmd11"
        "\n";
    std::cout << "input data:\n"
              << TestInput << "-------------\n"
              << "output data:\n";

    std::stringstream InputStream {TestInput};
    Processor MyProc  {BLOCKSIZE, InputStream};
    std::ostream& StreamCout = std::cout;
    Processor::s_SetOutputStream(&StreamCout);
    MyProc.addCallback(&print_to_stdout);
    MyProc.addCallback(&write_to_log);
    MyProc.parseInput();

    std::this_thread::sleep_for(3000ms);
    return EXIT_SUCCESS;
  }
}
