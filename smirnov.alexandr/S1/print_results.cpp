#include "print_results.hpp"
#include <limits>

void smirnov::printName(std::ostream & out, const List< std::pair< std::string, List< size_t > > > & sequences)
{
  for (auto it = sequences.begin(); it != sequences.end(); ++it)
  {
    if (it != sequences.begin())
    {
      out << " ";
    }
    out << it->first;
  }
  out << "\n";
}

void smirnov::printSequences(std::ostream & out, const List< List< size_t > > & sequences)
{
  for (auto it = sequences.begin(); it != sequences.end(); ++it)
  {
    for (auto numIt = it->begin(); numIt != it->end(); ++numIt)
    {
      if (numIt != it->begin())
      {
        out << " ";
      }
      out << *numIt;
    }
    out << "\n";
  }
}

void smirnov::printSums(std::ostream & out, const List< List< size_t > > & sequences)
{
  List< size_t > sums;
  for (auto it = sequences.begin(); it != sequences.end(); ++it)
  {
    size_t sum = 0;
    for (auto numIt = it->begin(); numIt != it->end(); ++numIt)
    {
      if (sum > std::numeric_limits< size_t >::max() - *numIt)
      {
        throw std::overflow_error("Overflow!");
      }
      sum += *numIt;
    }
    sums.push_back(sum);
  }
  for (auto it = sums.begin(); it != sums.end(); ++it)
  {
    if (it != sums.begin())
    {
      out << " ";
    }
    out << *it;
  }
  out << "\n";
}
