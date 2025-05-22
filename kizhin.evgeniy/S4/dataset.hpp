#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S4_DATASET_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S4_DATASET_HPP

#include <map.hpp>
#include <string>

namespace kizhin {
  using Dataset = Map< int, std::string >;
  using DSContainer = Map< std::string, Dataset >;
}

#endif

