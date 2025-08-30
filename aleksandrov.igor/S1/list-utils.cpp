#include "list-utils.hpp"
#include "list.hpp"
#include <stdexcept>
#include <iostream>
#include <limits>

namespace aleksandrov
{
  unsigned long long calcSum(const List< unsigned long long >& list)
  {
    unsigned long long sum = 0;
    for (auto it = list.cbegin(); it != list.cend(); ++it)
    {
      unsigned long long value = *it;
      sum = calcSum(sum, value);
    }
    return sum;
  }

  unsigned long long calcSum(unsigned long long a, unsigned long long b)
  {
    const unsigned long long maxValue = std::numeric_limits< unsigned long long >::max();
    if (maxValue - a > b)
    {
      return a + b;
    }
    else
    {
      throw std::overflow_error("There was an overflow error!");
    }
  }

  void outputList(const List< unsigned long long >& list, std::ostream& out)
  {
    out << *list.cbegin();
    for (auto it = ++list.cbegin(); it != list.cend(); ++it)
    {
      out << ' ' << *it;
    }
  }

  void getPairsList(std::istream& in, PairsList& list)
  {
    std::string listName;
    while (in >> listName)
    {
      List< unsigned long long > numList;
      unsigned long long num = 0;
      while (in >> num)
      {
        numList.emplaceBack(num);
      }
      in.clear();
      list.emplaceBack(std::make_pair(listName, numList));
    }
  }

  size_t calcMaxSubListSize(const PairsList& list)
  {
    if (list.empty())
    {
      return 0;
    }
    size_t maxSize = 0;
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      maxSize = std::max(maxSize, it->second.size());
    }
    return maxSize;
  }

  void getTransposedList(const PairsList& list, List< List< unsigned long long > >& toTranspose)
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
          numList.emplaceBack(*shiftedIt);
        }
      }
      ++shift;
      toTranspose.emplaceBack(numList);
    }
  }
}

