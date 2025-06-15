#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S1_TYPE_UTILS_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S1_TYPE_UTILS_HPP

#include <cstdint>
#include <iosfwd>
#include <forward-list.hpp>

namespace kizhin {
  using NumbersT = ForwardList< std::uint64_t >;
  using NamesT = ForwardList< std::string >;
}

#endif

