#ifndef LIST_HPP
#define LIST_HPP
#include <utility>
#include "iterator.hpp"
#include "node.hpp"

namespace demehin
{
  template< typename T >
  struct List
  {
  public:
    using Iter = ListIterator< T, false >;
    using cIter = ListIterator< T, true >;

    List();
    List(const List< T >&);
    List(List< T >&&);

    List(size_t, const T&);
    explicit List(std::initializer_list< T >);
    template< typename InputIt >
    List(InputIt, InputIt);

    ~List();

    List< T >& operator=(const List< T >&);
    bool operator==(const List< T >&) const noexcept;
    bool operator!=(const List< T >&) const noexcept;
    bool operator<(const List< T >&) const noexcept;
    bool operator<=(const List< T >&) const noexcept;
    bool operator>(const List< T >&) const noexcept;
    bool operator>=(const List< T >&) const noexcept;

    Iter begin() const noexcept;
    Iter end() const noexcept;

    cIter cbegin() const noexcept;
    cIter cend() const noexcept;

    size_t size() const noexcept;

    void pop_front() noexcept;
    void pop_back() noexcept;

    void push_front(const T&);
    void push_back(const T&);

    const T& front() const noexcept;
    T& front() noexcept;
    const T& back() const noexcept;
    T& back() noexcept;

    bool empty() const noexcept;

    void clear() noexcept;

    void swap(List&) noexcept;

    void splice(cIter pos, List< T >&) noexcept;
    void splice(cIter pos, List< T >&&) noexcept;
    void splice(cIter pos, List< T >&, cIter it) noexcept;
    void splice(cIter pos, List< T >&&, cIter it) noexcept;
    void splice(cIter pos, List< T >&, cIter first, cIter last) noexcept;
    void splice(cIter pos, List< T >&&, cIter first, cIter last) noexcept;

    void fill(Iter first, Iter last, const T&) noexcept;

    void remove(const T&) noexcept;
    template< typename UnaryPredicate >
    void removeIf(UnaryPredicate) noexcept;

    void assign(size_t, const T&);
    template< typename InputIt >
    void assign(InputIt, InputIt);
    void assign(std::initializer_list < T >);

    Iter erase(cIter pos) noexcept;
    Iter erase(cIter first, cIter last) noexcept;

    Iter insert(cIter, const T&);
    Iter insert(cIter, T&&);
    Iter insert(cIter, size_t, const T&);
    template< typename InputIt >
    Iter insert(cIter, InputIt, InputIt);
    Iter insert(cIter, std::initializer_list< T >);

    void reverse() noexcept;

  private:
    using Node = demehin::Node< T >;

    Node* fake_;
    Node* tail_;
    size_t size_;

  };

  template< typename T >
  using Node = demehin::Node< T >;

  template< typename T >
  List< T >::List():
    fake_(reinterpret_cast< Node* >(new char[sizeof(Node)])),
    tail_(fake_),
    size_(0)
  {
    fake_->next = fake_;
    fake_->prev = fake_;
  }

  template< typename T >
  List< T >::List(size_t count, const T& value):
    List()
  {
    assign(count, value);
  }

  template< typename T >
  List< T >::List(std::initializer_list< T > ilist):
    List()
  {
    assign(ilist);
  }

  template< typename T >
  template< typename InputIt >
  List< T >::List(InputIt first, InputIt last):
    List()
  {
    assign(first, last);
  }

  template< typename T >
  List< T >::~List()
  {
    clear();
    delete[] reinterpret_cast< char* >(fake_);
  }

  template< typename T >
  List< T >& List< T >::operator=(const List< T >& other)
  {
    assign(other.begin(), other.end());
    return *this;
  }

  template< typename T >
  bool List< T >::operator==(const List< T >& rhs) const noexcept
  {
    if (size() != rhs.size())
    {
      return false;
    }

    auto it1 = begin();
    auto it2 = rhs.begin();
    while (it1 != end())
    {
      if (*it1 != *it2)
      {
        return false;
      }
      ++it1;
      ++it2;
    }
    return true;
  }

