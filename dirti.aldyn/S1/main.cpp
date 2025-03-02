#include <iostream>
#include <list>
#include <utility>

int main()
{
  std::list< std::pair< std::string, std::list< int >>> mainlist;
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
  }
  
}
