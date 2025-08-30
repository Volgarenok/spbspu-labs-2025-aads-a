#include "keySum.hpp"
#include <limits>
#include <stdexcept>

void brevnov::KeySum::operator()(const std::pair< const int, std::string >& pair)
{
  const int min = std::numeric_limits< int >::min();
  const int max = std::numeric_limits< int >::max();
  if ((sum > max - pair.first) && (sum > 0 && pair.first > 0))
  {
    throw std::logic_error("Overflow");
  }
  if ((sum < min + pair.first) && (sum < 0 && pair.first < 0))
  {
    throw std::logic_error("Underflow");
  }
  sum += pair.first;
  str += " " + pair.second;
}
