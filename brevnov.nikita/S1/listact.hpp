#ifndef ACTIONSLIST_HPP
#define ACTIONSLIST_HPP
#include <iostream>
#include "list.hpp"

namespace brevnov
{
  using numbers = List< unsigned long long >;
  using pair = std::pair< std::string, numbers >;
  using list = List< pair >;
  list createList(std::istream& input);
  numbers calculateSum(const list& list);
  std::ostream& outputNumber(std::ostream& output, const numbers& list);
  std::ostream& outputName(std::ostream& output, const list& list);
  std::ostream& outputNumbers(std::ostream& output, const list& list);
}
#endif
