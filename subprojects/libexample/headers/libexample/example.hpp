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

  protected:
    std::array<U, size> m_array {};
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
}  // namespace MyArray