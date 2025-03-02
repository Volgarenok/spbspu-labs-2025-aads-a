#include "outputList.hpp"
#include <stdexcept>

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
  for (iter; iter != mainList.end(); ++iter)
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
  }
  out << '\n';
  for (size_t i = 0; i < size; ++i)
  {
    auto valueIter = valueList.begin();
    while ((*valueIter).empty())
    {
      ++valueIter;
    }
    out << (*valueIter).front();
    (*valueIter++).pop_front();
    for (valueIter; valueIter != valueList.end(); ++valueIter)
    {
      if (!((*valueIter).empty()))
      {
        out << " " << (*valueIter).front();
        (*valueIter).pop_front();
      }
    }
    out << "\n";
  }
  return;
}
