#include "list-utils.hpp"
#include "list.hpp"
#include <limits>
#include <stdexcept>
#include <iostream>

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

  void outputList(const List< unsigned long long >& list, std::ostream& output)
  {
    output << *list.cbegin();
    for (auto it = ++list.cbegin(); it != list.cend(); ++it)
    {
      output << " " << *it;
    }
  }

  void getPairsList(std::istream& input, PairsList& list)
  {
    try
    {
      std::string listName;
      while (input >> listName)
      {
        List< unsigned long long > numList;
        try
        {
          unsigned long long num = 0;
          while (input >> num)
          {
            numList.pushBack(num);
          }
          input.clear();
          list.pushBack(std::make_pair(listName, numList));
        }
        catch (const std::bad_alloc&)
        {
          numList.clear();
          throw;
        }
      }
    }
    catch (const std::bad_alloc&)
    {
      list.clear();
      throw;
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
    try
    {
      size_t shift = 0;
      size_t maxSubListSize = calcMaxSubListSize(list);
      while (shift != maxSubListSize)
      {
        List< unsigned long long > numList;
        try
        {
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
        }
        catch (const std::bad_alloc&)
        {
          numList.clear();
          throw;
        }
        ++shift;
        toTranspose.pushBack(numList);
      }
    }
    catch (const std::bad_alloc&)
    {
      toTranspose.clear();
      throw;
    }
  }
}

