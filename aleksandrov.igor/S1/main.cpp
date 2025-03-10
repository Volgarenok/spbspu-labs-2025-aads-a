#include <iostream>
#include <string>
#include "list.hpp"
#include "list-utils.hpp"

int main()
{
  aleksandrov::List< std::pair< std::string, aleksandrov::List< unsigned long long > > > list;
  std::string listName;
  size_t maxSubListSize = 0;
  while (std::cin >> listName)
  {
    aleksandrov::List< unsigned long long > numList;
    unsigned long long num = 0;
    size_t numCount = 0;
    while (std::cin >> num)
    {
      numList.pushBack(num);
      ++numCount;
      if (maxSubListSize < numCount)
      {
        maxSubListSize = numCount;
      }
    }
    std::cin.clear();
    list.pushBack(std::make_pair(listName, numList));
  }

  aleksandrov::List< std::string > nameList;
  for (auto it = list.begin(); it != list.end(); ++it)
  {
    nameList.pushBack(it->first);
  }

  if (nameList.empty())
  {
    std::cout << "0" << "\n";
    return 0;
  }
  std::cout << nameList.front();
  for (auto it = ++nameList.begin(); it != nameList.end(); ++it)
  {
    std::cout << " " << *it;
  }
  std::cout << "\n";

  aleksandrov::List< aleksandrov::List< unsigned long long > > yaNumList;
  size_t shift = 0;
  while (shift != maxSubListSize)
  {
    aleksandrov::List< unsigned long long > numList;
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
    yaNumList.pushBack(numList);
  }

  if (yaNumList.empty())
  {
    std::cout << "0" << "\n";
    return 0;
  }
  for (auto it = yaNumList.begin(); it != yaNumList.end(); ++it)
  {
    aleksandrov::outputList(*it, std::cout);
    std::cout << "\n";
  }

  aleksandrov::List< unsigned long long > sumList;
  for (auto it = yaNumList.begin(); it != yaNumList.end(); ++it)
  {
    try
    {
      unsigned long long sum = aleksandrov::calcSum(*it);
      sumList.pushBack(sum);
    }
    catch (const std::overflow_error& e)
    {
      std::cerr << e.what() << "\n";
      return 1;
    }
  }
  aleksandrov::outputList(sumList, std::cout);
  std::cout << "\n";
}

