#ifndef LIST_UTILS_HPP
#define LIST_UTILS_HPP

#include <ios>
#include "list.hpp"

namespace aleksandrov
{
  unsigned long long calcSum(const List< unsigned long long >&);
  unsigned long long calcSum(unsigned long long, unsigned long long);
  void outputList(const List< unsigned long long >&, std::ostream&);

  using PairsList = List< std::pair< std::string, List< unsigned long long > > >;

  void getPairsList(std::istream&, PairsList&);
  size_t calcMaxSubListSize(const PairsList&);
  void getTransposedList(const PairsList&, List< List< unsigned long long > >&);

  template< class InputIt1, class InputIt2 >
  bool lexicographicalCompare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
  {
    for (; (first1 != last1) && (first2 != last2); ++first1, ++first2)
    {
      if (*first1 < *first2)
      {
        return true;
      }
      if (*first2 < *first1)
      {
        return false;
      }
    }
    return (first1 == last1) && (first2 != last2);
  }
}

#endif

