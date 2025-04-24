#include <iostream>
#include <string>
#include <limits>
#include "list.hpp"

namespace
{
  using listOfPairs = mozhegova::List< std::pair< std::string, mozhegova::List< unsigned long long > > >;

  size_t getMax(const listOfPairs & list)
  {
    size_t maxNums = 0;
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      maxNums = std::max(maxNums, it->second.size());
    }
    return maxNums;
  }

  void printNums(std::ostream & out, const listOfPairs & list)
  {
    for (size_t i = 0; i < getMax(list); i++)
    {
      auto it = list.begin();
      auto currIt = it->second.begin();
      if (it->second.size() > i)
      {
        std::advance(currIt, i);
        out << *currIt;
      }
      else
      {
        while (it->second.size() <= i)
        {
          ++it;
        }
        currIt = it->second.begin();
        std::advance(currIt, i);
        out << *currIt;
      }
      ++it;
      for (; it != list.end(); ++it)
      {
        currIt = it->second.begin();
        if (it->second.size() > i)
        {
          std::advance(currIt, i);
          out << " " << *currIt;
        }
      }
      out << "\n";
    }
  }

  void addWithCheck(unsigned long long & sum, const unsigned long long n)
  {
    const unsigned long long max = std::numeric_limits< unsigned long long >::max();
    if (max - n < sum)
    {
      throw std::overflow_error("Overflow");
    }
    sum += n;
  }

  mozhegova::List< unsigned long long > calculateSum(const listOfPairs & list)
  {
    mozhegova::List< unsigned long long > sumList;
    for (size_t i = 0; i < getMax(list); i++)
    {
      unsigned long long sum = 0;
      auto it = list.begin();
      for (; it != list.end(); ++it)
      {
        auto currIt = it->second.begin();
        if (it->second.size() > i)
        {
          std::advance(currIt, i);
          addWithCheck(sum, *currIt);
        }
      }
      sumList.push_back(sum);
    }
    return sumList;
  }

  void printSums(std::ostream & out, const mozhegova::List< unsigned long long > sumList)
  {
    out << *sumList.begin();
    for (auto it = ++sumList.begin(); it != sumList.end(); ++it)
    {
      out << " " << *it;
    }
    out << "\n";
  }
}

int main()
{
  using namespace mozhegova;
  List< std::pair< std::string, List< unsigned long long > > > listOfPairs;
  std::string nameList;
  while (std::cin >> nameList)
  {
    List< unsigned long long > numList;
    unsigned long long num = 0;
    while (std::cin >> num)
    {
      numList.push_back(num);
    }
    listOfPairs.push_back(std::make_pair(nameList, numList));
    std::cin.clear();
  }

  if (listOfPairs.size() == 0)
  {
    std::cout << "0\n";
    return 0;
  }

  std::cout << listOfPairs.begin()->first;
  for (auto it = ++listOfPairs.begin(); it != listOfPairs.end(); ++it)
  {
    std::cout << " " << it->first;
  }
  std::cout << "\n";

  if (listOfPairs.front().second.empty())
  {
    std::cout << "0\n";
    return 0;
  }
  printNums(std::cout, listOfPairs);
  List< unsigned long long > sumList;
  try
  {
    sumList = calculateSum(listOfPairs);
  }
  catch (const std::overflow_error & e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  printSums(std::cout, sumList);
}
