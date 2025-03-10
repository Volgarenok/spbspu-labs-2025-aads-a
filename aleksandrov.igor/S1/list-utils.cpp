#include "list-utils.hpp"
#include "list.hpp"
#include <limits>
#include <stdexcept>
#include <iostream>

namespace aleksandrov
{
  unsigned long long calcSum(List< unsigned long long >& list)
  {
    const unsigned long long maxValue = std::numeric_limits< unsigned long long >::max();
    unsigned long long sum = 0;

    for (auto it = list.begin(); it != list.end(); ++it)
    {
      unsigned long long value = *it;
      if (maxValue - sum > value)
      {
        sum += value;
      }
      else
      {
        throw std::overflow_error("There was an overflow error!");
      }
    }
    return sum;
  }

  void outputList(List< unsigned long long >& list, std::ostream& output)
  {
    output << list.front();
    for (auto it = ++list.begin(); it != list.end(); ++it)
    {
      output << " " << *it;
    }
  }
}

