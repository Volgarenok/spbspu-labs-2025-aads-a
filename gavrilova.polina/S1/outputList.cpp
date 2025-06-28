#include "outputList.hpp"
#include <limits>

namespace {
  unsigned long long addition(unsigned long long first, unsigned long long second)
  {
    unsigned long long MAX_OF_ULL = std::numeric_limits< unsigned long long >::max();
    if (first > MAX_OF_ULL - second) {
      throw std::overflow_error("Overflow.");
    }
    return first + second;
  }

  template < typename T, typename Selector >
  std::ostream& outputListElements(std::ostream& out, const T& list, Selector select)
  {
    auto ptr = list.begin();
    auto end = list.end();
    if (ptr == end) {
      return out;
    }
    out << select(*ptr);
    ++ptr;
    while (ptr != end) {
      out << " " << select(*ptr);
      ++ptr;
    }
    return out;
  }

  struct SelectFirst {
    template < typename Pair >
    constexpr auto operator()(const Pair& pair) const
    {
      return pair.first;
    }
  };

  struct SelectSelf {
    template < typename T >
    constexpr const T& operator()(const T& value) const
    {
      return value;
    }
  };

}

std::ostream& gavrilova::outNames(std::ostream& out, gavrilova::FLPairs list)
{
  return outputListElements(out, list, SelectFirst());
}

std::ostream& gavrilova::outFwdListULL(std::ostream& out, const FwdList< ULL >& list)
{
  return outputListElements(out, list, SelectSelf());
}

gavrilova::FwdList< unsigned long long > gavrilova::outNumbers(std::ostream& out, FLPairs list, size_t maxLen, size_t n)
{
  FwdList< IteratorFwd< ULL > > ptr_list;
  FwdList< IteratorFwd< ULL > > ptr_end_list;
  FwdList< ULL > sums;

  auto ptr = list.begin();
  for (size_t i = 0; i < n && ptr != list.end(); ++i, ++ptr) {
    ptr_list.push_front(ptr->second.begin());
    ptr_end_list.push_front(ptr->second.end());
  }
  ptr_list.reverse();
  ptr_end_list.reverse();

  auto sums_tail = sums.begin();

  for (size_t i = 0; i < maxLen; ++i) {
    ULL curSum = 0;
    bool all_empty = true;
    bool first_element = true;

    auto current_ptr = ptr_list.begin();
    auto current_end = ptr_end_list.begin();

    while (current_ptr != ptr_list.end()) {
      if (*current_ptr != *current_end) {
        all_empty = false;
        out << (first_element ? "" : " ") << **current_ptr;
        first_element = false;
        try {
          curSum = addition(curSum, **current_ptr);
        } catch (...) {
          out << "\n";
          throw;
        }

        ++(*current_ptr);
      }
      ++current_ptr;
      ++current_end;
    }

    if (all_empty) {
      break;
    }
    if (i < maxLen - 1) {
      out << "\n";
    }

    sums_tail = sums.insert(sums_tail, curSum);
  }

  return sums;
}
