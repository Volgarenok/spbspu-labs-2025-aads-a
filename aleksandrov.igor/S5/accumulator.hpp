#ifndef ACCUMULATOR_HPP
#define ACCUMULATOR_HPP

#include <stdexcept>
#include <utility>
#include <string>
#include <limits>

namespace aleksandrov
{
  constexpr int min = std::numeric_limits< int >::min();
  constexpr int max = std::numeric_limits< int >::max();

  struct Accumulator
  {
    void operator()(const std::pair< int, std::string >& pair)
    {
      const bool isUnderflow = pair.first < 0 && key < min - pair.first;
      const bool isOverflow = pair.first > 0 && key > max - pair.first;
      if (isUnderflow || isOverflow)
      {
        throw std::overflow_error("Overflow error!");
      }
      key += pair.first;
      value += (value.empty() ? pair.second : ' ' + pair.second);
    }

    int key = 0;
    std::string value;
  };
}

#endif

