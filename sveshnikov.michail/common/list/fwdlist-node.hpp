#ifndef FWDLIST_NODE_HPP
#define FWDLIST_NODE_HPP

namespace sveshnikov
{
  template< typename T >
  struct node_t
  {
    T data_;
    node_t< T > *next_;
  };
}

#endif
