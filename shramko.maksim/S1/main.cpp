#include <iostream>
#include <string>
#include <limits>
#include "functionsForList.hpp"

int main()
{
  using namespace shramko;
  PairList data;
  std::string name;

  while (std::cin >> name)
  {
    NumberList numbers;
    unsigned long long num;

    while (std::cin >> num)
    {
      numbers.addToBack(num);
    }

    data.addToBack(std::make_pair(name, numbers));

    if (std::cin.eof())
    {
      break;
    }
    std::cin.clear();
  }

  printNames(data, std::cout);

  size_t maxLength = 0;
  for (const auto& pair: data)
  {
    if (pair.second.getSize() > maxLength)
    {
      maxLength = pair.second.getSize();
    }
  }

  bool hasOverflow = false;
  processLists(data, maxLength, hasOverflow, std::cout);

  if (hasOverflow)
  {
    std::cerr << "SUM OVERFLOW DETECTED!\n";
    return 1;
  }

  return 0;
}
