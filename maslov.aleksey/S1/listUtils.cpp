#include "listUtils.hpp"
#include <iostream>
#include <limits>

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
  const size_t max = std::numeric_limits< size_t >::max();
  maslov::FwdList< size_t > sums;
  bool flagOverflow = false;
  for (size_t i = 0; i < maxSize; ++i)
  {
    size_t sum = 0;
    bool flagPrint = false;
    for (auto it = listOfPairs.begin(); it != listOfPairs.end(); ++it)
    {
      if (!it->second.empty())
      {
        bool flagEnd = true;
        auto itNum = it->second.begin();
        for (size_t j = 0; j < i; ++j)
        {
          itNum++;
          if (itNum == it->second.end())
          {
            flagEnd = false;
            break;
          }
        }
        if (flagEnd)
        {
          if (flagPrint)
          {
            out << " ";
          }
          if (sum > max - *itNum)
          {
            flagOverflow = true;
          }
          sum += *itNum;
          out << *itNum;
          flagPrint = true;
        }
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
