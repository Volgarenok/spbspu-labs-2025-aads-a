#ifndef DICT_INPUT_OUTPUT_HPP
#define DICT_INPUT_OUTPUT_HPP
#include <iostream>
#include <list/list.hpp>
#include <tree/tree-2-3.hpp>

namespace alymova
{
  using Dictionary = TwoThreeTree< std::string, List< std::string >, std::less< std::string > >;

  std::istream& operator>>(std::istream& in, List< std::string >& list);
  std::ostream& operator<<(std::ostream& out, const List< std::string >& list);
  std::ostream& operator<<(std::ostream& out, const Dictionary& dict);
}
#endif
