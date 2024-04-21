#pragma once
#include <algorithm>
#include <array>
#include <cstddef>
#include <format>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

namespace Myspace
{
  class Container
  {
  public:
    Container() = default;
    ~Container() = default;
    explicit Container(int t_par1)
        : m_result(t_par1) {};

    void computeC(int t_arg)
    {
      if (!t_arg) {
        m_result = std::nullopt;
      } else {
        m_result = t_arg;
      }
    }

    friend std::ostream& operator<<(std::ostream& out, const Container& t_par);
    friend Container operator+(const Container&, int rhs);

  private:
    std::optional<int> m_result;
  };

}  // namespace Myspace

namespace MyArray
{

  template<typename U, int size>
  class Xtest
  {
  public:
    Xtest<U, size>()
    {
      for (int count {0}; count < size; ++count) {
        m_array[count] = count;
      }
    }

    void printout() const noexcept
    {
      for (int count {0}; count < size; ++count) {
        std::cout << "b value is " << m_array[count] << std::endl;
      }
    }

    Xtest& operator++()
    {
      m_counter++;
      std::cout << "increment!" << std::endl;
      return *this;
    }

  protected:
    std::array<U, size> m_array {};
    std::vector<std::pair<std::string, int>> m_paires {};
    size_t m_counter {0};
  };

  // inherite general case
  template<typename U, int size>
  class Ytest : virtual public Xtest<U, size>
  {
  };

  // specialize for double
  template<int size>
  class Ytest<double, size> : virtual public Xtest<double, size>

  {
  public:
    void printout() const noexcept
    {
      for (int count {0}; count < size; ++count) {
        std::cout << "OV value is " << this->m_array[count] << std::endl;
      }
    };
  };

  template<typename U, size_t SIZE>
  class Digit
  {
  public:
    explicit Digit(U arg)
        : m_count(arg)
    {
      for (int count {0}; count < SIZE; ++count) {
        m_array[count] = m_count + count;
      }
    }

    void printData();
    Digit& operator++(int);
    void operator()(void);

  protected:
    U m_count {};
    std::array<U, SIZE> m_array;
  };

  template<typename T, size_t SIZE>
  class DigitX : virtual public Digit<T, SIZE>
  {
  public:
    explicit DigitX(T arg)
        : Digit<T, SIZE>(arg)
    {
    }
  };

  template<int SIZE>
  class DigitX<double, SIZE> : virtual public Digit<double, SIZE>
  {
    // void printout() const noexcept;
  public:
    explicit DigitX(double arg)
        : Digit<double, SIZE>(arg)
    {
    }

    void printData();
  };

}  // namespace MyArray