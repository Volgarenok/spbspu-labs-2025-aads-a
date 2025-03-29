#ifndef POSTFIX_HPP
#define POSTFIX_HPP
#include <string>
#include "queue.hpp"

namespace alymova
{
  struct Postfix
  {
    Postfix();
    Postfix(const Postfix& other);
    Postfix(Postfix&& other);
    ~Postfix() = default;

    long long int operator+(const Postfix& other);
    long long int operator-(const Postfix& other);
    long long int operator*(const Postfix& other);
    long long int operator/(const Postfix& other);
    long long int operator%(const Postfix& other);
  private:
    Queue< std::string > postfix;
  };
}
#endif
