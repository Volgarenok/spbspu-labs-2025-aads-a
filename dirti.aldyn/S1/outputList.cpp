#include "outputList.hpp"
#include "outputList.hpp"
#include <stdexcept>

namespace
{
  void checkNum(int num1, int num2)
  {
    if (num1 > std::numeric_limits< int >::max() - num2)
    {
      throw std::overflow_error("big number");
    }
  }
}

void dirti::outputList(const std::list< std::pair< std::string, std::list< int >>> mainList, std::ostream& out)
{
  auto iter = mainList.begin();
  std::list< std::list< int >> valueList;
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
  std::list< int > sumList;
  for (size_t i = 0; i < size; ++i)
  {
    int s = 0;
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
