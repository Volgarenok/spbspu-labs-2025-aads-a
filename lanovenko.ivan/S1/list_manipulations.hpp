#ifndef LIST_MANIPULATIONS_HPP
#define LIST_MANIPULATIONS_HPP

#include <list>
#include <string>
#include <ostream>
#include <limits>

namespace lanovenko
{
  using list_ull = std::list<unsigned long long>;
  using list_of_pairs = std::list< std::pair < std::string, list_ull>>;
  void printSequencesName(const list_of_pairs temporary, std::ostream& out);
  bool isSumLimit(size_t a, size_t b);
  void printSumList(const std::list<int>& sumList, bool sumLimit, std::ostream& out);
  void printSequences(const list_of_pairs temporary, size_t maxSize, bool& sumLimit, std::ostream& out);
}

#endif
