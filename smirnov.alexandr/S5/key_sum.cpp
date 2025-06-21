#include "key_sum.hpp"
#include <limits>
#include <stdexcept>

void smirnov::KeySum::operator()(const std::pair< const int, std::string > & value)
{
  bool overflow = (value.first > 0 && result > std::numeric_limits< int >::max() - value.first);
  bool underflow = (value.first < 0 && result < std::numeric_limits< int >::min() - value.first);
  if (overflow || underflow)
  {
    throw std::overflow_error("overflow");
  }
  result += value.first;
  if (!elems.empty())
  {
    elems += " ";
  }
  elems += value.second;
}
