#include "actionsWithList.hpp"
#include <ostream>

namespace
{
  std::pair< finaev::List< size_t >, bool > listSumAndOverflow(finaev::List< finaev::List< size_t > > list)
  {
    bool isOverflow = 0;
    finaev::List< size_t > listSum;
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
      listSum.pushBack(res);
    }
    return { listSum, isOverflow };
  }
}

finaev::List< finaev::List< size_t > > finaev::createRightList(List< std::pair< std::string, List< size_t > > > List)
{
  finaev::List< finaev::List< size_t > > listOfLists;
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
    finaev::List< size_t > list;
    listOfLists.pushBack(list);
  }
  for (auto i = List.begin(); i != List.end(); ++i)
  {
    auto counter = listOfLists.begin();
    for (auto j = (*i).second.begin(); j != (*i).second.end(); ++j)
    {
      (*counter).pushBack(*j);
      if (counter != listOfLists.end())
      {
        ++counter;
      }
    }
  }
  return listOfLists;
}

void finaev::printList(List< size_t > list, std::ostream& out)
{
  if (list.isEmpty())
  {
    return;
  }
  out << *list.begin();
  for (auto i = ++list.begin(); i != list.end(); ++i)
  {
    out << " " << *i;
  }
  out << "\n";
}

void finaev::printList(const List< std::string >& list, std::ostream& out)
{
  out << *list.constBegin();
  for (auto i = ++list.constBegin(); i != list.constEnd(); ++i)
  {
    out << " " << *i;
  }
  out << "\n";
}

bool finaev::isOverflow(List< List< size_t > > list)
{
  return listSumAndOverflow(list).second;
}

finaev::List< size_t > finaev::createListOfSum(List< List< size_t > > list)
{
  return listSumAndOverflow(list).first;
}
