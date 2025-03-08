#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_ALGORITHMS_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_ALGORITHMS_HPP

namespace kizhin {
  template < typename InputIt >
  bool lexicographicalCompare(InputIt first1, InputIt last1, InputIt first2,
      InputIt last2)
  {
    for (; (first1 != last1) && (first2 != last2); ++first1, ++first2) {
      if (*first1 < *first2) {
        return true;
      }
      if (*first2 < *first1) {
        return false;
      }
    }
    return (first1 == last1) && (first2 != last2);
  }
}

#endif

