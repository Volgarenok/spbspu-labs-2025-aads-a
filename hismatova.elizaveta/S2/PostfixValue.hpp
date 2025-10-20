#ifndef POSTFIXVALUE_HPP
#define POSTFIXVALUE_HPP
#include <stdexcept>
namespace hismatova
{
  class PostfixValue
  {
  private:
    int value;
    char op;
    bool isNumber;
  public:
    PostfixValue();
    explicit PostfixValue(int val);
    explicit PostfixValue(char oper);
    bool isNumeric() const;
    int getValue() const;
    char getOperator() const;
    PostfixValue operator+(const PostfixValue& rhs) const;
    PostfixValue operator-(const PostfixValue& rhs) const;
    PostfixValue operator*(const PostfixValue& rhs) const;
    PostfixValue operator/(const PostfixValue& rhs) const;
    PostfixValue operator%(const PostfixValue& rhs) const;
  };
}
#endif
