#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <ostream>
#include <map>

namespace lebedev
{
  using dictOfDicts = std::map< std::string, std::map< int, std::string > >;
  void printCMD(std::ostream& out, const dictOfDicts& dict);
  void complementCMD(dictOfDicts& dict);
  void intersectCMD(dictOfDicts& dict);
  void unionCMD(dictOfDicts& dict);
}
#endif
