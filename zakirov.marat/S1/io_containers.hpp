#ifndef IO_CONTAINERS_HPP
#define IO_CONTAINERS_HPP
#include <forward_list>
#include <cstring>
#include <utility>
#include <iostream>
#include "fwd_list.hpp"
#include "fwd_iterator.hpp"

namespace zakirov
{
  using list_ull = FwdList< unsigned long long >;
  using pair_strlist = std::pair< std::string, list_ull >;
  using list_pair = FwdList< pair_strlist >;
  using list_iter = FwdList< FwdIterator< unsigned long long > >;

  void get_list_pair(std::istream & in, list_pair & forward_list);
  void get_list_ull(std::istream & in, list_ull & forward_list);
  void output_result(std::ostream & out, list_pair & forward_list);
}

#endif
