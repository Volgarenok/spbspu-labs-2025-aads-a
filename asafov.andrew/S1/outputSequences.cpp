#include "sequencefunctions.h"
#include <limits>
namespace
{
  bool allItersEnds(
    asafov::ForwardList< asafov::data_list_t::ConstIterator >& begins,
    asafov::ForwardList< asafov::data_list_t::ConstIterator >& ends,
    size_t size)
  {
    auto beginIt = begins.begin();
    auto endIt = ends.begin();
    for (size_t i = 0; i < size; i++)
    {
      if (*beginIt != *endIt)
      {
        return false;
      }
      ++beginIt;
      ++endIt;
    }
    return true;
  }
}

void asafov::outputSequences(sequence_list_t& sequences, std::ostream& out = std::cout)
{
  if (sequences.cbegin()->first == "")
  {
    out << "0\n";
    return;
  }
  if (sequences.cbegin()->second.empty())
  {
    out << sequences.cbegin()->first << "\n0\n";
    return;
  }

  ForwardList< data_list_t::ConstIterator > begins;
  ForwardList< data_list_t::ConstIterator > ends;
  size_t size = 0;
  auto iter = sequences.cbegin();
  out << iter->first;
  begins.push_back(iter->second.cbegin());
  ends.push_back(iter->second.cend());
  ++iter;
  ++size;
  for (; iter != sequences.cend(); ++iter)
  {
    out << ' ' << iter->first;
    begins.push_back(iter->second.cbegin());
    ends.push_back(iter->second.cend());
    ++size;
  }
  if (sequences.size() != 0)
  {
    out << '\n';
  }

  data_list_t sums;
  bool isAllItersEnds = true;
  while (!allItersEnds(begins, ends, size))
  {
    data_t sum = 0;
    size_t pos = 0;
    auto beginIt = begins.begin();
    auto endIt = ends.begin();
    for (; pos < size;)
    {
      if (*beginIt != *endIt)
      {
        if (sum > std::numeric_limits< data_t >::max() - **beginIt)
        {
          isAllItersEnds = false;
        }
        else
        {
          sum += **beginIt;
        }
        out << **beginIt;
        ++(*beginIt);
        ++beginIt;
        ++endIt;
        ++pos;
        break;
      }
      else
      {
        ++beginIt;
        ++endIt;
        ++pos;
      }
    }
    for (; pos < size;)
    {
      if (*beginIt != *endIt)
      {
        if (sum > std::numeric_limits< data_t >::max() - **beginIt)
        {
          isAllItersEnds = false;
        }
        else
        {
          sum += **beginIt;
        }
        out << ' ' << **beginIt;
        ++(*beginIt);
        ++beginIt;
        ++endIt;
        ++pos;
      }
      else
      {
        ++beginIt;
        ++endIt;
        ++pos;
      }
    }
    out << '\n';
    sums.push_back(sum);
  }

  if (isAllItersEnds == false)
  {
    sums.clear();
    throw std::overflow_error("owerflow!");
  }

  auto it = sums.cbegin();
  out << *it;
  ++it;
  for (; it != sums.cend(); ++it)
  {
    out << ' ' << *it;
  }
  out << '\n';
}
