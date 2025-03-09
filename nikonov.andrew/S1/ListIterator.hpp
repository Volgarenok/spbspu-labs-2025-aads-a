#include "ListNode.hpp"
#include <memory>
namespace nikonov
{
  template< typename T >
  struct ListIterator
  {
    ListNode< T > * node;
    using this_t = ListIterator< T >;
    ListIterator(ListNode<T>* ptr = nullptr):
      node(ptr)
    {}
    ListIterator():
      node(nullptr)
    {}
    ~ListIterator() = default;
    ListIterator(const this_t &) = default;
    this_t & operator=(const this_t &) = default;

    this_t & operator++()
    {
      node = node->next;
      return *this;
    }
    this_t operator++(int)
    {
      this_t tempCopy(*this);
      ++(*this);
      return tempCopy;
    }

    T & operator*()
    {
      return node->data;
    }

    const T * operator->() const
    {
      return std::addressof(node->data);
    }

    bool operator!=(const this_t & rhs) const
    {
      return !(*this == rhs);
    }
    bool operator==(const this_t & rhs) const
    {
      return node == rhs.node;
    }
  };

  template< typename T >
  struct ConstListIterator
  {
    ListNode< T > * node;
    using this_t = ConstListIterator< T >;
    ListIterator(ListNode<T>* ptr = nullptr):
      node(ptr)
    {}
    ListIterator():
      node(nullptr)
    {}
    ~ListIterator() = default;
    ListIterator(const this_t &) = default;
    this_t & operator=(const this_t &) = default;

    this_t & operator++()
    {
      node = node->next;
      return *this;
    }
    this_t operator++(int)
    {
      this_t tempCopy(*this);
      ++(*this);
      return tempCopy;
    }

    const T & operator*() const
    {
      return node->data;
    }

    const T * operator->() const
    {
      return std::addressof(node->data);
    }

    bool operator!=(const this_t & rhs) const
    {
      return !(*this == rhs);
    }
    bool operator==(const this_t & rhs) const
    {
      return node == rhs.node;
    }
  };
}
