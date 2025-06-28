#include "key_summ.hpp"
#include <limits>
#include <stdexcept>

namespace
{
  bool isOverflow(int a, int b)
  {
    int max = std::numeric_limits< int >::max();
    return a > 0 && b > 0 && a > max - b;
  }

  bool isUnderflow(int a, int b)
  {
    int min = std::numeric_limits< int >::min();
    return a < 0 && b < 0 && a < min - b;
  }
}

tkach::KeySumm::KeySumm():
  key_sum(0),
  values()
{}

void tkach::KeySumm::operator()(const std::pair< int, std::string >& key_value)
{
  if (isOverflow(key_sum, key_value.first))
  {
    throw std::overflow_error("overflow");
  }
  else if (isUnderflow(key_sum, key_value.first))
  {
    throw std::overflow_error("underflow");
  }
  key_sum += key_value.first;
  if (values.empty())
  {
    values += key_value.second;
  }
  else
  {
    values += ' ' + key_value.second;
  }
}
