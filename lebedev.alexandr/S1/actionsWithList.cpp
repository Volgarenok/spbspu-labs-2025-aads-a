#include "actionsWithList.hpp"
#include <limits>
#include <ostream>

void lebedev::printList(const lebedev::List< std::string >& list, std::ostream& output)
{
  if (list.empty())
  {
    return;
  }
  auto current = list.constBegin();
  output << *current;
  ++current;

  for (; current != list.constEnd(); current++)
  {
    output << ' ' << *current;
  }
  output << '\n';
}

void lebedev::printList(const lebedev::List< size_t >& list, std::ostream& output)
{
  if (list.empty())
  {
    return;
  }
  auto current = list.constBegin();
  output << *current;
  ++current;

  for (; current != list.constEnd(); ++current)
  {
    output << ' ' << *current;
  }
  output << '\n';
}

using listPair = lebedev::List< std::pair< std::string, lebedev::List< size_t > > >;
lebedev::List< lebedev::List< size_t > > lebedev::createReorderedList(const listPair& List)
{
  lebedev::List< lebedev::List< size_t > > reordered;
  size_t maxSize = 0;
  for (auto seq = List.constBegin(); seq != List.constEnd(); seq++)
  {
    if ((*seq).second.size() > maxSize)
    {
      maxSize = (*seq).second.size();
    }
  }
  for (size_t i = 0; i < maxSize; i++)
  {
    lebedev::List< size_t > seq;
    reordered.push_back(seq);
  }
  for (auto pair = List.constBegin(); pair != List.constEnd(); pair++)
  {
    auto it = reordered.begin();
    for (auto i = (*pair).second.constBegin(); i != (*pair).second.constEnd(); i++)
    {
      (*it).push_back(*i);
      if (it != reordered.end())
      {
        ++it;
      }
    }
  }
  return reordered;
}

std::pair< lebedev::List< size_t >, bool > lebedev::createListOfSum(const lebedev::List< lebedev::List< size_t > >& list)
{
  bool isOverflow = false;
  lebedev::List< size_t > listOfSum;
  for (auto seq = list.constBegin(); seq != list.constEnd(); seq++)
  {
    size_t res = 0;
    for (auto current = seq->constBegin(); current != seq->constEnd(); current++)
    {
      const size_t cur = *current;
      if (res > std::numeric_limits< int >::max() - cur)
      {
        isOverflow = true;
      }
      res += (*current);
    }
    listOfSum.push_back(res);
  }
  return { listOfSum, isOverflow };
}
