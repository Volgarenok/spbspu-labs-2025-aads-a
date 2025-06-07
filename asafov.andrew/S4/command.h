#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include <map.hpp>
namespace asafov
{
  using map_t = asafov::map<int, std::string>;
  using maps_t = asafov::map<std::string, map_t>;

  void printMap(const maps_t& maps, const std::string& name);
  bool complement(maps_t& maps, const std::string& newName, const std::string& left, const std::string& right);
  bool intersect(maps_t& maps, const std::string& newName, const std::string& left, const std::string& right);
  bool unionMaps(maps_t& maps, const std::string& newName, const std::string& left, const std::string& right);
  bool processCommand(maps_t& maps, const std::string& line);
}
#endif
