#ifndef IO_POCESSING_HPP
#define IO_POCESSING_HPP
#include <string>
#include <istream>
#include "stack.hpp"
namespace nikonov
{
  Stack< std::string > getInput(std::istream & in);
  std::string convertToPostfix(std::string & infix);
}
#endif
