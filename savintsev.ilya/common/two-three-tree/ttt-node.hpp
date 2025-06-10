#ifndef TTT_NODE_H
#define TTT_NODE_H
#include <cstddef>
#include <utility>
#include <vector>

namespace savintsev
{
  template< typename T >
  struct node_t
  {
    T data_[3];

    node_t * kids_[4] = {nullptr, nullptr, nullptr, nullptr};
    node_t * parent_ = nullptr;

    size_t len_ = 0;
  };
}

#endif
