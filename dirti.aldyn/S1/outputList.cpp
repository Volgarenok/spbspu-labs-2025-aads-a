#include "outputList.hpp"
#include "outputList.hpp"
#include <limits>

namespace
{
  void checkNum(size_t num1, size_t num2)
  {
    if (num1 > std::numeric_limits< size_t >::max() - num2)
    {
      throw std::overflow_error("big number");
    }
  }
}

void dirti::outputList(const std::list< std::pair< std::string, std::list< size_t >>> mainList, std::ostream& out)
{
  auto iter = mainList.begin();
  std::list< std::list< size_t >> valueList;
  size_t size = 0;
  if (!(iter->second.empty()))
  {
    valueList.push_back(iter->second);
    size = iter->second.size();
  }
  out << iter++->first;
  while (iter != mainList.end())
  {
    out << " " << iter->first;
    if (!(iter->second.empty()))
    {
      valueList.push_back(iter->second);
      if (iter->second.size() > size)
      {
        size = iter->second.size();
      }
    }
    iter++;
  }
  out << '\n';
  std::list< size_t > sumList;
  for (size_t i = 0; i < size; ++i)
  {
    size_t s = 0;
    auto valueIter = valueList.begin();
    while ((*valueIter).empty())
    {
      ++valueIter;
    }
    s += (*valueIter).front();
    out << (*valueIter).front();
    (*valueIter++).pop_front();
    while (valueIter != valueList.end())
    {
      if (!((*valueIter).empty()))
      {
        out << " " << (*valueIter).front();
        checkNum(s, (*valueIter).front());
        s += (*valueIter).front();
        (*valueIter).pop_front();
      }
      valueIter++;
    }
    sumList.push_back(s);
    out << "\n";
  }
  if (!sumList.empty())
  {
    auto iter = sumList.begin();
    out << *iter++;
    while (iter != sumList.end())
    {
      out << " " << *iter++;
    }
    out << "\n";
  }
  else
  {
    out << 0 << "\n";
  }
  return;
}
