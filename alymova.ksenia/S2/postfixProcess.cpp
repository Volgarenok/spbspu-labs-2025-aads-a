#include "postfixProcess.hpp"
#include <cstddef>
#include <string>
#include <limits>
#include <stdexcept>

bool alymova::isOverflowAddition(long long int lhs, long long int rhs)
{
  const long long int max_sum = std::numeric_limits< long long int >::max();
  const long long int min_sum = std::numeric_limits< long long int >::min();
  if (lhs > 0 && rhs > 0)
  {
    return ((max_sum - rhs) < lhs);
  }
  if (lhs < 0 && rhs < 0)
  {
    return ((min_sum - rhs) > lhs);
  }
  return false;
}
bool alymova::isOverflowSubstraction(long long int lhs, long long int rhs)
{
  const long long int max_substr = std::numeric_limits< long long int >::max();
  const long long int min_substr = std::numeric_limits< long long int >::min();
  if (lhs > 0 && rhs < 0)
  {
    return ((max_substr + rhs) < lhs);
  }
  if (lhs < 0 && rhs > 0)
  {
    return ((min_substr + rhs) > lhs);
  }
  return false;
}
bool alymova::isOverflowMulti(long long int lhs, long long int rhs)
{
  const long long int max_multi = std::numeric_limits< long long int >::max();
  const long long int min_multi = std::numeric_limits< long long int >::min();
  if ((lhs > 0 && rhs > 0) || (lhs < 0 && rhs < 0))
  {
    return (std::abs(max_multi / lhs) < std::abs(rhs));
  }
  else
  {
    return (std::abs(min_multi / lhs) < std::abs(rhs));
  }
  return false;
}
long long int alymova::mod(long long int item1, long long int item2)
{
  long long int quot = item1 / item2;
  if (item1 < 0 && item2 < 0)
  {
    quot++;
  }
  if (item1 < 0 && item2 > 0)
  {
    quot--;
  }
  return item1 - quot * item2;
}
bool alymova::detail::haveNotLessPriority(const std::string& token, const std::string& top)
{
  if (token == "*" || token == "/" || token == "%")
  {
    return (top == "*" || top == "/" || top == "%");
  }
  return isOperation(top);
}
bool alymova::detail::isOperation(const std::string& token)
{
  const std::string operations[5] = {"+", "-", "*", "/", "%"};
  for (std::string s: operations)
  {
    if (token == s)
    {
      return true;
    }
  }
  return false;
}
