#ifndef LIST_HPP
#define LIST_HPP

#include <functional>
#include <initializer_list>
#include "listNode.hpp"
#include "listIterator.hpp"
#include "listCIterator.hpp"

namespace duhanina
{
  template< typename T >
  class IteratorList;
  template< typename T >
  class ConstIteratorList;

  template < typename T >
  class List
  {
  public:
    List();
    List(size_t count, const T& value);
    List(std::initializer_list< T > init);
    ~List();

    List(const List< T >& list);
    List(List&& other);
    List< T >& operator=(const List< T >& list);
    List< T >& operator=(List< T >&& other) noexcept;

    IteratorList< T > begin() const noexcept;
    IteratorList< T > end() const noexcept;
    ConstIteratorList< T > cbegin() const noexcept;
    ConstIteratorList< T > cend() const noexcept;

    T& front() noexcept;
    T& back() noexcept;
    const T& front() const noexcept;
    const T& back() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void push_front(const T& value);
    void push_back(const T& value);
    void pop_front() noexcept;
    void pop_back() noexcept;
    void clear() noexcept;
    void swap(List& other) noexcept;
    void remove(const T& value) noexcept;
    template < typename Predicate >
    void remove_if(Predicate pred) noexcept;
    void assign(size_t count, const T& value);

    void splice(IteratorList< T > pos, List< T >&) noexcept;
    void splice(IteratorList< T > pos, List< T >&&) noexcept;
    void splice(IteratorList< T > pos, List< T >&, IteratorList< T > it) noexcept;
    void splice(IteratorList< T > pos, List< T >&&, IteratorList< T > it) noexcept;
    void splice(IteratorList< T > pos, List< T >&, IteratorList< T > first, IteratorList< T > last) noexcept;
    void splice(IteratorList< T > pos, List< T >&&, IteratorList< T > first, IteratorList< T > last) noexcept;

    bool operator==(const List< T >&) const noexcept;
    bool operator!=(const List< T >&) const noexcept;

  private:
    NodeList< T >* fake_;
    size_t listSize_;
  };

  template < typename T >
  List< T >::List():
    fake_(static_cast< NodeList< T >* >(operator new(sizeof(NodeList< T >)))),
    listSize_(0)
  {
    fake_->next_ = fake_;
  }

  template < typename T >
  List< T >::List(size_t count, const T& value):
    List()
  {
    for (size_t i = 0; i < count; ++i)
    {
      push_back(value);
    }
  }

  template < typename T >
  List< T >::~List()
  {
    clear();
    operator delete(fake_);
  }

  template< typename T >
  List< T >::List(const List< T >& list):
    List()
  {
    for (ConstIteratorList< T > it = list.cbegin(); it != list.cend(); ++it)
    {
      push_back(*it);
    }
  }

  template < typename T >
  List< T >::List(List&& other):
    fake_(other.fake_),
    listSize_(other.listSize_)
  {
    other.fake_ = nullptr;
    other.listSize_ = 0;
  }

  template < typename T >
  List< T >& List< T >::operator=(const List< T >& other)
  {
    if (this != std::addressof(other))
    {
      List< T > temp(other);
      swap(temp);
    }
    return *this;
  }

