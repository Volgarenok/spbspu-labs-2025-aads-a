#ifndef NODE_HPP
#define NODE_HPP

namespace demehin
{
  template< typename T >
  struct Node
  {
    T data;
    Node< T >* prev;
    Node< T >* next;

    explicit Node(const T&) noexcept;
  };

  template< typename T >
  Node< T >::Node(const T& value) noexcept:
    data(value),
    prev(nullptr),
    next(nullptr)
  {}
}

#endif
