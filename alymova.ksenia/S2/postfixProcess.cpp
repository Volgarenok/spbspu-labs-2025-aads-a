#include "postfixProcess.hpp"
#include <cstddef>
#include <string>
#include <limits>
#include <stdexcept>
#include "queue.hpp"
#include "stack.hpp"

bool alymova::my_isdigit(const std::string& s)
{
  size_t i = 0;
  if (s[0] == '-' && s.size() > 1)
  {
    i++;
  }
  for (; i < s.size(); i++)
  {
    if (!std::isdigit(s[i]))
    {
      return false;
    }
  }
  return true;
}
bool alymova::is_overflow_addition(long long int lhs, long long int rhs)
{
  long long int max_sum = std::numeric_limits< long long int >::max();
  long long int min_sum = std::numeric_limits< long long int >::min();
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
bool alymova::is_overflow_substraction(long long int lhs, long long int rhs)
{
  long long int max_sum = std::numeric_limits< long long int >::max();
  long long int min_sum = std::numeric_limits< long long int >::min();
  if (lhs > 0 && rhs < 0)
  {
    return ((max_sum + rhs) < lhs);
  }
  if (lhs < 0 && rhs > 0)
  {
    return ((min_sum + rhs) > lhs);
  }
  return false;
}
bool alymova::is_overflow_multi(long long int lhs, long long int rhs)
{
  long long int max_sum = std::numeric_limits< long long int >::max();
  long long int min_sum = std::numeric_limits< long long int >::min();
  if ((lhs > 0 && rhs > 0) || (lhs < 0 && rhs < 0))
  {
    return (std::abs(max_sum / lhs) < std::abs(rhs));
  }
  else
  {
    return (std::abs(min_sum / lhs) < std::abs(rhs));
  }
  return false;
}
long long int alymova::my_mod(long long int item1, long long int item2)
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
  long long int res = item1 - quot * item2;
  return res;
}
bool alymova::is_first_priority(const std::string& token)
{
  return (token == "*" || token == "/" || token == "%");
}
bool alymova::is_second_priority(const std::string& token)
{
  return (token == "+" || token == "-");
}
bool alymova::is_need_priority(const std::string& item, const std::string& token)
{
  if (is_second_priority(item))
  {
    return (is_first_priority(token) || is_second_priority(token));
  }
  else
  {
    return is_first_priority(token);
  }
}
