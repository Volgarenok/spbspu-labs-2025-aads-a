#include <iostream>
#include <limits>
#include <list>
#include <string>

using pairedList = std::list< std::pair<std::string, std::list< unsigned long long > > >;

namespace
{
  size_t calcOfSize(pairedList pairsList)
  {
    size_t maxSize = 0;

    for (const auto& pair : pairsList)
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

  while (std::cin >> listNumber)
  {
    std::list< unsigned long long > inputValueList;
    unsigned long long value = 0;
    while (std::cin >> value)
    {
      inputValueList.push_back(value);
    }
    pairsList.push_back(std::make_pair(listNumber, inputValueList));
    std::cin.clear();
  }

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
  std::list< std::list< unsigned long long > > valueList;

  for (size_t i = 0; i < maxSize; ++i)
  {
    std::list< unsigned long long > rowList;
    for (auto it = pairsList.begin(); it != pairsList.end(); ++it)
    {
      if (i < it->second.size())
      {
        auto valueIt = it->second.begin();
        for (size_t j = 0; j < i; j++)
        {
          ++valueIt;
        }
        rowList.push_back(*valueIt);
      }
    }
    valueList.push_back(rowList);
  }

  for (auto it = valueList.begin(); it != valueList.end(); ++it)
  {
    std::cout << *(it->begin());
    for (auto valueIt = ++(it->begin()); valueIt != it->end(); ++valueIt)
    {
      std::cout << " " << *valueIt;
    }
    std::cout << "\n";
  }

  std::list< unsigned long long > sumList;

  for (auto it = valueList.begin(); it != valueList.end(); ++it)
  {
    unsigned long long sum = 0;
    for (auto valueIt = it->begin(); valueIt != it->end(); ++valueIt)
    {
      if (sum > std::numeric_limits< unsigned long long >::max() - *valueIt)
      {
        std::cerr << "Overflow!\n";
        return 1;
      }
      sum += *valueIt;
    }
    sumList.push_back(sum);
  }

  std::cout << *(sumList.begin());
  for (auto it = ++(sumList.begin()); it != sumList.end(); ++it)
  {
    std::cout << " " << *it;
  }
  std::cout << "\n";
  return 0;
}
