#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S4_TEST_UTILS_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S4_TEST_UTILS_HPP

#include "map.hpp"

namespace kizhin {
  using MapT = Map< int, std::string >;
  void testMapInvariants(const MapT&);
  void testMapComparison(const MapT&, const MapT&);
}

#endif

