#ifndef LIST_HPP
#define LIST_HPP
#include "ListIterator.hpp"
#include <cstddef>
#include <cstdlib>
#include <initializer_list>
#include <type_traits>
namespace
{
  template< typename T >
  bool tLess(const T & a, const T & b);
  template< typename T >
  bool tGreater(const T & a, const T & b);
}
namespace nikonov
{
  template< typename T >
  class List
  {
    ListNode< T > * fake;
  public:
    List();
    List(const List & copy);
    List(List && copy);
    List(size_t k, const T & value);
    List(std::initializer_list< T > il);
    template < typename InputIterator, typename = typename std::enable_if< \
      std::is_same< InputIterator, ConstListIterator< T > >::value || \
        std::is_same< InputIterator, ListIterator< T > >::value >::type >
    List(InputIterator begin, InputIterator end);
    ~List() noexcept;

    List< T > & operator=(std::initializer_list< T > il);
    bool operator==(const List< T > & rhs);
    bool operator!=(const List< T > & rhs);
    bool operator<(const List< T > & rhs);
    bool operator>(const List< T > & rhs);
    bool operator<=(const List< T > & rhs);
    bool operator>=(const List< T > & rhs);

    ListIterator< T > begin() noexcept;
    ConstListIterator< T > cbegin() const noexcept;
    ListIterator< T > end() noexcept;
    ConstListIterator< T > cend() const noexcept;

    T & front();
    const T & front() const;
    T & back();
    const T & back() const;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void push_front(const T & value);
    void push_front(T && value);
    void push_back(const T & value);
    void push_back(T && value);
    void pop_front();
    void pop_back();

    void swap(List< T > & rhs) noexcept;
    void clear() noexcept;
    void remove(const T & val);
    template < typename Predicate >
    void remove_if(Predicate pred);
    void splice(ConstListIterator< T > position, List< T > & x);
    void splice(ConstListIterator< T > position, List< T > && x);
    void splice(ConstListIterator< T > position, List< T > & x, ConstListIterator< T > i);
    void splice(ConstListIterator< T > position, List< T > && x, ConstListIterator< T > i);
    void splice(ConstListIterator< T > position, List< T > & x, ConstListIterator< T > first, ConstListIterator< T > last);
    void splice(ConstListIterator< T > position, List< T > && x,ConstListIterator< T > first, ConstListIterator< T > last);
    void reverse() noexcept;

    void assign(size_t n, const T & val);
    void assign(ListIterator< T > begin, ListIterator< T > end);
    void assign(std::initializer_list< T > il);
    ListIterator< T > insert(ConstListIterator< T > position, const T& val);
    ListIterator< T > erase(ConstListIterator< T > position);
  };
  template< typename T >
  List< T >::List():
  fake(static_cast< ListNode< T > * >(malloc(sizeof(ListNode< T >))))
  {
    fake->next = fake;
  }

  template< typename T >
  List< T >::List(const List< T > & copy):
    fake(static_cast< ListNode< T > * >(malloc(sizeof(ListNode< T >))))
  {
    fake->next = fake;
    ConstListIterator< T > iter = copy.cbegin();
    ListNode< T > * curr = fake;
    while (iter != copy.cend())
    {
      ListNode< T > * newNode = new ListNode< T >{ *(iter++), fake };
      curr->next = newNode;
      curr = newNode;
    }
  }
  template< typename T >
  List< T >::List(List && copy):
    fake(copy.fake)
  {
    copy.fake = static_cast< ListNode< T > * >(malloc(sizeof(ListNode< T >)));;
    copy.fake->next = copy.fake;
  }

  template< typename T >
  List< T >::List(size_t k, const T & value):
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

  template< typename T >
  List< T >::List(std::initializer_list< T > il):
    fake(static_cast< ListNode< T > * >(malloc(sizeof(ListNode< T >))))
  {
    fake->next = fake;
    ListNode< T > * curr = fake;
    for (auto el : il)
    {
      ListNode< T > * newNode = new ListNode< T >{ el, fake };
      curr->next = newNode;
      curr = newNode;
    }
  }

  template< typename T >
  template< typename InputIterator, typename >
  List< T >::List(InputIterator begin, InputIterator end):
    fake(static_cast< ListNode< T > * >(malloc(sizeof(ListNode< T >))))
  {
    fake->next = fake;
    ListNode< T > * curr = fake;
    for (; begin != end; ++begin)
    {
      ListNode< T > * newNode = new ListNode< T >{ *begin, fake };
      curr->next = newNode;
      curr = newNode;
    }
  }

