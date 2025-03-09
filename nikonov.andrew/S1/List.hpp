#ifndef LIST_HPP
#define LIST_HPP
#include "ListIterator.hpp"
#include <cstddef>
#include <cstdlib>
namespace nikonov
{
  template< typename T >
  class List
  {
    ListNode< T > * fake;
  public:
    List():
      fake(static_cast< ListNode< T > * >(malloc(sizeof(ListNode< T >))))
    {
      fake->next = fake;
    }
    List(const List & copy):
      fake(static_cast< ListNode< T > * >(malloc(sizeof(ListNode< T >))))
    {
      fake->next = fake;
      ConstListIterator< T > iter = copy.cbegin();
      ListNode< T > * curr = fake;
      while (iter != copy.cend())
      {
        ListNode< T > * newNode = new ListNode< T >{ *iter, fake };
        curr->next = newNode;
      }
    }
    List(List && copy):
      fake(copy.fake)
    {
      copy.fake = nullptr;
    }
    List(size_t k, const T & value):
      fake(static_cast< ListNode< T > * >(malloc(sizeof(ListNode< T >))))
    {
      fake->next = fake;
      ListNode< T > * tailNode = nullptr;
      for (size_t i = 0; i < k; ++i)
      {
        ListNode< T > * newNode = new ListNode< T >{ value, fake };
        if (fake->next == fake)
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
      if (fake != nullptr)
      {
        ListNode< T > * curr = fake->next;
        while (curr != fake)
        {
          ListNode< T > * next = curr->next;
          delete curr;
          curr = next;
        }
      }
      free(fake);
    }

    ListIterator< T > begin() noexcept
    {
      return { fake->next };
    }
    ConstListIterator< T > cbegin() const noexcept
    {
      return { fake->next };
    }
    ListIterator< T > end() noexcept
    {
      return { fake };
    }
    ConstListIterator< T > cend() const noexcept
    {
      return { fake };
    }

    T & front()
    {
      return *begin();
    }
    const T & front() const
    {
      return *begin();
    }
    const T & back() const
    {
      ListNode< T > * iter = fake->next;
      while (iter->next != fake)
      {
        iter = iter->next;
      }
      return iter->data;
    }
    T & back()
    {
      return const_cast< T & >(static_cast< const List & >(*this).back());
    }

    bool empty() const noexcept
    {
      return fake->next == fake;
    }
    size_t size() const noexcept
    {
      ConstListIterator< T > iter = begin();
      size_t size = 0;
      while (iter != end())
      {
        ++size;
        ++iter;
      }
      return size;
    }

    void push_front(const T & value)
    {
      ListNode< T > * next = fake->next;
      ListNode< T > * newNode = new ListNode< T >{ value, next };
      fake->next = newNode;
    }
    void push_back(const T & value)
    {
      ListNode< T > * newNode = new ListNode< T >{ value, fake };
      ListNode< T > * iter = fake;
      while (iter->next != fake)
      {
        iter = iter->next;
      }
      iter->next = newNode;
    }
    void pop_front()
    {
      ListNode< T > * toDelete = fake->next;
      ListNode< T > * subhead = toDelete->next;
      fake->next = subhead;
      delete toDelete;
    }
    void pop_back()
    {
      ListNode< T > * toDelete = fake;
      ListNode< T > * subhead = fake;
      while (toDelete->next != fake)
      {
        subhead = toDelete;
        toDelete = toDelete->next;
      }
      delete toDelete;
      subhead->next = fake;
    }
    void swap(List< T > & rhs)
    {
      std::swap(fake, rhs.fake);
    }
    void clear() noexcept
    {
      ListNode< T > * toDelite = fake->next;
      while (toDelite != fake)
      {
        ListNode< T > * next = toDelite->next;
        delete toDelite;
        toDelite = next;
      }
      fake->next = fake;
    }
    void remove(const T & val)
    {
      auto equal = [& val](const T & data)
      {
        return val == data;
      };
      remove_if(equal);
    }
    template < typename Predicate >
    void remove_if(Predicate pred)
    {
      ListNode< T > * iter = fake->next;
      ListNode< T > * subhead = fake;
      ListNode< T > * next = iter->next;
      while (iter != fake)
      {
        next = iter->next;
        if (pred(iter->data))
        {
          delete iter;
          subhead->next = next;
        }
        else
        {
          subhead = iter;
        }
        iter = next;
      }
    }
    void assign(size_t n, const T & val)
    {
      ListNode< T > * curr = fake->next;
      size_t lSize = size();
      for (size_t i = 0; i < n && curr != fake; ++i)
      {
        curr->data = val;
      }
      if (size < n)
      {
        for (size_t j = 0; j < n - size; ++j)
        {
          curr->next = new ListNode< T >{ val, fake };
          curr = curr->next;
        }
      }
      else
      {
        ListNode< T > * next = curr->next;
        while (next != fake)
        {
          ListNode< T > * toDelete = next;
          next = next->next;
          delete toDelete;
        }
        curr->next = fake;
      }
    }
  };
}
#endif
