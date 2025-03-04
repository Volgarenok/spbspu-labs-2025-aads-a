#include <iostream>
#include <string>
#include <list>
#include "bidirList.hpp"

int main() 
{
  std::list<std::pair<std::string, std::list<int>>> sequences;
  std::string name;
  while (!std::cin.eof() && std::cin >> name) 
  {
    std::list<int> numbers;
    int num;
    while (std::cin >> num) 
    {
      numbers.push_back(num);
    }
  }
  if (sequences.empty()) 
  {
    std::cout << "0\n";
    return 0;
  }
  for (auto it = sequences.begin(); it != sequences.end(); ++it) 
  {
    std::cout << it->first;
  }
  std::cout << "\n";
  bool hasNumbers = true;
  while (hasNumbers) 
  {
    hasNumbers = false;
    for (auto it = sequences.begin(); it != sequences.end(); ++it) 
    {
      if (!it->second.empty()) 
      {
        std::cout << it->second.front();
        it->second.pop_front();
        hasNumbers = true;
      }
    }
    std::cout << "\n";
  }
  std::list<int> sums;
  for (auto it = sequences.begin(); it != sequences.end(); ++it) 
  {
    int sum = 0;
    for (auto numIt = it->second.begin(); numIt != it->second.end(); ++numIt) 
    {
      sum += *numIt;
    }
    sums.push_back(sum);
  }
  if (sums.empty()) 
  {
    std::cerr << "error: \n";
    return 1;
  }
  for (auto it = sums.begin(); it != sums.end(); ++it) 
  {
    std::cout << *it;
  }
  std::cout << "\n";
  return 0;
}
