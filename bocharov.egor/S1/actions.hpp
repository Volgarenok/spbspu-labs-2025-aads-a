#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include <istream>
#include <ostream>
#include <cstddef>
#include <string>
#include "list.hpp"

namespace bocharov
{
  using list_t = List< unsigned long long >;
  using pair = std::pair< std::string, list_t >;
  using pairs_list_t = List< pair >;

  std::size_t get_max_pairs_list_size(const pairs_list_t & list) noexcept;
  void get_lists_sums(list_t & sums, const pairs_list_t & pairs_list);
  void print_lists_info(std::ostream & out, const pairs_list_t & pairs_list);
  pairs_list_t createList(std::istream & input);
  std::ostream & outputList(std::ostream & output, const pairs_list_t & list);
}

#endif
