#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <ostream>
#include <map>

namespace finaev
{
  using mainDict = std::map < std::string, std::map< int, std::string > >;
  void printCMD(std::ostream& out, const mainDict& dict);
  void complementCMD(mainDict& dict);
  void intersectCMD(mainDict& dict);
  void unionCMD(mainDict& dict);
}

#endif
