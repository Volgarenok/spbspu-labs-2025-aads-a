#ifndef MAP_OPERATIONS_HPP
#define MAP_OPERATIONS_HPP

#include <string>
#include <functional>
#include <tree/declaration.hpp>

namespace maslevtsov {
  using set_t = maslevtsov::Tree< int, std::string >;
  using setmap_t = maslevtsov::Tree< std::string, set_t >;
  using commands_t = maslevtsov::Tree< std::string, std::function< void(std::istream&, setmap_t&) > >;

  void input_setmap(std::istream& in, setmap_t& map);

  void print_set(std::istream& in, const setmap_t& map);
  void complement_sets(std::istream& in, setmap_t& map);
  void intersect_sets(std::istream& in, setmap_t& map);
  void union_sets(std::istream& in, setmap_t& map);
}

#endif
