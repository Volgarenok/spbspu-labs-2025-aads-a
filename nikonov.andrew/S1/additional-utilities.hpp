#ifndef ADDITIONAL_UTILITIES_HPP
#define ADDITIONAL_UTILITIES_HPP
#include <istream>
#include <string>
#include <list>
namespace nikonov
{
  std::list< std::pair< std::string, std::list< size_t > > > * getPList(std::istream& input, bool& overflowFlag);
  void processPList(std::list< std::pair< std::string, std::list< size_t > > > * pList, bool& overflowFlag);
}
#endif
