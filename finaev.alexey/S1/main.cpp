#include <iostream>
#include "list.hpp"
#include "actionsWithList.hpp"

int main()
{
  finaev::List< std::pair< std::string, finaev::List< size_t > > > List;
  std::string str = "";
  while (std::cin >> str)
  {
    finaev::List< size_t > list;
    size_t num = 0;
    while (std::cin >> num)
    {
      list.pushBack(num);
    }
    List.pushBack(std::make_pair(str, list));
    if (std::cin.eof())
    {
      break;
    }
    std::cin.clear();
  }
  finaev::List< std::string > listOfHeads;
  for (auto i = List.begin(); i != List.end(); ++i)
  {
    listOfHeads.pushBack((*i).first);
  }
  if (listOfHeads.isEmpty())
  {
    std::cout << "0\n";
    return 0;
  }
  finaev::printList(listOfHeads, std::cout);
  finaev::List< finaev::List< size_t > > listOfLists = finaev::createRightList(List);
  if (listOfLists.size() == 0)
  {
    std::cout << 0 << "\n";
    return 0;
  }
  for (auto i = listOfLists.begin(); i != listOfLists.end(); ++i)
  {
    finaev::printList(*i, std::cout);
  }
  if (finaev::isOverflow(listOfLists))
  {
    std::cerr << "is overflow!\n";
    return 1;
  }
  finaev::List< size_t > listOfSum = finaev::createListOfSum(listOfLists);
  finaev::printList(listOfSum, std::cout);
}
