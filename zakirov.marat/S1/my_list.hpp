#ifndef MY_LIST_HPP
#define MY_LIST_HPP
#include "my_list_node.hpp"

namespace zakirov
{
  template< typename T >
  class List
  {
  public:
    List();
    ~List();
  private:
    MyListNode< T > fake_node_;
    MyListNode< T > head_node_;
  };
}

#endif
