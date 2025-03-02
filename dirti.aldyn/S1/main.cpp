#include <iostream>
#include <list>
#include <utility>

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
  auto it = mainList.begin();
  std::cout << it++->first;
  for (it; it != mainList.end(); ++it)
  {
    std::cout << " " << it->first;
  }
  std::cout << '\n';
}
