#include "output.hpp"

using UllList = karnauhova::FwdList< unsigned long long >;

void karnauhova::output_names(karnauhova::FwdList< std::pair< std::string, UllList > > l, std::ostream& out)
{
  if (l.size() == 0)
  {
    return;
  }
  auto it = l.begin();
  out << it->first;
  for (size_t i = (l.size() - 1); i > 0; i--)
  {
    it++;
    out << " " << it->first;
  }
  out << "\n";
}

void karnauhova::output_element_lists(UllList lists, size_t index, std::ostream& out, bool& first)
{
  size_t count = 1;
  auto it = lists.begin();
  if (lists.size() < index)
  {
    return;
  }
  while (count < index)
  {
    it++;
    count++;
  }
  if (!first)
  {
    out << " ";
  }
  first = false;
  out << *it;
}

void karnauhova::output_lists(karnauhova::FwdList< std::pair< std::string, UllList > > l, std::ostream& out)
{
  size_t max_length = max_lenght(l);
  for (size_t i = 0; i < max_length; ++i)
  {
    bool first = true;
    for (const auto& it: l)
    {
      output_element_lists(it.second, (i + 1), out, first);
    }
    out << "\n";
  }
}

size_t karnauhova::max_lenght(karnauhova::FwdList< std::pair< std::string, UllList > > l)
{
  size_t max = 0;
  for (const auto& it: l)
  {
    if ((it.second.size()) > max)
    {
      max = it.second.size();
    }
  }
  return max;
}
