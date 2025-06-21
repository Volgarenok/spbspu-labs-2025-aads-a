#ifndef INPUTOUTDICT_HPP
#define INPUTOUTDICT_HPP
#include <istream>
#include <string>
#include <AVLtree.hpp>

namespace finaev
{
  using mainDict = finaev::AVLtree< std::string, finaev::AVLtree< int, std::string > >;
  void inDictionaries(std::istream& in, mainDict& dict);
}

#endif
