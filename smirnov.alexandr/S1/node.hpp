#ifndef NODE_HPP
#define NODE_HPP

namespace smirnov
{
  template< typename T >
  struct Node
  {
    Node * next;
    char data[sizeof(T)];
    Node()
    {
      next = nullptr;
    }
    ~Node()
    {
      if (next != this)
      {
        static_cast< T* >(static_cast< void* >(data))->~T();
      }
    }
  }; 
}
#endif
