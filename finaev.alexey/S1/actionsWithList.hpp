#ifndef ACTIONSWITHLIST
#define ACTIONSWITHLIST
#include <iosfwd>
#include <string>
#include "list.hpp"

namespace finaev
{
  finaev::List < finaev::List < size_t > > createRightList(finaev::List< std::pair< std::string, finaev::List < size_t > > > List);
  void printList(finaev::List<size_t> list, std::ostream& out);
  void printList(finaev::List<std::string> list, std::ostream& out);
  bool isOverflow(finaev::List < finaev::List < size_t > > list);
  finaev::List < size_t > createListOfSum(finaev::List < finaev::List < size_t > > list);
}

#endif
