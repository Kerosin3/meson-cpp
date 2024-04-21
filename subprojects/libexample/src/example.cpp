
#include "example.hpp"
#include "example.tpp"

namespace Myspace
{
  Container operator+(const Container& t_cont, int rhs)
  {
    return Container {t_cont.m_result.value_or(0) + rhs};
  }

  std::ostream& operator<<(std::ostream& out, const Myspace::Container& t_par)
  {
    if (t_par.m_result.has_value()) {
      out << "your value is " << t_par.m_result.value() << std::endl;
    } else {
      out << "no value" << std::endl;
    }
    return out;
  }

}  // namespace Myspace



// template class MyArray::Digit<double>;
