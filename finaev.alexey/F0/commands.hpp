#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <AVLtree.hpp>
#include <hashTable.hpp>
#include <string>
#include <functional>
#include "openningInfo.hpp"

namespace finaev
{
  using globalDebuts = HashTable< std::string, finaev::OpenningInfo >;
  using debutsBases = HashTable< std::string, HashTable< std::string, bool > >;
  using mapOfCommands = AVLtree< std::string, std::function< void() > >;

  void loadDebutsFromFile(const std::string& filename, globalDebuts& debuts, std::ostream& out);

  void create_debut(std::istream&, std::ostream&, globalDebuts&);
  void create_base(std::istream&, std::ostream&, debutsBases&);
  void add(std::istream&, std::ostream&, const globalDebuts&, debutsBases&);
  void exact_find(std::istream&, std::ostream&, const globalDebuts&, const debutsBases&);
  void find(std::istream&, std::ostream&, const globalDebuts&, const debutsBases&);
  void print(std::istream&, std::ostream&, const globalDebuts&, const debutsBases&);
  void move(std::istream&, std::ostream&, const globalDebuts&, debutsBases&);
  void merge(std::istream&, std::ostream&, const globalDebuts&, debutsBases&);
  void intersect(std::istream&, std::ostream&, const globalDebuts&, debutsBases&);
  void complement(std::istream&, std::ostream&, const globalDebuts&, debutsBases&);
  void remove(std::istream&, std::ostream&, const globalDebuts&, debutsBases&);
  void force_delete(std::istream&, std::ostream&, globalDebuts&, debutsBases&);
  void safe_delete(std::istream&, std::ostream&, globalDebuts&, const debutsBases&);
  void delete_debut(std::istream&, std::ostream&, globalDebuts&, debutsBases&);
  void validate(std::istream&, std::ostream&, const globalDebuts&);
  void guess(std::istream&, std::ostream&, const globalDebuts&, const debutsBases&);

  mapOfCommands createCommandsHandler(std::istream& in, std::ostream& out, globalDebuts&, debutsBases&);
}

#endif
