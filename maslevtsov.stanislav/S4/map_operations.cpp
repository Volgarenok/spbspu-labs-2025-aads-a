#include "map_operations.hpp"
#include <iostream>
#include <tree/definition.hpp>

void maslevtsov::input_setmap(std::istream& in, setmap_t& map)
{
  std::string setname, value;
  int key = 0;
  while (in) {
    set_t set;
    in >> setname;
    while (in && in.peek() != '\n') {
      in >> key >> value;
      set.insert(std::make_pair(key, value));
    }
    map.insert(std::make_pair(setname, set));
  }
}

void maslevtsov::print_set(std::istream& in, const setmap_t& map)
{
  std::string setname;
  in >> setname;
  set_t set;
  set = map.at(setname);
  if (set.empty()) {
    std::cout << "<EMPTY>\n";
    return;
  }
  std::cout << setname;
  for (auto it = set.cbegin(); it != set.cend(); ++it) {
    std::cout << ' ' << it->first << ' ' << it->second;
  }
  std::cout << '\n';
}

void maslevtsov::complement_sets(std::istream& in, setmap_t& map)
{
  std::string newname, setname1, setname2;
  in >> newname >> setname1 >> setname2;
  set_t set1 = map.at(setname1), set2 = map.at(setname2);
  set_t newset;
  for (auto it = set1.cbegin(); it != set1.cend(); ++it) {
    if (set2.find(it->first) == set2.end()) {
      newset.insert(std::make_pair(it->first, it->second));
    }
  }
  map[newname] = newset;
}

void maslevtsov::intersect_sets(std::istream& in, setmap_t& map)
{
  std::string newname, setname1, setname2;
  in >> newname >> setname1 >> setname2;
  set_t set1 = map.at(setname1), set2 = map.at(setname2);
  set_t newset;
  for (auto it = set1.cbegin(); it != set1.cend(); ++it) {
    if (set2.find(it->first) != set2.end()) {
      newset.insert(std::make_pair(it->first, it->second));
    }
  }
  map[newname] = newset;
}

void maslevtsov::union_sets(std::istream& in, setmap_t& map)
{
  std::string newname, setname1, setname2;
  in >> newname >> setname1 >> setname2;
  set_t set1 = map.at(setname1), set2 = map.at(setname2);
  set_t newset;
  for (auto it = set1.cbegin(); it != set1.cend(); ++it) {
    newset.insert(std::make_pair(it->first, it->second));
  }
  for (auto it = set2.cbegin(); it != set2.cend(); ++it) {
    newset.insert(std::make_pair(it->first, it->second));
  }
  map[newname] = newset;
}
