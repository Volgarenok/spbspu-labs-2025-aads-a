#ifndef TTT_NODE_H
#define TTT_NODE_H
#include <cstddef>

namespace savintsev
{
  template< typename T >
  struct node_t
  {
    T data[3];

    node_t * kids[4] = {nullptr, nullptr, nullptr, nullptr};
    node_t * father = nullptr;

    size_t len = 0;
  };
}

#endif
