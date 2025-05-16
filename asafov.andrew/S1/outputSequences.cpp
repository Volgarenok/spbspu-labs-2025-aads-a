#include "sequencefunctions.h"
#include <limits>
namespace
{
  bool allItersEnds(asafov::data_list_t::const_iterator* begins, asafov::data_list_t::const_iterator* ends, size_t size)
  {
    for (size_t i = 0; i < size; i++)
    {
      if (begins[i] != ends[i])
      {
        return false;
      }
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

  data_list_t::const_iterator* begins = new data_list_t::const_iterator[sequences.size() * 2];
  data_list_t::const_iterator* ends = &begins[sequences.size()];
  sequence_list_t::const_iterator seqiter = sequences.cbegin();
  size_t size = 0;
  auto iter = sequences.cbegin();
  out << iter->first;
  begins[0] = iter->second.cbegin();
  ends[0] = iter->second.cend();
  ++iter;
  ++size;
  for (; iter != sequences.cend(); ++iter)
  {
    out << ' ' << iter->first;
    begins[size] = iter->second.cbegin();
    ends[size] = iter->second.cend();
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
    for (; pos < size;)
    {
      if(begins[pos] != ends[pos])
      {
        if(sum > std::numeric_limits<data_t>::max() - *begins[pos])
        {
          isAllItersEnds = false;
        } else
        {
          sum += *begins[pos];
        }
        out << *begins[pos];
        ++begins[pos++];
        break;
      }
      else
      {
        pos++;
      }
    }
    for (; pos < size;)
    {
      if (begins[pos] != ends[pos])
      {
        if (sum > std::numeric_limits<data_t>::max() - *begins[pos])
        {
          isAllItersEnds = false;
        }
        else
        {
          sum += *begins[pos];
        }
        out << ' ' << *begins[pos];
        ++begins[pos++];
      }
      else
      {
        pos++;
      }
    }
    out << '\n';
    sums.push_back(sum);
  }

  if (isAllItersEnds == false)
  {
    sums.clear();
    delete[] begins;
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
  delete[] begins;
}
