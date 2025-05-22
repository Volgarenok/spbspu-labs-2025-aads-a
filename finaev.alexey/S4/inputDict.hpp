#ifndef INPUTOUTDICT_HPP
#define INPUTOUTDICT_HPP
#include <istream>
#include <map>
#include <string>



namespace finaev
{
  using mainDict = std::map < std::string, std::map< int, std::string > >;
  void inDictionaries(std::istream& in, mainDict& dict);
}

#endif
