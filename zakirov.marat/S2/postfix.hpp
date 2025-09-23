#ifndef POSTFIX_HPP
#define POSTFIX_HPP
#include <string>
#include "queue.hpp"

namespace zakirov
{
  class Postfix
  {
  public:
    Postfix() = default;
    Postfix(const Postfix & other) = default;
    Postfix(Postfix && other) = default;
    Postfix(const std::string & infix);
    ~Postfix() = default;
    Postfix & operator=(const Postfix & other) = default;
    Postfix operator+(const Postfix & other);
    Postfix operator-(const Postfix & other);
    Postfix operator*(const Postfix & other);
    Postfix operator/(const Postfix & other);
    Postfix operator%(const Postfix & other);
    long long operator()();
  private:
    Postfix unite_postfixes(const Postfix & other);
    bool check_operand(const std::string & line);
    bool check_operator(std::string symbol);
    bool check_priority(std::string symbol);
    void check_overflow(long long first, long long second, char oper);
    Queue< std::string > transform_to_postfix(Queue< std::string > & infix);
    long long transform_to_llint(const std::string & line);
    long long calculate_postfix(long long first, long long second, char oper);
    Queue< std::string > postfix;
  };
}

#endif
