#ifndef LIST_MANIPULATIONS_HPP
#define LIST_MANIPULATIONS_HPP

#include <list>
#include <string>
#include <ostream>
#include <limits>
#include "forward_list.hpp"

namespace lanovenko
{
  using list_ull = ForwardList< unsigned long long >;
  using list_of_pairs = ForwardList< std::pair < std::string, list_ull > >;
  void printSequencesName(const list_of_pairs& temporary, std::ostream& out);
  bool isSumLimit(size_t a, size_t b);
  void printSumList(const ForwardList< int >& sumList, bool sumLimit, std::ostream& out);
  void printSequences(const list_of_pairs& temporary, size_t maxSize, bool& sumLimit, std::ostream& out);
  void printRowsLists(const ForwardList< list_ull >& rowsLists, std::ostream& out);
}

#endif
