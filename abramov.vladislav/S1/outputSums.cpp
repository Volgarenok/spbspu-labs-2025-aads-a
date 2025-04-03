#include "outputSums.hpp"
#include <stdexcept>
#include <limits>

namespace
{
  using list_of_lists = abramov::List< std::pair < std::string, abramov::List< unsigned long long int > > >*;
  unsigned long long int *calcSum(const list_of_lists lists, size_t count)
  {
    constexpr unsigned long long int max = std::numeric_limits< unsigned long long int >::max();
    unsigned long long int *sums = new unsigned long long int[count];
    for (size_t i = 0; i < count; ++i)
    {
      unsigned long long int sum = 0;
      for (auto iter = lists->begin(); iter != lists->end(); ++iter)
      {
        if (iter->second.size() > i)
        {
          auto iter2 = std::next(iter->second.begin(), i);
          if (*iter2 > max - sum)
          {
            delete[] sums;
            throw std::logic_error("overflow\n");
          }
          sum += *iter2;
        }
      }
      sums[i] = sum;
    }
    return sums;
  }
}

std::ostream &abramov::outputSums(std::ostream &out, const list_of_lists lists, size_t count)
{
  unsigned long long int *sums = calcSum(lists, count);
  bool empty = true;
  for (size_t i = 0; i < count; ++i)
  {
    if (sums[i] != 0)
    {
      empty = false;
      break;
    }
  }
  if (empty)
  {
    out << "0\n";
    delete[] sums;
    return out;
  }
  for (size_t i = 0; i < count - 1; ++i)
  {
    out << sums[i] << " ";
  }
  out << sums[count - 1] << "\n";
  delete[] sums;
  return out;
}
