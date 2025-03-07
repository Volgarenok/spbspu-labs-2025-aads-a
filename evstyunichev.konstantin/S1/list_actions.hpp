#ifndef LIST_ACTIONS_HPP
#define LIST_ACTIONS_HPP
#include <iostream>
#include "../common/list.hpp"

using pair_t = std::pair< std::string, evstyunichev::List< size_t> >;

namespace evstyunichev
{
  evstyunichev::List< pair_t > *form_list(std::istream &);
  std::ostream &list_out(evstyunichev::List< pair_t > *, std::ostream &);
}
#endif
