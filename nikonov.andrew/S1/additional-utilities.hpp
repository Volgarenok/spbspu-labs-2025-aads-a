#ifndef ADDITIONAL_UTILITIES_HPP
#define ADDITIONAL_UTILITIES_HPP
#include <istream>
#include <string>
#include "List.hpp"
namespace nikonov
{
  nikonov::List< std::pair< std::string, List< size_t > > > * getPList(std::istream& input, bool& overflowFlag);
  void processPList(List< std::pair< std::string, List< size_t > > > * pList, bool& overflowFlag);
}
#endif
