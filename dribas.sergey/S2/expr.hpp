#ifndef EXPR_HPP
#define EXPR_HPP

#include <istream>
#include <string>
#include <queue.hpp>

namespace dribas
{
  Queue< std::string > inputInfix(const std::string&);
  long long evaluatePostfix(Queue< std::string >&);
  Queue< std::string > infixToPostfix(Queue< std::string >&);

}

#endif
