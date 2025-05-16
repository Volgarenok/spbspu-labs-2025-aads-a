#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S4_ARGS_PARSER_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S4_ARGS_PARSER_HPP

#include <iosfwd>
#include <string>

namespace kizhin {
  struct Args
  {
    std::string filename;
  };
  Args parseArgs(int, const char* const*, std::ostream&);
}

#endif

