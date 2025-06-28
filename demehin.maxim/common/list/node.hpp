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
    Node(const T&, Node< T >*, Node< T >*) noexcept;
  };

  template< typename T >
  Node< T >::Node(const T& value) noexcept:
    data(value),
    prev(nullptr),
    next(nullptr)
  {}

  template< typename T >
  Node< T >::Node(const T& value, Node< T >* prev_ptr, Node< T >* next_ptr) noexcept:
    data(value),
    prev(prev_ptr),
    next(next_ptr)
  {}
}

#endif
