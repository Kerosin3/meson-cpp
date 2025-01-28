#include <iostream>

#include "async.hpp"

int
main(int argc, char* argv[])
{
  // async::func1();
  // async::func2();
  auto handler = async::connect(3);
  // auto handler2 = async::connect(3);
  std::string data =
      "cmd1\ncmd2\n{\ncmd3\ncmd4\n}\n{\ncmd5\ncmd6\n{\ncmd7\ncmd8\n}\ncmd9\n}"
      "\n{\ncmd10\ncmd11\n";
  async::receive(handler, data.c_str(), data.size());
  // async::receiveCin(handler);

  return EXIT_SUCCESS;
}
