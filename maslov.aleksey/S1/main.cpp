#include <iostream>
#include <string>
#include "list.hpp"
#include "listUtils.hpp"

int main()
{
  maslov::FwdList< std::pair< std::string, maslov::FwdList< size_t > > > listOfPairs;
  std::string name;
  size_t number = 0;
  size_t maxSize = 0;
  while ((std::cin >> name) && !std::cin.eof())
  {
    maslov::FwdList< size_t > numbers;
    while ((std::cin >> number) && !std::cin.eof())
    {
      numbers.pushFront(number);
    }
    numbers.reverse();
    maxSize = std::max(maxSize, numbers.size());
    listOfPairs.pushFront(std::make_pair(name, numbers));
    std::cin.clear();
  }
  listOfPairs.reverse();
  if (listOfPairs.empty())
  {
    std::cout << "0\n";
    return 0;
  }
  try
  {
    maslov::printData(std::cout, listOfPairs, maxSize);
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  std::cout << "\n";
}
