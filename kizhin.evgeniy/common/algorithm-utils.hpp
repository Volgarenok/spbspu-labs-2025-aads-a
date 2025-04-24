#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_ALGORITHMS_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_ALGORITHMS_HPP

#include <functional>
#include <iterator>

namespace kizhin {

  namespace detail {
    template < typename Iterator >
    using IteratorValueType = typename std::iterator_traits< Iterator >::value_type;

    template < typename Iterator >
    using DefaultComparator = std::less< IteratorValueType< Iterator > >;
  }

  template < typename InputIt, typename Comp = detail::DefaultComparator< InputIt > >
  bool lexicographicalCompare(InputIt first1, InputIt last1, InputIt first2,
      InputIt last2, Comp comp = Comp{})
  {
    for (; (first1 != last1) && (first2 != last2); ++first1, ++first2) {
      if (comp(*first1, *first2)) {
        return true;
      }
      if (comp(*first2, *first1)) {
        return false;
      }
    }
    return (first1 == last1) && (first2 != last2);
  }
}

#endif

