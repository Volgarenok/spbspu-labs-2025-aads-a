#ifndef ACTIONSWITHLIST
#define ACTIONSWITHLIST
#include <iosfwd>
#include <string>
#include "list.hpp"

namespace finaev
{
  List< List< size_t > > createRightList(List< std::pair< std::string, List< size_t > > > List);
  void printList(List< size_t > list, std::ostream& out);
  void printList(const List< std::string >& list, std::ostream& out);
  bool isOverflow(List< List< size_t > > list);
  List< size_t > createListOfSum(List< List< size_t > > list);
}

#endif
