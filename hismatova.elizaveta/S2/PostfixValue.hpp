#ifndef POSTFIXVALUE_HPP
#define POSTFIXVALUE_HPP
#include <stdexcept>
namespace hismatova
{
  class PostfixValue
  {
  private:
    long long value;
    char op;
    bool isNumber;
  public:
    PostfixValue();
    explicit PostfixValue(long long val);
    explicit PostfixValue(char oper);
    bool isNumeric() const;
    long long getValue() const;
    char getOperator() const;
    PostfixValue operator+(const PostfixValue& rhs) const;
    PostfixValue operator-(const PostfixValue& rhs) const;
    PostfixValue operator*(const PostfixValue& rhs) const;
    PostfixValue operator/(const PostfixValue& rhs) const;
    PostfixValue operator%(const PostfixValue& rhs) const;
  };
}
#endif
