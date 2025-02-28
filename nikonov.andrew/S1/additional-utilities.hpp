#ifndef ADDITIONAL_UTILITIES_HPP
#define ADDITIONAL_UTILITIES_HPP
#include <istream>
#include <string>
#include <list>
namespace nikonov
{
  std::list< std::pair< std::string, std::list< int > > > * getPList(std::istream& input);
  void processPList(std::list< std::pair< std::string, std::list< int > > > * pList);
  std::list< int >::iterator getIterAt(std::list< int > & list, size_t id);
}
#endif
