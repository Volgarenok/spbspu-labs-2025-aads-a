#ifndef KEYSUM_HPP
#define KEYSUM_HPP
#include <string>
#include <limits>
#include <stdexcept>

namespace kiselev
{
  struct KeySum
  {
    void operator()(const std::pair< const int, std::string >& pair)
    {
      const int max = std::numeric_limits< int >::max();
      const int min = std::numeric_limits< int >::min();
      if ((sum > 0 && pair.first > 0) && (sum > max - pair.first))
      {
        throw std::logic_error("Overflow");
      }
      if ((sum < 0 && pair.first < 0) && (sum < min + pair.first))
      {
        throw std::logic_error("Underflow");
      }
      sum += pair.first;
      str += " " + pair.second;
    }
    int sum = 0;
    std::string str;
  };
}
#endif
