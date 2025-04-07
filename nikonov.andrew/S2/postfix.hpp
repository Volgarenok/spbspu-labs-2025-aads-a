#ifndef POSTFIX_HPP
#define POSTFIX_HPP
#include "stack.hpp"
namespace nikonov
{
  class Postfix
  {
    std::string expression;
  public:
    Postfix() = default;
    Postfix(const std::string & infix, bool mode);

    Postfix operator+(const Postfix & rhs);
    Postfix & operator+=(const Postfix & rhs);
    Postfix operator-(const Postfix & rhs);
    Postfix & operator-=(const Postfix & rhs);
    Postfix operator*(const Postfix & rhs);
    Postfix & operator*=(const Postfix & rhs);
    Postfix operator/(const Postfix & rhs);
    Postfix & operator/=(const Postfix & rhs);
    Postfix operator%(const Postfix & rhs);
    Postfix & operator%=(const Postfix & rhs);
  
    friend std::ostream & operator<<(std::ostream & out, const Postfix & toOut)
    {
      out << toOut.expression;
      return out;
    }
  };
}
#endif
