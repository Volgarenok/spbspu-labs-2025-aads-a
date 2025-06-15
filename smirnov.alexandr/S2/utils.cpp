#include "utils.hpp"
#include <stdexcept>

bool smirnov::isOperator(const std::string & token)
{
  return token == "+" || token == "-" || token == "*" || token == "/" || token == "%";
}

int smirnov::getPriority(const std::string & op)
{
  if (op == "+" || op == "-")
  {
    return 1;
  }
  else if (op == "*" || op == "/" || op == "%")
  {
    return 2;
  }
  return 0;
}
