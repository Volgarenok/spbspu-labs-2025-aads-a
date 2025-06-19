#ifndef READ_EXPRESSIONS_HPP
#define READ_EXPRESSIONS_HPP
#include <string>
#include <list/list.hpp>

namespace smirnov
{
  List< std::string > readExpressions(const std::string & filename);
  void processInput(std::istream & input, List< std::string > & expressions);
}
#endif
