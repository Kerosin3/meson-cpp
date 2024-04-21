#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <thread>

#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <boost/program_options.hpp>

#include "libexample/example.hpp"
#include "libexample/example.tpp"
#include "test1.hpp"
namespace po = boost::program_options;

int
main(int argc, char* argv[])
{
  /*
  MyArray::Xtest<double, 10> MyArray0 {};
  MyArray0.printout();
  ++MyArray0;
  MyArray::Ytest<int, 10> MyArray1 {};
  MyArray1.printout();
  std::cout << "for Y specialized" << std::endl;
  MyArray::Ytest<double, 10> MyArray2 {};
  MyArray2.printout();
  */
  MyArray::Digit<double, 15> Digit0 {15};
  Digit0++;
  Digit0();
  Digit0.printData();
  MyArray::DigitX<double, 5> Digit1 {5};
  Digit1.printData();
  return EXIT_SUCCESS;
}
