#ifndef LISTNODE_HPP
#define LISTNODE_HPP

namespace duhanina
{
  template < typename T >
  struct NodeList
  {
    T data_;
    NodeList* next_;
  };
}

#endif
