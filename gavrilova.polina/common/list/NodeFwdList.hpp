#ifndef NODE_HPP
#define NODE_HPP

namespace gavrilova {
  template < class T >
  struct NodeFwdList {
    T data;
    NodeFwdList< T >* next;
  };
}

#endif
