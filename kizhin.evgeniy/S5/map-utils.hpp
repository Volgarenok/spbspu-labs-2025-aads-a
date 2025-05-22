#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S5_MAP_UTILS_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S5_MAP_UTILS_HPP

#include <iosfwd>
#include <map.hpp>

namespace kizhin {
  using MapT = Map< long long, std::string >;
  struct ValueCollector;
  std::istream& operator>>(std::istream&, MapT&);
}

struct kizhin::ValueCollector
{
  void operator()(MapT::const_reference);
  MapT::key_type keys = 0;
  MapT::mapped_type values;
};

#endif

