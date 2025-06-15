#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S4_IO_UTILS_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S4_IO_UTILS_HPP

#include <iosfwd>
#include "dataset.hpp"

namespace kizhin {
  std::istream& operator>>(std::istream&, DSContainer&);
  std::ostream& operator<<(std::ostream&, DSContainer::const_reference);
}

#endif

