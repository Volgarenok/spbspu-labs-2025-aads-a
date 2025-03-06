#include "actionsWithList.hpp"
#include <ostream>

namespace
{
  std::pair < std::list < size_t >, bool > listSumAndOverflow(std::list < std::list < size_t > > list)
  {
    bool isOverflow = 0;
    std::list < size_t > listSum;
    for (auto i = list.begin(); i != list.end(); ++i)
    {
      size_t res = 0;
      for (auto j = (*i).begin(); j != (*i).end(); ++j)
      {
        if ((res + *j) < res)
        {
          isOverflow = true;
        }
        res += *j;
      }
      listSum.push_back(res);
    }
    return { listSum, isOverflow };
  }
}

std::list < std::list < size_t > > finaev::createRightList(std::list< std::pair< std::string, std::list < size_t > > > List)
{
  std::list<std::list<size_t>> listOfLists;
  size_t max_size = 0;
  for (auto i = List.begin(); i != List.end(); ++i)
  {
    if ((*i).second.size() > max_size)
    {
      max_size = (*i).second.size();
    }
  }
  for (size_t i = 0; i < max_size; ++i)
  {
    std::list<size_t> list;
    listOfLists.push_back(list);
  }
  for (auto i = List.begin(); i != List.end(); ++i)
  {
    auto counter = listOfLists.begin();
    for (auto j = (*i).second.begin(); j != (*i).second.end(); ++j)
    {
      (*counter).push_back(*j);
      if (counter != listOfLists.end())
      {
        ++counter;
      }
    }
  }
  return listOfLists;
}

void finaev::printList(std::list< size_t > list, std::ostream& out)
{
  out << *list.begin();
  for (auto i = ++list.begin(); i != list.end(); ++i)
  {
    out << " " << *i;
  }
  out << "\n";
}

void finaev::printList(std::list<std::string> list, std::ostream& out)
{
  out << *list.begin();
  for (auto i = ++list.begin(); i != list.end(); ++i)
  {
    out << " " << *i;
  }
  out << "\n";
}

bool finaev::isOverflow(std::list < std::list < size_t > > list)
{
  return listSumAndOverflow(list).second;
}

std::list < size_t > finaev::createListOfSum(std::list < std::list < size_t > > list)
{
  return listSumAndOverflow(list).first;
}
