#ifndef ACTIONSWITHLIST_HPP
#define ACTIONSWITHLIST_HPP
#include <ostream>
#include <string>
#include "list.hpp"

namespace lebedev
{
  void printList(const List< std::string >& list, std::ostream& output);
  void printList(const List< size_t >& list, std::ostream& output);
  List< List< size_t > > createReorderedList(const List< std::pair< std::string, List< size_t > > >& List);
  std::pair< List< size_t >, bool > createListOfSum(const List< List< size_t > >& list);
}
#endif
