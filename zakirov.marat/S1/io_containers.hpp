#ifndef IO_CONTAINERS_HPP
#define IO_CONTAINERS_HPP
#include <forward_list>
#include <cstring>
#include <utility>
#include <iostream>
#include "fwd_list.hpp"
#include "fwd_iterator.hpp"

using list_ull = zakirov::FwdList< unsigned long long >;
using list_ull_it = zakirov::FwdIterator< unsigned long long >;
using it_to_it = zakirov::FwdIterator<list_ull_it>;
using pair_strlist = std::pair< std::string, list_ull >;
using list_pair = zakirov::FwdList< pair_strlist >;
using list_pair_it = zakirov::FwdIterator< pair_strlist >;
using list_iter = zakirov::FwdList< list_ull_it >;
using list_iter_it = zakirov::FwdIterator< zakirov::FwdIterator< list_ull > >;

namespace zakirov
{
  void get_list_pair(std::istream & in, list_pair & forward_list);
  void get_list_ull(std::istream & in, list_ull & forward_list);
  void output_result(std::ostream & out, list_pair & forward_list);
}

#endif
