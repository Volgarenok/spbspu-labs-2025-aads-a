#ifndef POSTFIX_HPP
#define POSTFIX_HPP
#include <string>
#include <queue.hpp>
#include <stack.hpp>

namespace sveshnikov
{
  class Postfix
  {
  public:
    Postfix() = default;
    Postfix(const Postfix &other);
    Postfix(Postfix &&other);
    Postfix(Queue< std::string > infix);
    ~Postfix() = default;
    Postfix &operator=(const Postfix &other);
    Postfix &operator=(Postfix &&other) noexcept;

    Postfix operator+(const Postfix &other) const;
    Postfix operator-(const Postfix &other) const;
    Postfix operator/(const Postfix &other) const;
    Postfix operator%(const Postfix &other) const;
    Postfix operator*(const Postfix &other) const;

    long long calculate() const;

  private:
    Queue< std::string > expr_;
    Postfix arith_operator_impl(Postfix other, const std::string &op) const;
    void push_out_stack(Stack< std::string > &stack, const std::string &op);
  };
}

#endif
