#ifndef FORWARDLISTNODE_HPP
#define FORWARDLISTNODE_HPP

namespace shramko
{
  template< typename T >
  struct ListNode
  {
    T dataValue;
    ListNode<T>* nextPtr;
    ListNode(T val) : dataValue(val), nextPtr(nullptr) {}
  };
}

#endif
