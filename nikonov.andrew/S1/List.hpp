#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <memory>
namespace nikonov
{
  template< typename T >
  struct ListNode
  {
    T data;
    ListNode< T > * next;
  };
  template< typename T >
  struct ListIterator
  {
    ListNode< T > * node;
    using this_t = ListIterator< T >;

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
    const T & operator*() const
    {
      return node->data;
    }

    T * operator->()
    {
      return std::addressof(node->data);
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
  class List
  {
    ListNode< T > * fake;
  public:
    List():
      fake(new ListNode< T >{ T(), nullptr })
    {}
    List(size_t k, const T & value):
      fake(new ListNode< T >{ T(), nullptr })
    {
      ListNode< T > * tailNode = nullptr;
      for (size_t i = 0; i < k; ++i)
      {
        ListNode< T > * newNode = new ListNode< T >{ value, fake };
        if (fake->next == nullptr)
        {
          fake->next = newNode;
          tailNode = newNode;
          continue;
        }
        tailNode->next = newNode;
        tailNode = newNode;
      }
    }
    ~List() noexcept
    {
      ListNode< T > * curr = fake->next;
      while (curr != end())
      {
        ListNode< T > * next = curr->next;
        delete curr;
        curr = next; 
      }
    }
    ListIterator< T > begin() noexcept
    {
      return { fake->next };
    }
    ListIterator< T > end() noexcept
    {
      return ListIterator< T >{ fake };
    }

    T& front() const
    {
      return fake->data;
    }
    T& back() const
    {
      return fake->data;
    }

    bool empty() const noexcept
    {
      return fake->next == nullptr;
    }
    size_t size() const noexcept
    {
      ListIterator< T > iter = begin();
      size_t size = 0;
      while (iter++ != fake)
      {
        ++size;
      }
      return size;
    }

    void push_(const T &  value)
    {
      ListNode< T > * newNode = new ListNode< T >{ value, fake };
      ListNode< T > * iter = fake->next;
      while (iter->next != fake)
      {
        iter = iter->next;
      }
      iter->next = newNode;
    }
    void pop_()
    {
      ListNode< T > * iter = fake->next;
      while (iter->next != fake)
      {
        iter = iter->next;
      }
      delete iter;
      iter = fake;
    }
  };
}
#endif
