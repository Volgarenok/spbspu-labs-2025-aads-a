#include <algorithm>
#include <exception>
#include <limits>
#include <stdexcept>

#include "expression.hpp"
#include "queue.hpp"
#include "stack.hpp"

inline bool isNumber(const std::string& token)
{
  if (token.empty() || (token.size() == 1 && !std::isdigit(token[0])))
  {
    return false;
  }
  size_t start = token[0] == '+' || token[0] == '-' ? 1 : 0;
  bool flag = true;
  for (size_t i = start; i < token.size() && flag; ++i)
  {
    flag = std::isdigit(token[i]);
  }
  return flag;
}

inline bool isOperation(const std::string& token)
{
  return token == "+" || token == "-" || token == "*" || token == "/" || token == "%";
}

inline int prior(const std::string& op)
{
  if (op == "+" || op == "-")
  {
    return 1;
  }
  if (op == "*" || op == "/" || op == "%")
  {
    return 2;
  }
  return 0;
}
