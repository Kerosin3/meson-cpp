#include <algorithm>
#include <chrono>
#include <concepts>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <istream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <bits/iterator_concepts.h>
#include <boost/program_options.hpp>
#include <sys/types.h>

namespace po = boost::program_options;

using std::cerr;
using std::cout;
using std::vector;

void
print(std::integral auto val)
{
  cout << "your integral is " << val << "\n";
}

template< typename U >
  requires std::is_integral< U >::value
void
printX(U val)
{
  cout << "your integral is " << val << "\n";
}

template< typename U >
concept Exmple = requires(const U x) {
  {
    x.size()
  } -> std::convertible_to< size_t >;
};

void
funx1(Exmple auto v)
{
  cout << "OK!"
       << "\n";
}

struct Test1
{
  size_t size() const { return 15; }
};

struct Test2
{
};

template< typename T >
  requires(std::is_arithmetic_v< T >)
void
func2(T var1)
{}

int
main(int argc, char** argv)
{
  constexpr auto LOG_MAX_SIZE_BYTES = 270 * 10;
  std::string postCodeDisplayPath {"hehe.txt"};
  for (size_t i = 0; i < 10; i++) {
    // get time
    const auto now = std::chrono::system_clock::now();
    const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
    auto gmt_time = gmtime(&t_c);
    auto timestamp = std::put_time(gmt_time, "%Y-%m-%d %H:%M:%S");

    std::ofstream postCodeDisplayFile(
        postCodeDisplayPath,
        std::ios_base::app | std::ios_base::binary | std::ios_base::ate);
    // finc to write
    auto code = 0x1565;
    auto write = [&postCodeDisplayFile, code, &timestamp]()
    {
      postCodeDisplayFile << std::nounitbuf << timestamp << " 0x"
                          << std::setfill('0') << std::setw(2) << std::hex
                          << code << "\n";
    };
    std::cout << "size is " << postCodeDisplayFile.tellp() << "\n";
    if (postCodeDisplayFile.tellp() > LOG_MAX_SIZE_BYTES) {
      write();
      postCodeDisplayFile.flush();
      postCodeDisplayFile.close();
      postCodeDisplayFile.clear();
      std::ofstream postCodeDisplayFileTrunc(postCodeDisplayPath,
                                             std::ios_base::trunc);
      postCodeDisplayFileTrunc.close();
      postCodeDisplayFileTrunc.clear();
      continue;
    }
    write();
    postCodeDisplayFile.close();
  }

  // funx1(iTest2);
  // printX(5);

  /*
  auto changed = [prev = 0] (auto val) mutable{
    bool changed = prev != val;
    prev = val;
  };
  */
  /*
  auto changed = [prev = 0](auto elem) mutable
  {
    bool changed = elem != prev;
    prev = elem;
    return changed;
  };
  auto vec1 = std::vector {1, 2, 3, 3, 4, 5};
  std::copy_if(vec1.cbegin(),
               vec1.cend(),
               std::ostream_iterator< int > {std::cout, ", "},
               changed);
  cout << std::endl;
  */
  return EXIT_SUCCESS;
}
