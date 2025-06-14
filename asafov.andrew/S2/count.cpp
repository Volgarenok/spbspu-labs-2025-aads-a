#include "countFunctions.h"
#include <stdexcept>
#include <iomanip>

void asafov::count(asafov::stack_num_t& result, std::istream& is)
{
  while (!is.eof())
  {
    asafov::evaluateExpressionFromString(is, result);
  }
  if (result.empty())
  {
    std::cout << '\n';
    return;
  }
  while (result.size() > 1)
  {
    std::cout << std::fixed << std::setprecision(0) << result.top() << ' ';
    result.pop();
  }
  std::cout << std::fixed << std::setprecision(0) << result.top() << '\n';
  result.pop();
}
