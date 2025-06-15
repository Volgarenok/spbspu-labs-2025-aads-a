#include "print_results.hpp"
#include <limits>
#include <list/list_utils.hpp>

void smirnov::printName(std::ostream & out, const List< std::pair< std::string, List< size_t > > > & sequences)
{
  if (sequences.empty())
  {
    out << "\n";
    return;
  }
  auto it = sequences.cbegin();
  out << it->first;
  for (auto it = ++sequences.cbegin(); it != sequences.cend(); ++it)
  {
    out << " " << it->first;
  }
  out << "\n";
}

void smirnov::printSequences(std::ostream & out, const List< List< size_t > > & sequences)
{
  for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
  {
    if (it->empty())
    {
      out << "\n";
      continue;
    }
    auto numIt = it->cbegin();
    out << *numIt;
    for (auto numIt = ++it->cbegin(); numIt != it->cend(); ++numIt)
    {
      out << " " << *numIt;
    }
    out << "\n";
  }
}

void smirnov::printSums(std::ostream & out, const List< List< size_t > > & sequences)
{
  List< size_t > sums;
  for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
  {
    size_t sum = 0;
    for (auto numIt = it->cbegin(); numIt != it->cend(); ++numIt)
    {
      if (sum > std::numeric_limits< size_t >::max() - *numIt)
      {
        throw std::overflow_error("Overflow!");
      }
      sum += *numIt;
    }
    pushBack(sums, sum);
  }
  if (!sums.empty())
  {
    auto it = sums.cbegin();
    out << *it;
    for (++it; it != sums.cend(); ++it)
    {
      out << " " << *it;
    }
  }
  out << "\n";
}
