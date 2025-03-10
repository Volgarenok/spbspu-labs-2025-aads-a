#ifndef ACTIONSWITHLIST
#define ACTIONSWITHLIST
#include <list>
#include <iosfwd>
#include <string>

namespace finaev
{
  std::list < std::list < size_t > > createRightList(std::list< std::pair< std::string, std::list < size_t > > >);
  void printList(std::list<size_t> list, std::ostream& out);
  void printList(std::list<std::string> list, std::ostream& out);
  bool isOverflow(std::list < std::list < size_t > > list);
  std::list < size_t > createListOfSum(std::list < std::list < size_t > > list);
}

#endif
