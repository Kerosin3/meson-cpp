#include <cstddef>
#include <format>
#include <iostream>
#include <string>

#include "example.hpp"

// #include "libexample/example.hpp"
namespace MyArray
{
  template<typename U, size_t SIZE>
  void Digit<U, SIZE>::printData()
  {
    std::string strTmp {};
    for (int count {0}; count < SIZE; ++count) {
      strTmp = std::format("your value is {}", this->m_array[count]);
      std::cout << strTmp << std::endl;
    }
  }

  template<typename U, size_t SIZE>
  Digit<U, SIZE>& Digit<U, SIZE>::operator++(int)
  {
    this->m_count += 1;
    return *this;
  }

  template<typename U, size_t SIZE>
  void Digit<U, SIZE>::operator()(void)
  {
    std::cout << "your call!" << std::endl;
  }

  template<int SIZE>
  void DigitX<double, SIZE>::printData()
  {
    std::string strTmp {};
    for (int count {0}; count < SIZE; ++count) {
      strTmp = std::format("Xvalue is {}", this->m_array[count]);
      std::cout << strTmp << std::endl;
    }
  }

}  // namespace MyArray