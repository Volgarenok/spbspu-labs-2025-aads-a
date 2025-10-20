#include "listUtils.hpp"
#include <iostream>
#include <limits>

namespace
{
  void checkOverflow(size_t sum, size_t itNum, bool & flagOverflow)
  {
    const size_t max = std::numeric_limits< size_t >::max();
    if (sum > max - itNum)
    {
      flagOverflow = true;
    }
  }
}

void maslov::printData(std::ostream & out, const list & listOfPairs, size_t maxSize)
{
  out << listOfPairs.begin()->first;
  for (auto it = ++listOfPairs.begin(); it != listOfPairs.end(); ++it)
  {
    out << " " << it->first;
  }
  out << "\n";
  if (maxSize == 0)
  {
    out << "0";
    return;
  }
  maslov::FwdList< size_t > sums;
  bool flagOverflow = false;
  for (size_t i = 0; i < maxSize; ++i)
  {
    size_t sum = 0;
    auto it = listOfPairs.begin();
    while (it != listOfPairs.end() && it->second.size() <= i)
    {
      ++it;
    }
    if (it != listOfPairs.end())
    {
      auto itNum = it->second.begin();
      std::advance(itNum, i);
      checkOverflow(sum, *itNum, flagOverflow);
      sum += *itNum;
      out << *itNum;
      ++it;
    }
    for (; it != listOfPairs.end(); ++it)
    {
      if (!it->second.empty() && it->second.size() > i)
      {
        auto itNum = it->second.begin();
        std::advance(itNum, i);
        checkOverflow(sum, *itNum, flagOverflow);
        sum += *itNum;
        out << " " << *itNum;
      }
    }
    sums.pushFront(sum);
    out << "\n";
  }
  sums.reverse();
  if (flagOverflow)
  {
    throw std::overflow_error("ERROR: overflow");
  }
  auto it = sums.begin();
  out << *it;
  for (++it; it != sums.end(); ++it)
  {
    out << " " << *it;
  }
}
