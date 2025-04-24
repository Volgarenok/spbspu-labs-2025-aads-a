#include <iostream>
#include <utility>
#include <string>
#include <limits>
#include "list.hpp"

namespace
{
  using pair_t = duhanina::List< std::pair< std::string, duhanina::List< unsigned long long > > >;

  void printNames(const pair_t& listPair)
  {
    std::cout << listPair.begin()->first;
    for (auto it = ++listPair.begin(); it != listPair.end(); ++it)
    {
      std::cout << " " << it->first;
    }
    std::cout << "\n";
  }

  duhanina::List< duhanina::List< unsigned long long > > generateSequences(const pair_t sequences)
  {
    duhanina::List< duhanina::List< unsigned long long > > result;
    size_t maxLen = 0;
    for (auto it = sequences.begin(); it != sequences.end(); ++it)
    {
      if (it->second.size() > maxLen)
      {
        maxLen = it->second.size();
      }
    }
    for (size_t i = 0; i < maxLen; ++i)
    {
      duhanina::List< unsigned long long > newSeq;
      for (auto it = sequences.begin(); it != sequences.end(); ++it)
      {
        if (i < it->second.size())
        {
          auto numIt = it->second.begin();
          for (size_t j = 0; j < i; ++j)
          {
            ++numIt;
          }
          newSeq.push_back(*numIt);
        }
      }
      result.push_back(newSeq);
    }
    return result;
  }

  void printListPair(const duhanina::List< duhanina::List< unsigned long long > >& listPair)
  {
    for (auto it = listPair.begin(); it != listPair.end(); ++it)
    {
      std::cout << *it->begin();
      for (auto numIt = ++it->begin(); numIt != it->end(); ++numIt)
      {
        std::cout << " " << *numIt;
      }
      std::cout << "\n";
    }
  }

  void printSums(const duhanina::List< duhanina::List< unsigned long long > >& listPair)
  {
    if (listPair.empty())
    {
      std::cout << "0\n";
      return;
    }
    duhanina::List< unsigned long long > sums;
    for (auto it = listPair.begin(); it != listPair.end(); ++it)
    {
      unsigned long long sum = 0;
      for (auto numIt = it->begin(); numIt != it->end(); ++numIt)
      {
        if (sum > std::numeric_limits< unsigned long long >::max() - *numIt)
        {
          throw std::overflow_error("Overflow");
        }
        sum += *numIt;
      }
      sums.push_back(sum);
    }
    std::cout << *sums.begin();
    for (auto it = ++sums.begin(); it != sums.end(); ++it)
    {
      std::cout << " " << *it;
    }
    std::cout << "\n";
  }
}

int main()
{
  using namespace duhanina;
  List< std::pair< std::string, List< unsigned long long > > > listPair;
  std::string name;
  while (std::cin >> name)
  {
    List< unsigned long long > numbersList;
    unsigned long long number = 0;
    while (std::cin >> number)
    {
      numbersList.push_back(number);
    }
    listPair.push_back(std::make_pair(name, numbersList));
    std::cin.clear();
  }
  if (listPair.empty())
  {
    std::cout << "0\n";
    return 0;
  }
  printNames(listPair);
  auto resultListPair = generateSequences(listPair);
  printListPair(resultListPair);
  try
  {
    printSums(resultListPair);
  }
  catch (const std::exception&)
  {
    std::cerr << "Error\n";
    return 1;
  }
  return 0;
}
