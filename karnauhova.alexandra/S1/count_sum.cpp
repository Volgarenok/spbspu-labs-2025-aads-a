#include "count_sum.hpp"
#include <limits>
#include "output.hpp"

int karnauhova::element_lists(karnauhova::List<unsigned long long> lists, size_t index)
{
  size_t count = 1;
  auto it = lists.begin();
  while (it != lists.end() && count < index)
  {
    it++;
    count++;
  }
  if (it == lists.end())
  {
    return 0;
  }
  return *it;
}

void karnauhova::count_lists(karnauhova::List<std::pair<std::string, karnauhova::List<unsigned long long>>> l, std::ostream& out)
{
  size_t max_length = max_lenght(l);
  unsigned long long sum = 0;
  if (max_length == 0)
  {
    out << 0 << "\n";
    return;
  }
  List< unsigned long long > sums;
  for (size_t i = 0; i < max_length; ++i)
  {
    for (const auto& it : l)
    {
      unsigned long long element = element_lists(it.second, (i + 1));
      if (std::numeric_limits<unsigned long long>::max() - element < sum)
      {
        throw std::logic_error("Incorrect sum");
      }
      sum += element;
    }
    sums.push_front(sum);
    sum = 0;
  }
  sums.reverse();
  out << sums.front();
  auto it = sums.begin();
  it++;
  for (size_t i = 1; i < max_length; i++)
  {
    out << " ";
    out << it.node->data;
    it++;
  }
  out << "\n";
}
