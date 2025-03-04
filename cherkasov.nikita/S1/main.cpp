#include <iostream>
#include <list>
#include <string>
#include <limits>
#include <algorithm>

int main()
{
  std::list<std::pair<std::string, std::list<unsigned long long>>> list;
  std::string in;
  std::cin >> in;
  size_t maxSize = 0;
  while (!std::cin.eof())
  {
    std::pair<std::string, std::list<unsigned long long>> pair;
    pair.first = in;
    std::cin >> in;
    while (!std::cin.eof() && std::all_of(in.begin(), in.end(), ::isdigit))
    {
      pair.second.push_back(strtoull(in.c_str(), nullptr, 0));
      std::cin >> in;
    }
    if (maxSize < pair.second.size())
    {
      maxSize = pair.second.size();
    }
    list.push_back(pair);
  }
  if (list.empty())
  {
    std::cout << "0\n";
    return 0;
  }
  for (auto it = list.begin(); it != --(list.end()); ++it)
  {
    std::cout << it->first << " ";
  }
  std::cout << (--list.end())->first << "\n";
  std::list<unsigned long long> sumList;
  bool overFlow = false;
  unsigned long long maxValue = std::numeric_limits<unsigned long long>::max();
  for (size_t i = 0; i < maxSize; ++i)
  {
    unsigned long long sumOfLine = 0;
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      if (it->second.size() <= i)
      {
        continue;
      }
      auto currentValue = std::next(it->second.begin(), i);
      if (maxValue - sumOfLine < *currentValue)
      {
        overFlow = true;
        break;
      }
      sumOfLine += *currentValue;
    }
    sumList.push_back(sumOfLine);
  }
  for (size_t i = 0; i < maxSize; ++i)
  {
    bool wasAvailable = false;
    for (auto it = list.begin(); it != --(list.end()); ++it)
    {
      if (it->second.size() > i)
      {
        if (wasAvailable)
        {
          std::cout << " ";
        }
        auto current = std::next(it->second.begin(), i);
        std::cout << *current;
        wasAvailable = true;
       }
     }
     if ((--(list.end()))->second.size() <= i)
     {
       std::cout << "\n";
        continue;
     }
     std::cout << " " << *(std::next((--(list.end()))->second.begin(), i)) << "\n";
  }
  if (overFlow)
  {
    std::cerr << "overflow!\n";
    return 1;
  }
  for (auto it = sumList.begin(); it != --(sumList.end()); ++it)
  {
    std::cout << *it << " ";
  }
  std::cout << *(--(sumList.end())) << "\n";
  return 0;
}
