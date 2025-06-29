#ifndef OPENNINGINFO
#define OPENNINGINFO
#include <dynamicArr.hpp>
#include <string>

namespace finaev
{
  struct DebutMoves
  {
    DynamicArr< std::string > moves;
    bool containsSequence(const DynamicArr< std::string >& sequence) const;
  };

  std::istream& operator>>(std::istream&, DebutMoves&);

  struct OpenningInfo
  {
    DebutMoves moves;
    std::string name;
    std::string description;
  };
}

#endif