  template < typename T >
  List< T >& List< T >::operator=(List< T >&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      List< T > temp(std::move(other));
      swap(temp);
    }
    return *this;
  }

  template < typename T >
  IteratorList< T > List< T >::begin() const noexcept
  {
    return IteratorList< T >(fake_->next_);
  }

  template < typename T >
  IteratorList< T > List< T >::end() const noexcept
  {
    return IteratorList< T >(fake_);
  }

  template < typename T >
  ConstIteratorList< T > List< T >::cbegin() const noexcept
  {
    return ConstIteratorList< T >(fake_->next_);
  }

  template < typename T >
  ConstIteratorList< T > List< T >::cend() const noexcept
  {
    return ConstIteratorList< T >(fake_);
  }

  template < typename T >
  T& List< T >::front() noexcept
  {
    return fake_->next_->data_;
  }

  template < typename T >
  const T& List< T >::front() const noexcept
  {
    return fake_->next_->data_;
  }

  template < typename T >
  T& List< T >::back() noexcept
  {
    NodeList< T >* current = fake_->next_;
    while (current->next_ != fake_)
    {
      current = current->next_;
    }
    return current->data_;
  }

  template < typename T >
  const T& List< T >::back() const noexcept
  {
    NodeList< T >* current = fake_->next_;
    while (current->next_ != fake_)
    {
      current = current->next_;
    }
    return current->data_;
  }

  template < typename T >
  bool List< T >::empty() const noexcept
  {
    return listSize_ == 0;
  }

  template < typename T >
  size_t List< T >::size() const noexcept
  {
    return listSize_;
  }

  template < typename T >
  void List< T >::push_front(const T& value)
  {
    NodeList< T >* newNode = new NodeList< T >{ value, nullptr };
    newNode->next_ = fake_->next_;
    fake_->next_ = newNode;
    ++listSize_;
  }

  template < typename T >
  void List< T >::push_back(const T& value)
  {
    NodeList< T >* newNode = new NodeList< T >{ value, nullptr };
    NodeList< T >* current = fake_;
    while (current->next_ != fake_)
    {
      current = current->next_;
    }
    current->next_ = newNode;
    newNode->next_ = fake_;
    ++listSize_;
  }

  template < typename T >
  void List< T >::pop_front() noexcept
  {
    NodeList< T >* temp = fake_->next_;
    fake_->next_ = temp->next_;
    delete temp;
    --listSize_;
  }

  template < typename T >
  void List< T >::pop_back() noexcept
  {
    NodeList< T >* current = fake_;
    while (current->next_->next_ != fake_)
    {
      current = current->next_;
    }
    delete current->next_;
    current->next_ = fake_;
    --listSize_;
  }

  template < typename T >
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_front();
    }
  }

  template < typename T >
  void List< T >::swap(List& other) noexcept
  {
    std::swap(fake_, other.fake_);
    std::swap(listSize_, other.listSize_);
  }

  template < typename T >
  void List< T >::remove(const T& value) noexcept
  {
    remove_if(std::bind(std::equal_to< T >(), std::placeholders::_1, value));
  }

  template < typename T >
  template <typename Predicate>
  void List< T >::remove_if(Predicate pred) noexcept
  {
    NodeList< T >* current = fake_->next_;
    NodeList< T >* prev = fake_;
    while (current != fake_)
    {
      if (pred(current->data_))
      {
        prev->next_ = current->next_;
        delete current;
        current = prev->next_;
        --listSize_;
      }
      else
      {
        prev = current;
        current = current->next_;
      }
    }
  }

  template < typename T >
  void List< T >::assign(size_t count, const T& value)
  {
    List< T > temp;
    for (size_t i = 0; i < count; ++i)
    {
      temp.push_back(value);
    }
    swap(temp);
  }

  template< typename T >
  List< T >::List(std::initializer_list< T > init):
    List()
  {
    for (const T& value: init)
    {
      push_back(value);
    }
  }

  template < typename T >
  void List< T >::splice(IteratorList< T > pos, List< T >& other) noexcept
  {
    splice(pos, other, other.begin(), other.end());
  }

  template < typename T >
  void List< T >::splice(IteratorList< T > pos, List< T >&& other) noexcept
  {
    splice(pos, other);
  }

  template < typename T >
  void List< T >::splice(IteratorList< T > pos, List< T >& other, IteratorList< T > it) noexcept
  {
    auto it1 = it;
    splice(pos, other, it, ++it1);
  }

  template < typename T >
  void List< T >::splice(IteratorList< T > pos, List< T >&& other, IteratorList< T > it) noexcept
  {
    splice(pos, other, it);
  }

  template < typename T >
  void List< T >::splice(IteratorList< T > pos, List< T >& other, IteratorList< T > first, IteratorList< T > last) noexcept
  {
    if (other.empty())
    {
      return;
    }
    if (first == last || first.node_ == other.fake_)
    {
      return;
    }
    size_t count = 0;
    for (auto it = first; it != last; ++it)
    {
      ++count;
    }
    NodeList< T >* prevOther = other.fake_;
    while (prevOther->next_ != first.node_)
    {
      prevOther = prevOther->next_;
    }
    NodeList< T >* lastOther = first.node_;
    while (lastOther->next_ != last.node_)
    {
      lastOther = lastOther->next_;
    }
    prevOther->next_ = last.node_;
    NodeList< T >* prevPos = pos.node_;
    NodeList< T >* nextPos = prevPos->next_;
    prevPos->next_ = first.node_;
    lastOther->next_ = nextPos;
    listSize_ += count;
    other.listSize_ -= count;
  }

  template < typename T >
  void List< T >::splice(IteratorList< T > pos, List< T >&& other, IteratorList< T > first, IteratorList< T > last) noexcept
  {
    splice(pos, other, first, last);
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
}

#endif
