#ifndef SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_F0_STATE_HPP
#define SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_F0_STATE_HPP

#include <iosfwd>
#include <string>
#include <vector>
#include <map.hpp>

namespace kizhin {
  using State = Map< std::string, std::vector< std::string > >;
  std::string getStateFile();
  void saveState(std::ostream&, const State&);
  State loadState(std::istream&);
}

#endif

