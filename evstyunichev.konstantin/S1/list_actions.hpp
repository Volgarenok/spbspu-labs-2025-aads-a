#ifndef LIST_ACTIONS_HPP
#define LIST_ACTIONS_HPP
#include <iostream>
#include "../common/list.hpp"

namespace evstyunichev
{
  using pair_t = std::pair< std::string, List< size_t > >;
  std::ostream &list_out(const List< pair_t > &seqences, std::ostream &out);
}
#endif
