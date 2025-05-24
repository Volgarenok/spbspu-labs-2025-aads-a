#include "tree-sum-functor.hpp"
#include <exception>
#include <limits>

alymova::TreeSum::TreeSum():
  keys(0),
  values()
{}

void alymova::TreeSum::operator()(const std::pair< int, std::string >& key_value)
{
  if (isOverflowAddition(keys, key_value.first))
  {
    throw std::logic_error("<SUM OVERFLOW>");
  }
  keys += key_value.first;
  values.push(key_value.second);
}
std::ostream& alymova::operator<<(std::ostream& out, const TreeSum& object)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  out << object.keys;
  Queue< std::string > copy_values(object.values);
  while (!copy_values.empty())
  {
    out << ' ' << copy_values.front();
    copy_values.pop();
  }
  return out;
}
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
