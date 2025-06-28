#ifndef UTILS_HPP
#define UTILS_HPP
#include <string>
#include <list/list.hpp>

namespace smirnov
{
  bool isOperator(const std::string & token);
  bool comparePriority(const std::string & op1, const std::string & op2);
  List< std::string > splitBySpace(const std::string & line);
}
#endif
