#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <ostream>
#include <AVLtree.hpp>

namespace lebedev
{
  using dictOfDicts = lebedev::AVLtree< std::string, lebedev::AVLtree< int, std::string > >;
  void printCMD(std::ostream& out, const dictOfDicts& dict);
  void complementCMD(dictOfDicts& dict);
  void intersectCMD(dictOfDicts& dict);
  void unionCMD(dictOfDicts& dict);
}
#endif
