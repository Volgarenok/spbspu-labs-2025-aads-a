#include "list_actions.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include "../common/list.hpp"

namespace
{
  bool summ_overflow(size_t a, size_t b)
  {
    size_t n = std::numeric_limits< size_t >::max();
    return (n - a < b);
  }
}

std::ostream & evstyunichev::list_out(const List< pair_t > &sequences, std::ostream &out)
{
  bool overflow_flag = false;
  if (sequences.empty())
  {
    out << "0";
    return out;
  }
  List< size_t > summ{};
  List< ConstListIterator< size_t > > iters{};
  auto iter_seq = sequences.cbegin();
  iters.push_back(iter_seq->second.cbegin());
  size_t mx = iter_seq->second.size();
  out << (iter_seq++)->first;
  for (; iter_seq != sequences.cend(); ++iter_seq)
  {
    out << ' ' << iter_seq->first;
    mx = std::max(mx, iter_seq->second.size());
    iters.push_back(iter_seq->second.cbegin());
  }
  if (!mx)
  {
    out << '\n';
    summ.push_back(0);
  }
  for (size_t i = 0; i < mx; i++)
  {
    auto iter_seq = sequences.cbegin();
    auto it_iters = iters.begin();
    out << '\n';
    for (; *it_iters == iter_seq->second.cend(); ++it_iters, ++iter_seq);
    size_t num = **it_iters;
    out << num;
    if (!overflow_flag)
    {
      summ.push_back(num);
    }
    ++iter_seq;
    ++(*it_iters);
    ++it_iters;
    for (; iter_seq != sequences.cend(); ++iter_seq, ++it_iters)
    {
      if (*it_iters == iter_seq->second.cend())
      {
        continue;
      }
      size_t num = **it_iters;
      ++(*it_iters);
      out << ' ' << num;
      if (!overflow_flag)
      {
        if (!summ_overflow(summ.back(), num))
        {
          summ.back() += num;
        }
        else
        {
          overflow_flag = 1;
        }
      }
    }
  }
  if (!overflow_flag)
  {
    if (mx)
    {
      out << '\n';
    }
    auto it = summ.begin();
    out << *(it++);
    for (; it != summ.end(); it++)
    {
      out << ' ' << *it;
    }
  }
  else
  {
    out << "\n";
    throw std::overflow_error("overflow!");
  }
  return out;
}
