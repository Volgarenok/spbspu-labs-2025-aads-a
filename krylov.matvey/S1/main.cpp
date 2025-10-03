#include <iostream>
#include <string>
#include <limits>
#include "list.hpp"

int main()
{
  using namespace krylov;
  List< std::pair< std::string, List< unsigned long long > > > listOfPairs;
  std::string line;
  std::cin >> line;
  size_t maxSizeOfNumList = 0;
  while (!std::cin.eof())
  {
    std::pair< std::string, List< unsigned long long > > pair;
    pair.first = line;
    std::cin >> line;
    while (!std::cin.eof() && std::isdigit(line[0]))
    {
      pair.second.push_back(std::strtoull(line.c_str(), nullptr, 0));
      std::cin >> line;
    }
    maxSizeOfNumList = std::max(pair.second.size(), maxSizeOfNumList);
    listOfPairs.push_back(pair);
  }
  if (listOfPairs.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  for (auto it = listOfPairs.begin(); it != --(listOfPairs.end()); ++it)
  {
    std::cout << it->first << " ";
  }
  std::cout << (--listOfPairs.end())->first << "\n";
  List< unsigned long long > listOfLineSums;
  bool wasOverflow = false;
  unsigned long long maxValue = std::numeric_limits< unsigned long long >::max();
  for (size_t i = 0; i < maxSizeOfNumList; ++i)
  {
    unsigned long long sumOfLine = 0;
    for (auto it = listOfPairs.begin(); it != listOfPairs.end(); ++it)
    {
      if (it->second.size() <= i)
      {
        continue;
      }
      auto currentValue = std::next(it->second.begin(), i);
      if (maxValue - sumOfLine < *currentValue)
      {
        wasOverflow = true;
        break;
      }
      sumOfLine += *currentValue;
    }
    listOfLineSums.push_back(sumOfLine);
  }

  for (size_t i = 0; i < maxSizeOfNumList; ++i)
  {
    bool isSpaceAvailable = false;
    for (auto it = listOfPairs.begin(); it != --(listOfPairs.end()); ++it)
    {
      if (it->second.size() > i)
      {
        if (isSpaceAvailable)
        {
          std::cout << " ";
        }
        auto current = std::next(it->second.begin(), i);
        std::cout << *current;
        isSpaceAvailable = true;
      }
    }
    if ((--(listOfPairs.end()))->second.size() <= i)
    {
      std::cout << "\n";
      continue;
    }
    std::cout << " " << *(std::next((--(listOfPairs.end()))->second.begin(), i)) << "\n";
  }

  if (wasOverflow)
  {
    std::cerr << "Overflow!\n";
    return 1;
  }
  if (listOfLineSums.size())
  {
    for (auto it = listOfLineSums.begin(); it != --(listOfLineSums.end()); ++it)
    {
      std::cout << *it << " ";
    }
    std::cout << *(--(listOfLineSums.end())) << "\n";
  }
  else
  {
    std::cout << "0\n";
  }
}

