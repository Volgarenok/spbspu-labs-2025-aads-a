#ifndef POSTFIX_HPP
#define POSTFIX_HPP
#include "stack.hpp"
#include "queue.hpp"
namespace nikonov
{
  class Postfix
  {
  public:
    Postfix() = default;
    Postfix(const std::string & infix);

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
    long long operator()() const;
  private:
    Queue< std::string > tokens_;
    
    Postfix& doMathOperator(const Postfix& another, const std::string& operation);
  };
}
#endif
