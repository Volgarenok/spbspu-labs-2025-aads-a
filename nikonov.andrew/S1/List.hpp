#ifndef LIST_HPP
#define LIST_HPP
#include "ListIterator.hpp"
#include <cstddef>
#include <cstdlib>
#include <initializer_list>
namespace nikonov
{
  template< typename T >
  class List;
}
namespace
{
  template< typename T, typename Compare >
  bool compareLists(const nikonov::List< T > & lhs, const nikonov::List< T > & rhs, Compare cmp);
  template< typename T >
  bool tEqual(const T & a, const T & b);
  template< typename T >
  bool tLess(const T & a, const T & b);
  template< typename T >
  bool tGreater(const T & a, const T & b);
}
namespace nikonov
{
  template< typename T >
  using iter = ListIterator< T >;
  template< typename T >
  using citer = ConstListIterator< T >;

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
    template < typename InputIterator >
    List(InputIterator begin, InputIterator end);
    ~List() noexcept;

    List< T > & operator=(std::initializer_list< T > il);
    bool operator==(const List< T > & rhs) const;
    bool operator!=(const List< T > & rhs) const;
    bool operator<(const List< T > & rhs) const;
    bool operator>(const List< T > & rhs) const;
    bool operator<=(const List< T > & rhs) const;
    bool operator>=(const List< T > & rhs) const;

    iter< T > begin() noexcept;
    citer< T > begin() const noexcept;
    citer< T > cbegin() const noexcept;
    iter< T > end() noexcept;
    citer< T > end() const noexcept;
    citer< T > cend() const noexcept;

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
    void splice(citer< T > position, List< T > & x);
    void splice(citer< T > position, List< T > && x);
    void splice(citer< T > position, List< T > & x, citer< T > i);
    void splice(citer< T > position, List< T > && x, citer< T > i);
    void splice(citer< T > position, List< T > & x, citer< T > first, citer< T > last);
    void splice(citer< T > position, List< T > && x,citer< T > first, citer< T > last);
    void reverse() noexcept;

    void assign(size_t n, const T & val);
    void assign(iter< T > begin, iter< T > end);
    void assign(std::initializer_list< T > il);
    iter< T > insert(citer< T > position, const T& val);
    iter< T > erase(citer< T > position);
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
    citer< T > iter = copy.cbegin();
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
  template< typename InputIterator >
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
  bool List< T >::operator==(const List< T > & rhs) const
  {
    if (std::addressof(*this) == std::addressof(rhs))
    {
      return true;
    }
    else if (size() != rhs.size())
    {
      return false;
    }
    return compareLists(*this, rhs, tEqual< T >);
  }

  template< typename T >
  bool List< T >::operator!=(const List< T > & rhs) const
  {
    return !(*this == rhs);
  }

  template< typename T >
  bool List< T >::operator<(const List< T > & rhs) const
  {
   if (size() < rhs.size())
    {
      return true;
    }
    return compareLists(*this, rhs, tLess< T >);
  }

  template< typename T >
  bool List< T >::operator>(const List< T > & rhs) const
  {
    if (size() > rhs.size())
    {
      return true;
    }
    return compareLists(*this, rhs, tGreater< T >);
  }

  template< typename T >
  bool List< T >::operator<=(const List< T > & rhs) const
  {
    return !(*this > rhs);
  }

  template< typename T >
  bool List< T >::operator>=(const List< T > & rhs) const
  {
    return !(*this < rhs);
  }

  template< typename T >
  iter< T > List< T >::begin() noexcept
  {
    return { fake->next };
  }

  template< typename T >
  citer< T > List< T >::begin() const noexcept
  {
    return { fake->next };
  }

  template< typename T >
  citer< T > List< T >::cbegin() const noexcept
  {
    return { fake->next };
  }

  template< typename T >
  iter< T > List< T >::end() noexcept
  {
    return { fake };
  }

  template< typename T >
  citer< T > List< T >::end() const noexcept
  {
    return { fake };
  }

  template< typename T >
  citer< T > List< T >::cend() const noexcept
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
    citer< T > iter = cbegin();
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
  void List< T >::splice(citer< T > position, List< T > & x)
  {
    splice(position, std::move(x));
  }

  template< typename T >
  void List< T >::splice(citer< T > position, List< T > && x)
  {
    if (x.empty())
    {
      return;
    }
    iter< T > curr = begin();
    while (curr.node->next != position.node)
    {
      ++curr;
    }
    ListNode< T > * prevNext = curr.node->next;
    curr.node->next = x.fake->next;
    ListNode< T > * listIter = x.fake;
    while (listIter->next != x.fake)
    {
      listIter = listIter->next;
    }
    listIter->next = prevNext;
    x.fake->next = x.fake;
  }

  template< typename T >
  void List< T >::splice(citer< T > position, List< T > & x, citer< T > i)
  {
    splice(position, std::move(x), i);
  }

  template< typename T >
  void List< T >::splice(citer< T > position, List< T > && x, citer< T > i)
  {
    if (x.empty())
    {
      return;
    }
    iter< T > curr = begin();
    while (curr.node->next != position.node)
    {
      ++curr;
    }
    ListNode< T > * prevNext = curr.node->next;
    iter< T > subheadX = x.begin();
    while (subheadX.node->next != i.node)
    {
      ++subheadX;
    }
    ListNode< T > * toMove = subheadX.node->next;
    subheadX.node->next = toMove->next;
    curr.node->next = toMove;
    toMove->next = prevNext;
  }

  template< typename T >
  void List< T >::splice(citer< T > position, List< T > & x, citer< T > first, citer< T > last)
  {
    splice(position, std::move(x), first, last);
  }

  template< typename T >
  void List< T >::splice(citer< T > position, List< T > && x, citer< T > first, citer< T > last)
  {
    if (x.empty())
    {
      return;
    }
    iter< T > curr = begin();
    while (curr.node->next != position.node)
    {
      ++curr;
    }
    ListNode< T > * prevNext = curr.node->next;
    iter< T > subheadX = x.begin();
    while (subheadX.node->next != first.node)
    {
      ++subheadX;
    }
    curr.node->next = subheadX.node->next;
    iter< T > currTail = subheadX.node;
    iter< T > endX = subheadX.node->next;
    while (currTail.node->next != last.node)
    {
      ++currTail;
      ++endX;
    }
    currTail.node->next = prevNext;
    subheadX.node->next = endX.node;
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
  void List< T >::assign(iter< T > begin, iter< T > end)
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
  iter< T > List< T >::insert(citer< T > position, const T& val)
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
    return iter< T >{ newNode };
  }

  template< typename T >
  iter< T > List< T >::erase(citer< T > position)
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
    return iter< T >{ next };
  }
}
namespace
{
  template< typename T, typename Compare >
  bool compareLists(const nikonov::List< T > & lhs, const nikonov::List< T > & rhs, Compare cmp)
  {
    assert(lhs.size() == rhs.size());
    nikonov::citer< T > thisIt = lhs.cbegin();
    nikonov::citer< T > anotherIt = rhs.cbegin();
    while (thisIt != lhs.end())
    {
      if (!cmp(*thisIt, *anotherIt))
      {
        return false;
      }
      ++thisIt;
      ++anotherIt;
    }
    return true;
  }
  template< typename T >
  bool tEqual(const T & a, const T & b)
  {
    return a == b;
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
