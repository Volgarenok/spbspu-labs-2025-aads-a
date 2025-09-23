#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <ostream>
#include <AVLtree.hpp>

namespace finaev
{
  using mainDict = finaev::AVLtree< std::string, finaev::AVLtree< int, std::string > >;
  void printCMD(std::ostream& out, const mainDict& dict);
  void complementCMD(mainDict& dict);
  void intersectCMD(mainDict& dict);
  void unionCMD(mainDict& dict);
}

#endif
