#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S7_COMMAND_PROCESSOR_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S7_COMMAND_PROCESSOR_HPP

#include <iosfwd>
#include <map>
#include "graph.hpp"

namespace kizhin {
  using GraphContainer = std::map< std::string, Graph >;
  void processCommands(GraphContainer&, std::istream&, std::ostream&);
}

#endif

