#ifndef LIST_ACTIONS_HPP
#define LIST_ACTIONS_HPP
#include <iostream>
#include <list>

using pair_t = std::pair< std::string, std::list< size_t> >;

namespace evstyunichev
{
  std::list< pair_t > *form_list(std::istream &);
  std::ostream &list_out(std::list< pair_t > *, std::ostream &);
}
#endif
