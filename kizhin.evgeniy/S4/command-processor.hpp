#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S4_COMMAND_PROCESSOR_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S4_COMMAND_PROCESSOR_HPP

#include <iosfwd>
#include "dataset.hpp"

namespace kizhin {
  void processCommands(DSContainer&, std::istream&, std::ostream&);
}

#endif

