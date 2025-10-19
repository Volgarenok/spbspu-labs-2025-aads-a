#ifndef INPUTUTIL_HPP
#define INPUTUTIL_HPP

#include <istream>
#include <map>
#include "graph.hpp"

namespace kushekbaev
{
  void inputGraphs(std::istream& in, std::map< std::string, Graph >& graphs);
}

#endif
