#ifndef PROJECT_BODY_HPP
#define PROJECT_BODY_HPP
#include <string>
#include <list.hpp>
#include <shape.hpp>
#include <two-three-tree.h>

namespace savintsev
{
  using Layer = std::pair< std::string, Shape * >;
  using Project = List< Layer >;
  using Projects = TwoThreeTree< std::string, Project >;
}

#endif
