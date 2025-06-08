#ifndef NODE_HPP
#define NODE_HPP

namespace aleksandrov
{
  namespace detail
  {
    template< typename T>
    struct Node
    {
      T data;
      Node< T >* next;

      Node(const T& data):
        data(data),
        next(nullptr)
      {}
    };
  }
}

#endif

