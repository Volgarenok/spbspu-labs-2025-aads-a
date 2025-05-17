#ifndef FWDLISTNODE_HPP
#define FWDLISTNODE_HPP

#include <utility>

namespace zholobov {

  struct FwdListNodeBase {
    FwdListNodeBase() = default;
    explicit FwdListNodeBase(FwdListNodeBase* next_node):
      next(next_node)
    {}

    FwdListNodeBase(const FwdListNodeBase&) = delete;
    FwdListNodeBase(FwdListNodeBase&& other):
      next(other.next)
    {
      other.next = nullptr;
    }

    FwdListNodeBase& operator=(const FwdListNodeBase&) = delete;
    FwdListNodeBase& operator=(FwdListNodeBase&& other)
    {
      next = other.next;
      other.next = nullptr;
      return *this;
    }

    FwdListNodeBase* next = nullptr;
  };

  template < typename T >
  struct FwdListNode: public FwdListNodeBase {
    T value;
    FwdListNode():
      FwdListNodeBase(),
      value()
    {}

    explicit FwdListNode(const T& val, FwdListNodeBase* next_node = nullptr):
      FwdListNodeBase(next_node),
      value(val)
    {}

    explicit FwdListNode(T&& val, FwdListNodeBase* next_node = nullptr):
      FwdListNodeBase(next_node),
      value(std::move(val))
    {}
  };

}

#endif
