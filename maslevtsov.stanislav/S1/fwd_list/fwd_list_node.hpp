#ifndef FWD_LIST_NODE_HPP
#define FWD_LIST_NODE_HPP

namespace maslevtsov {
  template< typename T >
  struct FwdListNode
  {
    T data_;
    FwdListNode< T >* next_;
  };
}

#endif
