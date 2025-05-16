#ifndef COMMAND_PROCESSING_HPP
#define COMMAND_PROCESSING_HPP

#include "input_output_dictionaries.hpp"

namespace lanovenko
{
  enum class CommandType { PRINT, COMPLEMENT, INTERSECT, UNION, INVALID };
  void merge(map_t& dictionaries);
  void complement(map_t& dictionaries);
  void intersect(map_t& dictionaries);
  CommandType parseCommand(const std::string& cmd);
}

#endif
