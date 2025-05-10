#ifndef READ_EXPRESSIONS_HPP
#define READ_EXPRESSIONS_HPP
#include <string>
#include <list>

namespace smirnov
{
  std::list< std::string > readExpressions(const std::string & filename);
  void processInput(std::istream & input, std::list< std::string > & expressions);
}
#endif
