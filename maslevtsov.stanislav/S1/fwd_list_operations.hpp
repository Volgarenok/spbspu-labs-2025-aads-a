#ifndef FWD_LIST_OPERATIONS_HPP
#define FWD_LIST_OPERATIONS_HPP

#include <cstddef>
#include <list>
#include <string>
#include "fwd_list.hpp"

namespace maslevtsov {
  using list_t = std::list< unsigned long long >;
  using pairs_list_t = std::list< std::pair< std::string, list_t > >;

  std::size_t get_max_pairs_list_size(const pairs_list_t& list) noexcept;
  void get_lists_sums(list_t& sums, const pairs_list_t& pairs_list);
  void print_lists_info(std::ostream& out, const pairs_list_t& pairs_list);
}

#endif
