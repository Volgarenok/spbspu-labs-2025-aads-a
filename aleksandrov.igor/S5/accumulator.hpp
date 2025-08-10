#ifndef ACCUMULATOR_HPP
#define ACCUMULATOR_HPP

#include <stdexcept>
#include <utility>
#include <string>
#include <limits>

namespace aleksandrov
{
  constexpr long long int min = std::numeric_limits< long long int >::min();
  constexpr long long int max = std::numeric_limits< long long int >::max();

  struct Accumulator
  {
    void operator()(const std::pair< long long int, std::string >& pair)
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

    long long int key = 0;
    std::string value;
  };
}

#endif

