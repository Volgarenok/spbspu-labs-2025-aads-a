#ifndef FWDLISTNODE_HPP
#define FWDLISTNODE_HPP

#include <utility>

namespace zholobov {

  template < typename T >
  struct FwdListNode {
    T value;
    FwdListNode* next;
    FwdListNode():
      value(), next(nullptr) {}
    explicit FwdListNode(const T& val, FwdListNode* nextNode = nullptr):
      value(val), next(nextNode) {}
    explicit FwdListNode(T&& val, FwdListNode* nextNode = nullptr):
      value(std::move(val)), next(nextNode) {}
  };

}

#endif
