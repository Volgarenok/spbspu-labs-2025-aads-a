#ifndef EXPR_URILS_HPP
#define EXPR_URILS_HPP

#include <string>
#include <ios>
#include "queue.hpp"

namespace aleksandrov
{
  namespace detail
  {
    bool isOperation(const std::string&);
    bool isNumber(const std::string&);
    bool isLessImportant(const std::string&, const std::string&);
    int sign(long long int);
    bool sameSign(long long int, long long int);
  }
  long long int sum(long long int, long long int);
  long long int mult(long long int, long long int);
  long long int div(long long int, long long int);
  long long int mod(long long int, long long int);
  Queue< std::string> infixToPostfix(Queue< std::string >&);
  long long int evalPostfix(Queue< std::string >&);
}

#endif

