#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace ivanova
{
  template < typename T >
  struct ListNode
  {
    using value_type = T;

    ListNode* next;
    ListNode* prev;
    value_type value;

    ListNode(const value_type& value):
      next(nullptr),
      prev(nullptr),
      value(value)
    {}

    template < typename... Args >
    ListNode(Args&&... args):
      next(nullptr),
      prev(nullptr),
      value(std::forward< Args >(args)...)
    {}
  };
}

#endif
