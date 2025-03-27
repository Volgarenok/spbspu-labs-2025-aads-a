#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S1_TEST_UTILS_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S1_TEST_UTILS_HPP

#include <boost/test/unit_test.hpp>
#include <buffer.hpp>

namespace kizhin {
  using BufferT = Buffer< double >;
  void testBufferInvariants(const BufferT&);
  void testBufferComparison(const BufferT&, const BufferT&);
}

#endif

