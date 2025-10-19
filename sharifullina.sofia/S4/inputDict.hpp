#ifndef INPUTOUTDICT_HPP
#define INPUTOUTDICT_HPP

#include <istream>
#include <string>
#include "AVLtree.hpp"

namespace sharifullina
{
  using mainDict = sharifullina::AVLtree< std::string, sharifullina::AVLtree< int, std::string > >;

  void inDictionaries(std::istream& in, mainDict& dict);
}

#endif
