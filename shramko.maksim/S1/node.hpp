#ifndef NODE_HPP
#define NODE_HPP

namespace shramko
{
  template< typename T >
  class FwdListNode
  {
  public:
    T data;
    FwdListNode< T >* next;
  };
}


#endif