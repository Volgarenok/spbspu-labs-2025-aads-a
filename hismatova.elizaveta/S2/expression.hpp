#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP
#include <string>
#include "queue.hpp"
#include "PostfixValue.hpp"
namespace hismatova
{
  Queue< PostfixValue > infixToPostfix(const std::string& expr);
  long long evaluatePostfix(Queue< PostfixValue >& postfix);
}
#endif
