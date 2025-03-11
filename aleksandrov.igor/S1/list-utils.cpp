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

  void getPairsList(std::istream& input, PairsList& list)
  {
    std::string listName;
    while (input >> listName)
    {
      List< unsigned long long > numList;
      unsigned long long num = 0;
      while (input >> num)
      {
        numList.pushBack(num);
      }
      input.clear();
      list.pushBack(std::make_pair(listName, numList));
    }
  }

  size_t calcMaxSubListSize(PairsList& list)
  {
    size_t maxSize = 0;
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      maxSize = std::max(maxSize, it->second.size());
    }
    return maxSize;
  }

  void getTransposedList(PairsList& list, List< List< unsigned long long > >& toTranspose)
  {
    size_t shift = 0;
    size_t maxSubListSize = calcMaxSubListSize(list);
    while (shift != maxSubListSize)
    {
      List< unsigned long long > numList;
      for (auto it = list.begin(); it != list.end(); ++it)
      {
        auto shiftedIt = it->second.begin();
        for (size_t i = 0; i < shift && shiftedIt != it->second.end(); ++i)
        {
          ++shiftedIt;
        }
        if (shiftedIt != it->second.end())
        {
          numList.pushBack(*shiftedIt);
        }
      }
      ++shift;
      toTranspose.pushBack(numList);
    }
  }
}

