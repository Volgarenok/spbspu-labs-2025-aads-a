#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <ostream>
#include <functional>
#include "AVLtree.hpp"

namespace sharifullina
{
  using mainDict = sharifullina::AVLtree< std::string, sharifullina::AVLtree< int, std::string > >;

  void printCMD(std::ostream& out, const mainDict& dict);
  void complementCMD(mainDict& dict);
  void intersectCMD(mainDict& dict);
  void unionCMD(mainDict& dict);
}

#endif
