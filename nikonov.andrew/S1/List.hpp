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
namespace detail
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
  using iterator = ListIterator< T >;
  template< typename T >
  using const_iterator = ConstListIterator< T >;

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
    template< typename InputIterator >
    List(InputIterator begin, InputIterator end);
    ~List() noexcept;

    List< T > & operator=(std::initializer_list< T > il);
    bool operator==(const List< T > & rhs) const noexcept;
    bool operator!=(const List< T > & rhs) const noexcept;
    bool operator<(const List< T > & rhs) const noexcept;
    bool operator>(const List< T > & rhs) const noexcept;
    bool operator<=(const List< T > & rhs) const noexcept;
    bool operator>=(const List< T > & rhs) const noexcept;

    iterator< T > begin() noexcept;
    const_iterator< T > begin() const noexcept;
    const_iterator< T > cbegin() const noexcept;
    iterator< T > end() noexcept;
    const_iterator< T > end() const noexcept;
    const_iterator< T > cend() const noexcept;

    T & front() noexcept;
    const T & front() const noexcept;
    T & back() noexcept;
    const T & back() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void push_front(const T & value);
    void push_front(T && value);
    void push_back(const T & value);
    void push_back(T && value);
    void pop_front() noexcept;
    void pop_back() noexcept;

    void swap(List< T > & rhs) noexcept;
    void clear() noexcept;
    void remove(const T & val) noexcept;
    template< typename Predicate >
    void remove_if(Predicate pred) noexcept;
    void splice(const_iterator< T > pos, List< T > & x) noexcept;
    void splice(const_iterator< T > pos, List< T > && x) noexcept;
    void splice(const_iterator< T > pos, List< T > & x, const_iterator< T > i) noexcept;
    void splice(const_iterator< T > pos, List< T > && x, const_iterator< T > i) noexcept;
    void splice(const_iterator< T > pos, List< T > & x, const_iterator< T > first, const_iterator< T > last) noexcept;
    void splice(const_iterator< T > pos, List< T > && x,const_iterator< T > first, const_iterator< T > last) noexcept;
    void reverse() noexcept;

    void assign(size_t n, const T & val) noexcept;
    void assign(iterator< T > begin, iterator< T > end) noexcept;
    void assign(std::initializer_list< T > il) noexcept;
    iterator< T > insert(const_iterator< T > position, const T& val);
    iterator< T > insert(const_iterator< T > position, size_t n, const T & val);
    template < typename InputIterator >
    iterator< T > insert(const_iterator< T > position, InputIterator first, InputIterator last);
    iterator< T > insert(const_iterator< T > position, T && val);
    iterator< T > insert(const_iterator< T > position, std::initializer_list< T > il);
    iterator< T > erase(const_iterator< T > position) noexcept;
    iterator< T > erase(const_iterator< T > first, const_iterator< T > last) noexcept;
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
    const_iterator< T > iter = copy.cbegin();
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
  bool List< T >::operator==(const List< T > & rhs) const noexcept
  {
    if (std::addressof(*this) == std::addressof(rhs))
    {
      return true;
    }
    else if (size() != rhs.size())
    {
      return false;
    }
    return detail::compareLists(*this, rhs, detail::tEqual< T >);
  }

  template< typename T >
  bool List< T >::operator!=(const List< T > & rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< typename T >
  bool List< T >::operator<(const List< T > & rhs) const noexcept
  {
   if (size() < rhs.size())
    {
      return true;
    }
    return detail::compareLists(*this, rhs, detail::tLess< T >);
  }

  template< typename T >
  bool List< T >::operator>(const List< T > & rhs) const noexcept
  {
    if (size() > rhs.size())
    {
      return true;
    }
    return detail::compareLists(*this, rhs, detail::tGreater< T >);
  }

  template< typename T >
  bool List< T >::operator<=(const List< T > & rhs) const noexcept
  {
    return !(*this > rhs);
  }

  template< typename T >
  bool List< T >::operator>=(const List< T > & rhs) const noexcept
  {
    return !(*this < rhs);
  }

  template< typename T >
  iterator< T > List< T >::begin() noexcept
  {
    return iterator< T >(fake->next);
  }

  template< typename T >
  const_iterator< T > List< T >::begin() const noexcept
  {
    return const_iterator< T >(fake->next);
  }

  template< typename T >
  const_iterator< T > List< T >::cbegin() const noexcept
  {
    return const_iterator< T >(fake->next);
  }

  template< typename T >
  iterator< T > List< T >::end() noexcept
  {
    return iterator< T >(fake);
  }

  template< typename T >
  const_iterator< T > List< T >::end() const noexcept
  {
    return const_iterator< T >(fake);
  }

  template< typename T >
  const_iterator< T > List< T >::cend() const noexcept
  {
    return const_iterator< T >(fake);
  }

  template< typename T >
  T & List< T >::front() noexcept
  {
    assert(!empty());
    return *begin();
  }

  template< typename T >
  const T & List< T >::front() const noexcept
  {
    assert(!empty());
    return *begin();
  }

  template< typename T >
  const T & List< T >::back() const noexcept
  {
    assert(!empty());
    ListNode< T > * iter = fake->next;
    while (iter->next != fake)
    {
      iter = iter->next;
    }
    return iter->data;
  }

  template< typename T >
  T & List< T >::back() noexcept
  {
    assert(!empty());
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
    const_iterator< T > iter = cbegin();
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
  void List< T >::pop_front() noexcept
  {
    assert(!empty());
    ListNode< T > * toDelete = fake->next;
    ListNode< T > * subhead = toDelete->next;
    fake->next = subhead;
    delete toDelete;
  }

  template< typename T >
  void List< T >::pop_back() noexcept
  {
    assert(!empty());
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
  void List< T >::remove(const T & val) noexcept
  {
    auto equal = [& val](const T & data)
    {
      return val == data;
    };
    remove_if(equal);
  }

  template< typename T >
  template< typename Predicate >
  void List< T >::remove_if(Predicate pred) noexcept
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
  void List< T >::splice(const_iterator< T > pos, List< T > & x) noexcept
  {
    splice(pos, std::move(x));
  }

  template< typename T >
  void List< T >::splice(const_iterator< T > pos, List< T > && x) noexcept
  {
    if (x.empty())
    {
      return;
    }
    iterator< T > curr = begin();
    while (curr.node->next != pos.node)
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
  void List< T >::splice(const_iterator< T > pos, List< T > & x, const_iterator< T > i) noexcept
  {
    splice(pos, std::move(x), i);
  }

  template< typename T >
  void List< T >::splice(const_iterator< T > pos, List< T > && x, const_iterator< T > i) noexcept
  {
    if (x.empty())
    {
      return;
    }
    iterator< T > curr = begin();
    while (curr.node->next != pos.node)
    {
      ++curr;
    }
    ListNode< T > * prevNext = curr.node->next;
    iterator< T > subheadX = x.begin();
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
  void List< T >::splice(const_iterator< T > pos, List< T > & x, const_iterator< T > first, const_iterator< T > last) noexcept
  {
    splice(pos, std::move(x), first, last);
  }

  template< typename T >
  void List< T >::splice(const_iterator< T > pos, List< T > && x, const_iterator< T > first, const_iterator< T > last) noexcept
  {
    if (x.empty())
    {
      return;
    }
    iterator< T > curr = begin();
    while (curr.node->next != pos.node)
    {
      ++curr;
    }
    ListNode< T > * prevNext = curr.node->next;
    iterator< T > subheadX = x.begin();
    while (subheadX.node->next != first.node)
    {
      ++subheadX;
    }
    curr.node->next = subheadX.node->next;
    iterator< T > currTail(subheadX.node);
    iterator< T > endX(subheadX.node->next);
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
  void List< T >::assign(size_t n, const T & val) noexcept
  {
    List< T > tempList(n, val);
    swap(tempList);
  }

  template< typename T >
  void List< T >::assign(iterator< T > begin, iterator< T > end) noexcept
  {
    List< T > tempList(begin, end);
    swap(tempList);
  }

  template< typename T >
  void List< T >::assign(std::initializer_list< T > il) noexcept
  {
    List< T > tempList(il);
    swap(tempList);
  }

  template< typename T >
  iterator< T > List< T >::insert(const_iterator< T > position, const T& val)
  {
    ListNode< T > * subhead = fake;
    ListNode< T > * next = fake->next;
    while (next != position.node)
    {
      subhead = next;
      next = next->next;
    }
    ListNode< T > * newNode = new ListNode< T >{ val, next };
    subhead->next = newNode;
    return iterator< T >{ newNode };
  }

  template< typename T >
  iterator< T > List< T >::insert(const_iterator< T > position, size_t n, const T & val)
  {
    iterator< T > curr = begin();
    while (curr.node != position.node)
    {
      ++curr;
    }
    if (!n)
    {
      return curr;
    }
    List< T > tempObj(n, val);
    iterator< T > retVal(tempObj.begin());
    splice(position, tempObj);
    return ++curr;
  }

  template < typename T >
  template < typename InputIterator >
  iterator< T > List< T >::insert(const_iterator< T > position, InputIterator first, InputIterator last)
  {
    iterator< T > curr = begin();
    while (curr.node != position.node)
    {
      ++curr;
    }
    if (first == last)
    {
      return curr;
    }
    List< T > tempObj(first, last);
    iterator< T > retVal(tempObj.begin());
    splice(position, tempObj);
    return ++curr;
  }

  template< typename T >
  iterator< T > List< T >::insert(const_iterator< T > position, T && val)
  {
    iterator< T > curr = begin();
    while (curr.node != position.node)
    {
      ++curr;
    }
    T tempObj(val);
    insert(position, tempObj);
    return ++curr;
  }

  template< typename T >
  iterator< T > List< T >::insert(const_iterator< T > position, std::initializer_list< T > il)
  {
    iterator< T > curr = begin();
    while (curr.node != position.node)
    {
      ++curr;
    }
    if (!il.size())
    {
      return curr;
    }
    List< T > tempObj(il);
    iterator< T > retVal(tempObj.begin());
    splice(position, tempObj);
    return ++curr;
  }

  template< typename T >
  iterator< T > List< T >::erase(const_iterator< T > position) noexcept
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
    return iterator< T >{ next };
  }

  template< typename T >
  iterator< T > List< T >::erase(const_iterator< T > first, const_iterator< T > last) noexcept
  {
    iterator< T > returnIter(fake);
    while (first != last)
    {
      returnIter = erase(first++);
    }
    return returnIter;
  }
}

namespace detail
{
  template< typename T, typename Compare >
  bool compareLists(const nikonov::List< T > & lhs, const nikonov::List< T > & rhs, Compare cmp)
  {
    assert(lhs.size() == rhs.size());
    nikonov::const_iterator< T > thisIt = lhs.cbegin();
    nikonov::const_iterator< T > anotherIt = rhs.cbegin();
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
