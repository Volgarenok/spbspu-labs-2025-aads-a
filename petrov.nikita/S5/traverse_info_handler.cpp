#include "traverse_info_handler.hpp"
#include <limits>

void petrov::SumUpKeysDisplayValues::operator()(const std::pair< const long long, std::string > & key_val)
{
  if ((result > 0 || key_val.first > 0) && result <= std::numeric_limits< long long >::max() - key_val.first)
  {
    result += key_val.first;
  }
  else if (result < 0 && key_val.first < 0 && result >= std::numeric_limits< long long >::min() - key_val.first)
  {
    result += key_val.first;
  }
  else
  {
    throw std::out_of_range("ERROR: Out of range");
  }
  queue_.push(key_val.second);
}

std::ostream & petrov::SumUpKeysDisplayValues::displayValuesInTraverseOrder(std::ostream & out)
{
  out << queue_.front();
  queue_.pop();
  while (!queue_.empty())
  {
    out << ' ' << queue_.front();
    queue_.pop();
  }
  return out;
}
