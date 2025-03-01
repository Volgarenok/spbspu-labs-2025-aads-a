#ifndef LIST_ACTIONS_HPP
#define LIST_ACTIONS_HPP
#include <iostream>
#include <list>

using pair_t = std::pair< std::string, std::list< int > >;

namespace evstyunichev
{
  std::list< pair_t > *form_list(std::istream &);
  int list_out(std::list< pair_t > *, std::ostream &);
}
#endif