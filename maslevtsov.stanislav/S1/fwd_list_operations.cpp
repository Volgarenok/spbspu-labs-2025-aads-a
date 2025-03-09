#include "fwd_list_operations.hpp"
#include <limits>
#include <ostream>

namespace {
  int checked_sum(unsigned long long a, unsigned long long b)
  {
    const unsigned long long max_ull = std::numeric_limits< unsigned long long >::max();
    if (max_ull - a > b) {
      return a + b;
    }
    throw std::overflow_error("overflow");
  }

  void print_list_elements(std::ostream& out, const maslevtsov::list_t& list) noexcept
  {
    if (!list.empty()) {
      out << *list.begin();
      for (auto j = ++list.begin(); j != list.end(); ++j) {
        out << ' ' << *j;
      }
    }
  }
}

std::size_t maslevtsov::get_max_pairs_list_size(const pairs_list_t& list) noexcept
{
  std::size_t maximum = 0;
  for (auto i = list.begin(); i != list.end(); ++i) {
    maximum = std::max(maximum, i->second.size());
  }
  return maximum;
}

void maslevtsov::get_lists_sums(list_t& sums, const pairs_list_t& pairs_list)
{
  std::size_t max_pairs_list_size = get_max_pairs_list_size(pairs_list);
  for (std::size_t i = 0; i != max_pairs_list_size; ++i) {
    std::size_t cur_sum = 0;
    for (auto j = pairs_list.begin(); j != pairs_list.end(); ++j) {
      list_t cur_list = j->second;
      if (cur_list.size() > i) {
        auto it = cur_list.begin();
        for (std::size_t k = 0; k != i; ++k, ++it);
        cur_sum = checked_sum(cur_sum, *it);
      }
    }
    sums.push_back(cur_sum);
  }
}

void maslevtsov::print_lists_info(std::ostream& out, const pairs_list_t& pairs_list)
{
  std::size_t max_pairs_list_size = get_max_pairs_list_size(pairs_list);
  for (std::size_t i = 0; i != max_pairs_list_size; ++i) {
    list_t column_elements;
    for (auto j = pairs_list.begin(); j != pairs_list.end(); ++j) {
      list_t cur_list = j->second;
      if (cur_list.size() > i) {
        auto it = cur_list.begin();
        for (std::size_t k = 0; k != i; ++k, ++it);
        column_elements.push_back(*it);
      }
    }
    if (!column_elements.empty()) {
      print_list_elements(out, column_elements);
      out << '\n';
    }
  }
  list_t sums;
  get_lists_sums(sums, pairs_list);
  if (sums.empty()) {
    out << 0;
    return;
  }
  print_list_elements(out, sums);
}
