#ifndef LIST_HPP
#define LIST_HPP
#include "ListIterator.hpp"
#include <cstddef>
#include <cstdlib>
#include <initializer_list>
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
    List(std::initializer_list< T > il):
      fake(static_cast< ListNode< T > * >(malloc(sizeof(ListNode< T >))))
    {
      fake->next = fake;
      for (auto el : il)
      {
        ListNode< T > * newNode = new ListNode< T >{ el, fake };
        fake->next = newNode;
      }
    }
    List(ConstListIterator< T > begin, ConstListIterator< T > end):
      fake(static_cast< ListNode< T > * >(malloc(sizeof(ListNode< T >))))
    {
      fake->next = fake;
      while (begin != end)
      {
        ListNode< T > * newNode = new ListNode< T >{ *(begin++), fake };
        fake->next = newNode;
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

    List< T > & operator= (std::initializer_list< T > il)
    {
      List< T > tempList(il);
      swap(tempList);
      return *this;
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
      ConstListIterator< T > iter = cbegin();
      size_t size = 0;
      while (iter != cend())
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
      ListNode< T > * curr = fake->next;
      ListNode< T > * subhead = fake;
      while (curr != fake)
      {
        ListNode< T > * next = curr->next;
        if (pred(curr->data))
        {
          delete curr;
          subhead->next = next;
        }
        else
        {
          subhead = curr;
        }
        curr = next;
      }
    }
    void assign(size_t n, const T & val)
    {
      List< T > tempList(n, val);
      swap(tempList);
    }
    void assign(ListIterator< T > begin, ListIterator< T > end)
    {
      List< T > tempList(begin, end);
      swap(tempList);
    }
    void assign(std::initializer_list< T > il)
    {
      List< T > tempList(il);
      swap(tempList);
    }
    ListIterator< T > insert(ConstListIterator< T > position, const T& val)
    {
      ListNode< T > * subhead = fake;
      ListNode< T > * next = fake->next;
      while(next != position.node)
      {
        subhead = next;
        next = next->next;
      }
      ListNode< T > * newNode = new ListNode< T >{ val,  next };
      subhead->next = newNode;
      return ListIterator< T >{ newNode };
    }
    ListIterator< T > erase(ConstListIterator< T > position)
    {
      ListNode< T > * subhead = fake;
      ListNode< T > * curr = fake->next;
      ListNode< T > * next = curr->next;
      while(curr != position.node)
      {
        subhead = curr;
        curr = next;
        next = curr->next;
      }
      delete curr;
      subhead->next = next;
      return ListIterator< T >{ next };
    }
  };
}
#endif