  template< typename T >
  List< T >::~List() noexcept
  {
    clear();
    free(fake);
  }

  template< typename T >
  List< T > & List< T >::operator=(std::initializer_list< T > il)
  {
    List< T > tempList(il);
    swap(tempList);
    return *this;
  }

  template< typename T >
  bool List< T >::operator==(const List< T > & rhs)
  {
    if (std::addressof(*this) == std::addressof(rhs))
    {
      return true;
    }
    return compareList(*this, rhs, std::equal< T >);
  }
  
  template< typename T >
  bool List< T >::operator!=(const List< T > & rhs)
  {
    return !(*this == rhs);
  }

  template< typename T >
  bool List< T >::operator<(const List< T > & rhs)
  {
    return compareLists(*this, rhs, tLess< T >);
  }

  template< typename T >
  bool List< T >::operator>(const List< T > & rhs)
  {
    return compareLists(*this, rhs, tGreater< T >);
  }

  template< typename T >
  bool List< T >::operator<=(const List< T > & rhs)
  {
    return !(*this > rhs);
  }

  template< typename T >
  bool List< T >::operator>=(const List< T > & rhs)
  {
    return !(*this < rhs);
  }

  template< typename T >
  ListIterator< T > List< T >::begin() noexcept
  {
    return { fake->next };
  }

  template< typename T >
  ConstListIterator< T > List< T >::cbegin() const noexcept
  {
    return { fake->next };
  }

  template< typename T >
  ListIterator< T > List< T >::end() noexcept
  {
    return { fake };
  }

  template< typename T >
  ConstListIterator< T > List< T >::cend() const noexcept
  {
    return { fake };
  }

  template< typename T >
  T &  List< T >::front()
  {
    return *begin();
  }

  template< typename T >
  const T &  List< T >::front() const
  {
    return *begin();
  }

  template< typename T >
  const T & List< T >::back() const
  {
    ListNode< T > * iter = fake->next;
    while (iter->next != fake)
    {
      iter = iter->next;
    }
    return iter->data;
  }

  template< typename T >
  T & List< T >::back()
  {
    return const_cast< T & >(static_cast< const List & >(*this).back());
  }

  template< typename T >
  bool List< T >::empty() const noexcept
  {
    return fake->next == fake;
  }

  template< typename T >
  size_t List< T >::size() const noexcept
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

  template< typename T >
  void List< T >::push_front(const T & value)
  {
    ListNode< T > * next = fake->next;
    ListNode< T > * newNode = new ListNode< T >{ value, next };
    fake->next = newNode;
  }

  template< typename T >
  void List< T >::push_front(T && value)
  {
    T * copy(new T(value));
    push_front(*copy);
  }

  template< typename T >
  void List< T >::push_back(const T & value)
  {
    ListNode< T > * newNode = new ListNode< T >{ value, fake };
    ListNode< T > * iter = fake;
    while (iter->next != fake)
    {
      iter = iter->next;
    }
    iter->next = newNode;
  }

  template< typename T >
  void List< T >::push_back(T && value)
  {
    T * copy(new T(value));
    push_back(*copy);
  }

  template< typename T >
  void List< T >::pop_front()
  {
    ListNode< T > * toDelete = fake->next;
    ListNode< T > * subhead = toDelete->next;
    fake->next = subhead;
    delete toDelete;
  }

  template< typename T >
  void List< T >::pop_back()
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

  template< typename T >
  void List< T >::swap(List< T > & rhs) noexcept
  {
    std::swap(fake, rhs.fake);
  }

  template< typename T >
  void List< T >::clear() noexcept
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

  template< typename T >
  void List< T >::remove(const T & val)
  {
    auto equal = [& val](const T & data)
    {
      return val == data;
    };
    remove_if(equal);
  }

  template< typename T >
  template < typename Predicate >
  void List< T >::remove_if(Predicate pred)
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
  template< typename T >
  void List< T >::splice(ConstListIterator< T > position, List< T > & x)
  {
    splice(position, std::move(x));
  }

  template< typename T >
  void List< T >::splice(ConstListIterator< T > position, List< T > && x)
  {
    ListIterator< T > curr = begin();
    while (curr.node != position.node)
    {
      ++curr;
    }
    ListNode< T > * prevNext = curr.node->next;
    curr.node->next = x.fake->next;
    ListNode< T > * listIter = x.fake;
    while (listIter->next != x.fake)
    {
      ++listIter;
    }
    listIter->next = prevNext;
    x.fake->next == x.fake;
  }

