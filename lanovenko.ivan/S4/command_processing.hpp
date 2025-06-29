#ifndef COMMAND_PROCESSING_HPP
#define COMMAND_PROCESSING_HPP

#include <functional>
#include "input_output_dictionaries.hpp"

namespace lanovenko
{
  using commands_t = Tree< std::string, std::function< void (map_t&) >, std::less< std::string > >;
  void merge(map_t& dictionaries);
  void complement(map_t& dictionaries);
  void intersect(map_t& dictionaries);
}

#endif
