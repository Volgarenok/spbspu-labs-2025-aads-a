#ifndef UTILS_HPP
#define UTILS_HPP
#include <string>

namespace smirnov
{
  bool isOperator(const std::string & token);
  int getPriority(const std::string & op);
}
#endif
