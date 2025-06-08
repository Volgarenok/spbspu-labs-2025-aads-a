#include <iostream>
#include <limits>
#include <string>
#include "fwdlist.hpp"

using pairedList = kushekbaev::FwdList< std::pair<std::string, kushekbaev::FwdList< unsigned long long > > >;

namespace
{
  size_t calcOfSize(const pairedList& pairsList)
  {
    size_t maxSize = 0;

    for (const auto& pair: pairsList)
    {
      maxSize = std::max(maxSize, pair.second.size());
    }

    return maxSize;
  }
}

int main()
{
  pairedList pairsList;
  std::string listNumber = "";

  unsigned long long value = 0;
  while (std::cin >> listNumber)
  {
    kushekbaev::FwdList< unsigned long long > inputValueList;
    while (std::cin >> value)
    {
      inputValueList.push_front(value);
    }
    inputValueList.reverse();
    pairsList.push_front(std::make_pair(listNumber, inputValueList));
    std::cin.clear();
  }
  pairsList.reverse();

  if (pairsList.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  std::cout << pairsList.begin()->first;
  for (auto it = ++pairsList.begin(); it != pairsList.end(); ++it)
  {
    std::cout << " " << it->first;
  }
  std::cout << "\n";

  size_t maxSize = calcOfSize(pairsList);
  kushekbaev::FwdList< kushekbaev::FwdList< unsigned long long > > valueList;

  for (size_t i = 0; i < maxSize; ++i)
  {
    kushekbaev::FwdList< unsigned long long > rowList;
    for (auto it = pairsList.begin(); it != pairsList.end(); ++it)
    {
      if (i < it->second.size())
      {
        auto valueIt = it->second.begin();
        std::advance(valueIt, i);
        rowList.push_front(*valueIt);
      }
    }
    rowList.reverse();
    valueList.push_front(rowList);
  }
  valueList.reverse();

  for (auto it = valueList.begin(); it != valueList.end(); ++it)
  {
    std::cout << *(it->begin());
    for (auto valueIt = ++(it->begin()); valueIt != it->end(); ++valueIt)
    {
      std::cout << " " << *valueIt;
    }
    std::cout << "\n";
  }

  kushekbaev::FwdList< unsigned long long > sumList;

  for (auto it = valueList.begin(); it != valueList.end(); ++it)
  {
    unsigned long long sum = 0;
    for (auto valueIt = it->begin(); valueIt != it->end(); ++valueIt)
    {
      if (*valueIt > std::numeric_limits< unsigned long long >::max() - sum)
      {
        std::cerr << "Overflow!\n";
        return 1;
      }
      sum += *valueIt;
    }
    sumList.push_front(sum);
  }

  if (!sumList.empty())
  {
    sumList.reverse();
    std::cout << *(sumList.begin());
    for (auto it = ++(sumList.begin()); it != sumList.end(); ++it)
    {
      std::cout << " " << *it;
    }
  }
  else
  {
    std::cout << 0;
  }
  std::cout << "\n";
  return 0;
}
