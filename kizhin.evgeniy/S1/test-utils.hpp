#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S1_TEST_UTILS_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S1_TEST_UTILS_HPP

#include <boost/test/unit_test.hpp>
#include <forward-list.hpp>

namespace kizhin {
  using ListT = ForwardList< double >;
  void testForwardListInvariants(const ListT&);
  void testComparisonInvariants(const ListT&, const ListT&);
}

#endif