  template< typename T >
  void List< T >::splice(ConstListIterator< T > position, List< T > & x, ConstListIterator< T > i)
  {
    splice(position, std::move(x), i);
  }

  template< typename T >
  void List< T >::splice(ConstListIterator< T > position, List< T > && x, ConstListIterator< T > i)
  {
    ListIterator< T > curr = begin();
    while (curr.node != position.node)
    {
      ++curr;
    }
    ListNode< T > * prevNext = curr.node->next;
    ListIterator< T > subheadX = x.begin();
    while (subheadX.node->next != i->node)
    {
      ++subheadX;
    }
    ListNode< T > * toMove = subheadX.node->next;
    subheadX.node->next = toMove->next;
    curr.node->next = toMove;
    toMove->next = prevNext;
  }

  template< typename T >
  void List< T >::splice(ConstListIterator< T > position, List< T > & x, ConstListIterator< T > first, ConstListIterator< T > last)
  {
    splice(position, std::move(x), first, last);
  }

  template< typename T >
  void List< T >::splice(ConstListIterator< T > position, List< T > && x, ConstListIterator< T > first, ConstListIterator< T > last)
  {
    ListIterator< T > curr = begin();
    while (curr.node != position.node)
    {
      ++curr;
    }
    ListNode< T > * prevNext = curr.node->next;
    ListIterator< T > subheadX = x.begin();
    while (subheadX.node->next != first->node)
    {
      ++subheadX;
    }
    curr.node->next = subheadX.node->next;
    ListIterator< T > currTail = subheadX.node;
    ListIterator< T > endX = subheadX.node->next;
    while (currTail.node->next != last.node)
    {
      ++currTail;
      ++endX;
    }
    currTail.node->next = prevNext;
    subheadX.node->next = endX;
  }

  template< typename T >
  void List< T >::reverse() noexcept
  {
    ListNode< T > * prevPtr = fake;
    ListNode< T > * iter = prevPtr->next;
    while (iter != fake)
    {
      ListNode< T > * next = iter->next;
      iter->next = prevPtr;
      prevPtr = iter;
      iter = next;
    }
    iter->next = prevPtr;
  }

  template< typename T >
  void List< T >::assign(size_t n, const T & val)
  {
    List< T > tempList(n, val);
    swap(tempList);
  }

  template< typename T >
  void List< T >::assign(ListIterator< T > begin, ListIterator< T > end)
  {
    List< T > tempList(begin, end);
    swap(tempList);
  }

  template< typename T >
  void List< T >::assign(std::initializer_list< T > il)
  {
    List< T > tempList(il);
    swap(tempList);
  }

  template< typename T >
  ListIterator< T > List< T >::insert(ConstListIterator< T > position, const T& val)
  {
    ListNode< T > * subhead = fake;
    ListNode< T > * next = fake->next;
    while (next != position.node)
    {
      subhead = next;
      next = next->next;
    }
    ListNode< T > * newNode = new ListNode< T >{ val,  next };
    subhead->next = newNode;
    return ListIterator< T >{ newNode };
  }

  template< typename T >
  ListIterator< T > List< T >::erase(ConstListIterator< T > position)
  {
    ListNode< T > * subhead = fake;
    ListNode< T > * curr = fake->next;
    ListNode< T > * next = curr->next;
    while (curr != position.node)
    {
      subhead = curr;
      curr = next;
      next = curr->next;
    }
    delete curr;
    subhead->next = next;
    return ListIterator< T >{ next };
  }
}
namespace
{
  template< typename T, typename Compare >
  bool compareLists(const nikonov::List< T > & lhs, const nikonov::List< T > & rhs, Compare cmp)
  {
    nikonov::ConstListIterator< T > thisIt = lhs.begin();
    nikonov::ConstListIterator< T > anotherIt = rhs.begin();
    while (thisIt != lhs.end())
    {
      if (anotherIt == rhs.end() || !cmp(*thisIt, *anotherIt))
      {
        return false;
      }
      ++thisIt;
      ++anotherIt;
    }
    return true;
  }
  template< typename T >
  bool tLess(const T & a, const T & b)
  {
    return a < b;
  }
  template< typename T >
  bool tGreater(const T & a, const T & b)
  {
    return a > b;
  }
}
#endif
