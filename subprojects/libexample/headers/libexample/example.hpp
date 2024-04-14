#pragma once
#include <algorithm>
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
