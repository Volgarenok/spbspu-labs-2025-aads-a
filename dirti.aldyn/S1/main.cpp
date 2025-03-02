#include <iostream>
#include <list>
#include <utility>
#include "outputList.hpp"
#include <limits>

int main()
{
  std::list< std::pair< std::string, std::list< int >>> mainList;
  std::string listName;
  while (std::cin >> listName && !std::cin.eof())
  {
    int value = 0;
    std::list< int > valueList;
    while (std::cin >> value)
    {
      valueList.push_back(value);
    }
    mainList.push_back({ listName, valueList });
    if (!std::cin.eof())
    {
      std::cin.clear();
    }
  }
  if (mainList.empty())
  {
    std::cerr << "Empty input" << "\n";
    return 1;
  }
  try
  {
    dirti::outputList(mainList, std::cout);
  }
  catch (std::overflow_error&)
  {
    std::cerr << "Big number" << "\n";
    return 1;
  }
}
