#include "countFunctions.h"
#include <stdexcept>

int asafov::evaluateExpressionFromString(std::istream& is, std::stack<num_t>& result)
{
  std::string str;
  std::getline(is, str);
  if (str.empty()) return 0;
  asafov::queue_t queue;
  asafov::parseStringToQueue(queue, str);
  try
  {
    asafov::convertInfixToPostfix(queue);
    result.push(asafov::evaluatePostfixExpression(queue));
  }
  catch (const std::invalid_argument& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  catch (const std::logic_error& e)
  {
    std::cerr << e.what() << '\n';
    return 2;
  }
  return 0;
}
