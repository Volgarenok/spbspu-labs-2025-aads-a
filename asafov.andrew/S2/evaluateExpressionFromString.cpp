#include "countFunctions.h"
#include <stdexcept>

int asafov::evaluateExpressionFromString(std::istream& is, asafov::stack_num_t& result)
{
  std::string str;
  std::getline(is, str);
  if (str.empty()) return 0;
  asafov::queue_str_t queue;
  asafov::parseStringToQueue(queue, str);
  asafov::convertInfixToPostfix(queue);
  result.push(asafov::evaluatePostfixExpression(queue));
  return 0;
}
