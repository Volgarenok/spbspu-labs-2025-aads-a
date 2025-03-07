#ifndef IO_CONTAINERS_HPP
#define IO_CONTAINERS_HPP
#include <forward_list>
#include <cstring>
#include <utility>
#include <iostream>

using pair_strlist = std::pair< std::string, std::forward_list< unsigned long long > >;
using list_ull = std::forward_list< unsigned long long >;
using list_pair = std::forward_list< std::pair< std::string, std::forward_list< unsigned long long> > >;
using list_iter = std::forward_list< list_ull::iterator >;

namespace zakirov
{
  void get_list_pair(std::istream & in, list_pair & forward_list);
  void get_list_ull(std::istream & in, list_ull & forward_list);
  void output_result(std::ostream & out, list_pair & forward_list);
}

#endif