  template< typename T >
  bool List< T >::operator!=(const List< T >& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< typename T >
  bool List< T >::operator>(const List< T >& rhs) const noexcept
  {
    auto it1 = begin();
    auto it2 = rhs.begin();
    while (it1 != end() && it2 != rhs.end())
    {
      if (*it1 > *it2)
      {
        return true;
      }
      if (*it1 < *it2)
      {
        return false;
      }
      it1++;
      it2++;
    }

    return size() > rhs.size();
  }

  template< typename T >
  bool List< T >::operator<=(const List< T >& rhs) const noexcept
  {
    return !(*this > rhs);
  }

  template< typename T >
  bool List< T >::operator<(const List< T >& rhs) const noexcept
  {
    return rhs > *this;
  }

  template< typename T >
  bool List< T >::operator>=(const List< T >& rhs) const noexcept
  {
    return !(*this < rhs);
  }

  template< typename T >
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_back();
    }
  }

  template< typename T >
  List< T >::List(const List< T >& rhs):
    List()
  {
    Node* current = rhs.fake_->next;
    while (current != rhs.fake_)
    {
      push_back(current->data);
      current = current->next;
    }
  }

  template< typename T >
  List< T >::List(List< T >&& rhs):
    fake_(std::exchange(rhs.fake_, nullptr)),
    tail_(std::exchange(rhs.tail_, nullptr)),
    size_(std::exchange(rhs.size_, 0))
  {}

  template< typename T >
  typename List< T >::Iter List< T >::begin() const noexcept
  {
    return Iter(fake_->next);
  }

  template< typename T >
  typename List< T >::Iter List< T >::end() const noexcept
  {
    return Iter(fake_);
  }

  template< typename T >
  typename List< T >::cIter List< T >::cbegin() const noexcept
  {
    return cIter(fake_->next);
  }

  template< typename T >
  typename List< T >::cIter List< T >::cend() const noexcept
  {
    return cIter(fake_);
  }

  template< typename T >
  void List< T >::pop_front() noexcept
  {
    erase(cbegin());
  }

  template< typename T >
  void List< T >::pop_back() noexcept
  {
    erase(cIter(tail_));
  }

  template< typename T >
  void List< T >::push_back(const T& data)
  {
    insert(cend(), data);
  }

  template< typename T >
  void List< T >::push_front(const T& data)
  {
    insert(cbegin(), data);
  }

  template< typename T >
  const T& List< T >::front() const noexcept
  {
    return fake_->next->data;
  }

  template< typename T >
  T& List< T >::front() noexcept
  {
    return fake_->next->data;
  }

  template< typename T >
  const T& List< T >::back() const noexcept
  {
    return tail_->data;
  }

  template< typename T >
  T& List< T >::back() noexcept
  {
    return tail_->data;
  }

  template< typename T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  void List< T >::swap(List< T >& other) noexcept
  {
    std::swap(fake_, other.fake_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }

  template< typename T >
  void List< T >::fill(Iter first, Iter last, const T& value) noexcept
  {
    for (; first != last; ++first)
    {
      *first = value;
    }
  }

  template < typename T >
  void List< T >::splice(cIter pos, List< T >& other) noexcept
  {
    splice(pos, other, other.cbegin(), other.cend());
  }

  template < typename T >
  void List< T >::splice(cIter pos, List< T >&& other) noexcept
  {
    splice(pos, other);
  }

  template < typename T >
  void List< T >::splice(cIter pos, List< T >& other, cIter it) noexcept
  {
    auto it2 = it;
    splice(pos, other, it, ++it2);
  }

  template < typename T >
  void List< T >::splice(cIter pos, List< T >&& other, cIter it) noexcept
  {
    splice(pos, other, it);
  }

  template < typename T >
  void List< T >::splice(cIter pos, List< T >& other, cIter first, cIter last) noexcept
  {
    if (first == last || other.empty() || &other == this)
    {
        return;
    }

    Node* posNode = pos.getNode();
    Node* firstNode = first.getNode();
    Node* lastNode = (last.getNode() != nullptr) ? last.getNode()->prev : other.tail_;

    size_t count = 0;
    for (auto it = first; it != last; it++)
    {
      count++;
    }

    if (firstNode->prev)
    {
      firstNode->prev->next = last.getNode();
    }
    else
    {
      other.fake_->next = last.getNode();
    }

    if (last.getNode())
    {
      last.getNode()->prev = firstNode->prev;
    }
    else
    {
      other.tail_ = firstNode->prev;
    }

    firstNode->prev = posNode->prev;
    lastNode->next = posNode;

    if (posNode->prev)
    {
      posNode->prev->next = firstNode;
    }
    else
    {
      fake_->next = firstNode;
    }
    posNode->prev = lastNode;

    if (posNode == fake_->next)
    {
      tail_ = lastNode;
    }

    other.size_ -= count;
    size_ += count;
  }

  template < typename T >
  void List< T >::splice(cIter pos, List< T >&& other, cIter first, cIter last) noexcept
  {
    splice(pos, other, first, last);
  }

  template< typename T >
  void List< T >::remove(const T& value) noexcept
  {
    for (auto it = cbegin(); it != cend();)
    {
      if (*it == value)
      {
        it = erase(it);
      }
      else
      {
        it++;
      }
    }
  }

  template< typename T >
  template< typename UnaryPredicate >
  void List< T >::removeIf(UnaryPredicate p) noexcept
  {
    for (auto it = cbegin(); it != cend();)
    {
      if (p(*it))
      {
        it = erase(it);
      }
      else
      {
        it++;
      }
    }
  }

  template< typename T >
  void List< T >::assign(size_t count, const T& value)
  {
    List< T > temp;
    for (size_t i = 0; i < count; i++)
    {
      temp.push_back(value);
    }
    swap(temp);
  }

  template< typename T >
  template< typename InputIt >
  void List< T >::assign(InputIt first, InputIt last)
  {
    List< T > temp;
    for (auto it = first; it != last; it++)
    {
      temp.push_back(*it);
    }
    swap(temp);
  }

  template< typename T >
  void List< T >::assign(std::initializer_list< T > ilist)
  {
    assign(ilist.begin(), ilist.end());
  }

  template< typename T >
  typename List< T >::Iter List< T >::erase(cIter pos) noexcept
  {
    Node* todelete = pos.getNode();
    if (todelete == fake_)
    {
      return end();
    }
    Iter toreturn(todelete->next);
    todelete->prev->next = todelete->next;
    todelete->next->prev = todelete->prev;

    if (todelete == tail_)
    {
      tail_ = (todelete->prev != fake_) ? todelete->prev : fake_;
    }
    if (todelete == fake_->next)
    {
      fake_->next = todelete->next;
    }
    size_--;
    delete todelete;
    return toreturn;
  }

  template< typename T >
  typename List< T >::Iter List< T >::erase(cIter first, cIter last) noexcept
  {
    Iter toreturn;
    for (auto it = first; it != last; it++)
    {
      toreturn = erase(it);
    }
    return toreturn;
  }

  template< typename T >
  typename List< T >::Iter List< T >::insert(cIter pos, const T& value)
  {
    Node* posNode = pos.getNode() ? pos.getNode() : fake_;
    Node* newNode = new Node(value, posNode->prev, posNode);
    posNode->prev->next = newNode;
    posNode->prev = newNode;

    if (posNode == fake_)
    {
      tail_ = newNode;
    }
    if (newNode->prev == fake_)
    {
      fake_->next = newNode;
    }
    size_++;
    return Iter(newNode);
  }

  template< typename T >
  typename List< T >::Iter List< T >::insert(cIter pos, T&& value)
  {
    T temp_val = std::move(value);
    return insert(pos, temp_val);
  }

  template< typename T >
  typename List< T >::Iter List< T >::insert(cIter pos, size_t count, const T& value)
  {
    if (count == 0)
    {
      return Iter(pos.getNode());
    }

    Iter toreturn = insert(pos, value);
    if (count != 1)
    {
      List< T > values(--count, value);
      splice(pos, values);
    }
    return toreturn;
  }

  template< typename T >
  template< typename InputIt >
  typename List< T >::Iter List< T >::insert(cIter pos, InputIt first, InputIt last)
  {
    if (first == last)
    {
      return Iter(pos.getNode());
    }

    Iter result = insert(pos, *first);
    List< T > tempList(++first, last);
    splice(pos, tempList);
    return result;
  }

  template< typename T >
  typename List< T >::Iter List< T >::insert(cIter pos, std::initializer_list< T > ilist)
  {
    if (ilist.size() == 0)
    {
      return Iter(pos.getNode());
    }

    return insert(pos, ilist.begin(), ilist.end());
  }

  template< typename T >
  void List< T >::reverse() noexcept
  {
    if (size() <= 1)
    {
      return;
    }

    Node* current = fake_->next;
    Node* prev = fake_;
    Node* next = nullptr;
    tail_ = fake_->next;
    while (current != fake_)
    {
      next = current->next;
      current->next = prev;
      current->prev = next;
      prev = current;
      current = next;
    }
    fake_->next = prev;
    fake_->prev = tail_;
    tail_->next = fake_;
  }
}

#endif

