#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S5_ARGS_PARSER_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S5_ARGS_PARSER_HPP

#include <string>

namespace kizhin {
  struct Args
  {
    std::string filename;
    enum TraverseT {
      ascending,
      descending,
      breadth,
    } traverse;
  };
  Args parseArgs(int, const char* const*, std::ostream&);
}

#endif

