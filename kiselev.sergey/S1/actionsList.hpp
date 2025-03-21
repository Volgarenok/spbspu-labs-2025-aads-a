#ifndef ACTIONSLIST_HPP
#define ACTIONSLIST_HPP
#include <iostream>
#include "list.hpp"

namespace kiselev
{
  using numberList = List< unsigned long long >;
  using pair = std::pair< std::string, numberList >;
  using list = List< pair >;
  list createList(std::istream& input);
  numberList calculateSum(const list& list);
  std::ostream& outputNumber(std::ostream& output, const numberList& list);
  std::ostream& outputName(std::ostream& output, const list& list);
  std::ostream& outputNumbers(std::ostream& output, const list& list);
}
#endif
