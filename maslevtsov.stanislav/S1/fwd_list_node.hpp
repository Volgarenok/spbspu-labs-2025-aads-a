#ifndef FWD_LIST_NODE_HPP
#define FWD_LIST_NODE_HPP

namespace maslevtsov {
  template< typename T >
  class FwdListNode
  {
  public:
    T data_;
    FwdListNode< T >* next_;
  };
}

#endif
