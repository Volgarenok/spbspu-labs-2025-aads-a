#ifndef FWD_LIST_OPERATIONS_HPP
#define FWD_LIST_OPERATIONS_HPP

#include <cstddef>
#include <string>
#include "fwd_list/declaration.hpp"

namespace maslevtsov {
  using list_t = maslevtsov::FwdList< unsigned long long >;
  using pairs_list_t = maslevtsov::FwdList< std::pair< std::string, list_t > >;

  std::size_t get_max_pairs_list_size(const pairs_list_t& list) noexcept;
  void get_lists_sums(list_t& sums, const pairs_list_t& pairs_list);
  void print_lists_info(std::ostream& out, const pairs_list_t& pairs_list);
}

#endif
