#include <iostream>
#include <utility>
#include <string>
#include <limits>
#include "list.hpp"

using mainList = averenkov::List< std::pair< std::string, averenkov::List< unsigned long long > > >;
using listUll = averenkov::List< unsigned long long >;
using listOfListUll = averenkov::List< listUll >;
mainList readInput()
{
  mainList list;
  std::string name;
  while (std::cin >> name)
  {
    listUll numbersList;
    unsigned long long number = 0;
    while (std::cin >> number)
    {
      numbersList.push_back(number);
    }
    list.push_back(std::make_pair(name, numbersList));
    std::cin.clear();
  }
  return list;
}

void printNames(const mainList& list)
{
  std::cout << list.begin()->first;
  for (auto it = ++(list.begin()); it != list.end(); it++)
  {
    std::cout << " " << it->first;
  }
  std::cout << "\n";
}

listOfListUll createResult(const mainList& list)
{
  listOfListUll result;
  size_t maxLen = 0;
  for (auto it = list.begin(); it != list.end(); it++)
  {
    if (it->second.size() > maxLen)
    {
      maxLen = it->second.size();
    }
  }
  for (size_t i = 0; i < maxLen; i++)
  {
    listUll sublist;
    for (auto it = list.begin(); it != list.end(); it++)
    {
      if (i < it->second.size())
      {
        auto numIt = it->second.begin();
        for (size_t j = 0; j < i; j++)
        {
          numIt++;
        }
        sublist.push_back(*numIt);
      }
    }
    result.push_back(sublist);
  }
  return result;
}

void printResult(const listOfListUll& result)
{
  for (auto it = result.begin(); it != result.end(); it++)
  {
    std::cout << *(it->begin());
    for (auto numIt = ++(it->begin()); numIt != it->end(); numIt++)
    {
      std::cout << " " << *numIt;
    }
    std::cout << "\n";
  }
}

void calcPrintSum(const listOfListUll& result)
{
  listUll sums {};
  for (auto it = result.begin(); it != result.end(); it++)
  {
    unsigned long long sum = 0;
    for (auto numIt = it->begin(); numIt != it->end(); numIt++)
    {
      if (sum > std::numeric_limits< unsigned long long >::max() - *numIt)
      {
        throw std::overflow_error("Overflow");
      }
      sum += *numIt;
    }
    sums.push_back(sum);
  }
  std::cout << *(sums.begin());
  for (auto it = ++(sums.begin()); it != sums.end(); it++)
  {
    std::cout << " " << *it;
  }
  std::cout << "\n";
}

int main()
{
  try
  {
    auto list = readInput();
    if (list.empty())
    {
      std::cout << "0\n";
      return 0;
    }
    printNames(list);
    auto result = createResult(list);
    printResult(result);
    calcPrintSum(result);
  }
  catch (...)
  {
    std::cerr << "Error\n";
    return 1;
  }
  return 0;
}
