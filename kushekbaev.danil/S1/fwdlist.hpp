#ifndef FWDLIST_HPP
#define FWDLIST_HPP

#include <utility>
#include <initializer_list>
#include <type_traits>
#include "iterator.hpp"

namespace kushekbaev
{
  template< typename T >
  struct FwdList
  {
    template< typename InputIterator >
    using enableIf = std::enable_if_t< std::is_convertible< decltype(*std::declval< InputIterator >()), T >::value >;

    FwdList();
    FwdList(size_t count, const T& value);
    FwdList(std::initializer_list< T > init);
    template< typename InputIterator, typename = enableIf< InputIterator > >
    FwdList(InputIterator first, InputIterator last);
    FwdList(const FwdList& other);
    FwdList(FwdList&& other);
    ~FwdList();

    FwdList& operator=(const FwdList< T >& other);
    FwdList& operator=(FwdList< T >&& other);
    FwdList& operator=(std::initializer_list< T > init);
    bool operator==(const FwdList& other) const noexcept;
    bool operator!=(const FwdList& other) const noexcept;
    bool operator<(const FwdList& other) const noexcept;
    bool operator>(const FwdList& other) const noexcept;
    bool operator<=(const FwdList& other) const noexcept;
    bool operator>=(const FwdList& other) const noexcept;

    Iterator< T > begin() const noexcept;
    Iterator< T > end() const noexcept;

    Iterator< T > cbegin() const noexcept;
    Iterator< T > cend() const noexcept;

    Iterator< T > before_begin() const noexcept;
    Iterator< T > cbefore_begin() const noexcept;

    T& front() noexcept;
    const T& front() const noexcept;
    T& back() noexcept;
    const T& back() const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    template< typename InputIterator, typename = enableIf< InputIterator > >
    void assign(InputIterator first, InputIterator last);
    void assign(size_t size, const T& value);
    void assign(std::initializer_list< T > init);
    void push_front(const T& value);
    void pop_front() noexcept;
    void swap(FwdList& other) noexcept;
    void clear() noexcept;
    Iterator< T > insert_after(Iterator< T > position, const T& value);
    Iterator< T > insert_after(Iterator< T > position, T&& value);
    Iterator< T > insert_after(Iterator< T > position, size_t size, const T& value);
    template< typename InputIterator, typename = enableIf< InputIterator > >
    Iterator< T > insert_after(Iterator< T > position, InputIterator first, InputIterator last);
    Iterator< T > insert_after(Iterator< T > position, std::initializer_list< T > il);
    Iterator< T > erase_after(Iterator< T > position);
    Iterator< T > erase_after(Iterator< T > position, Iterator< T > last);

    void reverse() noexcept;
    void remove(const T& value);
    template< typename Predicate >
    void remove_if(Predicate predicate);
    void splice_after(Iterator< T > position, FwdList& other);
    void splice_after(Iterator< T > position, FwdList&& other);
    void splice_after(Iterator< T > position, FwdList& other, Iterator< T > i);
    void splice_after(Iterator< T > position, FwdList&& other, Iterator< T > i);
    void splice_after(Iterator< T > position, FwdList& other, Iterator< T > first, Iterator< T > last);
    void splice_after(Iterator< T > position, FwdList&& other, Iterator< T > first, Iterator< T > last);

    private:
      Node< T >* fake_;
      size_t size_;
  };

  template< typename T >
  FwdList< T >::FwdList():
    fake_(static_cast< Node< T >* >(std::malloc(sizeof(Node< T >)))),
    size_(0)
  {
    fake_->next_ = fake_;
  }

  template< typename T >
  FwdList< T >::FwdList(size_t count, const T& value):
    FwdList()
  {
    while (count--)
    {
      push_front(value);
    }
  }

  template< typename T >
  FwdList< T >::FwdList(std::initializer_list< T > init):
    FwdList(init.begin(), init.end())
  {}

  template< typename T >
  template< typename InputIterator, typename >
  FwdList< T >::FwdList(InputIterator first, InputIterator last):
    FwdList()
  {
    for (auto it = first; it != last; ++it)
    {
      push_front(*it);
    }
    reverse();
  }

  template< typename T >
  FwdList< T >::FwdList(const FwdList& other):
    FwdList(other.cbegin(), other.cend())
  {}

  template< typename T >
  FwdList< T >::FwdList(FwdList&& other):
    fake_(other.fake_),
    size_(other.size_)
  {
    other.fake_ = static_cast< Node< T > * >(std::malloc(sizeof(Node< T >)));
    other.fake_->next_ = other.fake_;
    other.size_ = 0;
  }

  template< typename T >
  FwdList< T >::~FwdList()
  {
    clear();
    std::free(fake_);
  }

  template< typename T >
  FwdList< T >& FwdList< T >::operator=(const FwdList< T >& other)
  {
    FwdList tmp(other);
    swap(tmp);
    return *this;
  }

  template< typename T >
  FwdList< T >& FwdList< T >::operator=(FwdList< T >&& other)
  {
    clear();
    fake_ = other.fake_;
    size_ = other.size_;
    other.fake_ = nullptr;
    other.size_ = 0;
    return *this;
  }

  template< typename T >
  FwdList< T >& FwdList< T >::operator=(std::initializer_list< T > init)
  {
    assign(init);
    return *this;
  }

  template< typename T >
  bool FwdList< T >::operator==(const FwdList& other) const noexcept
  {
    if (size() != other.size())
    {
      return false;
    }
    auto it1 = cbegin();
    auto it2 = other.cbegin();
    while (it1 != cend())
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
  bool FwdList< T >::operator!=(const FwdList& other) const noexcept
  {
    return !(*this == other);
  }

  template< typename T >
  bool FwdList< T >::operator<(const FwdList& other) const noexcept
  {
    auto it1 = begin();
    auto it2 = other.begin();
    for (; it1 != end() && it2 != other.end(); ++it1, ++it2)
    {
      if (*it1 < *it2)
      {
        return true;
      }
      else if (*it2 < *it1)
      {
        return false;
      }
    }
    return (it1 == end() && it2 != other.end());
  }

  template< typename T >
  bool FwdList< T >::operator>(const FwdList& other) const noexcept
  {
    return other < *this;
  }

  template< typename T >
  bool FwdList< T >::operator<=(const FwdList& other) const noexcept
  {
    return !(*this > other);
  }

  template< typename T >
  bool FwdList< T >::operator>=(const FwdList& other) const noexcept
  {
    return !(*this < other);
  }

  template< typename T >
  Iterator< T > FwdList< T >::begin() const noexcept
  {
    return Iterator< T >(fake_->next_);
  }

  template< typename T >
  Iterator< T > FwdList< T >::end() const noexcept
  {
    return Iterator< T >(fake_);
  }

  template< typename T >
  Iterator< T > FwdList< T >::cbegin() const noexcept
  {
    return Iterator< T >(fake_->next_);
  }

  template< typename T >
  Iterator< T > FwdList< T >::cend() const noexcept
  {
    return Iterator< T >(fake_);
  }

  template< typename T >
  Iterator< T > FwdList<T>::before_begin() const noexcept
  {
    return Iterator< T >(fake_);
  }

  template< typename T >
  Iterator< T > FwdList<T>::cbefore_begin() const noexcept
  {
    return Iterator< T >(fake_);
  }

  template< typename T >
  T& FwdList< T >::front() noexcept
  {
    return const_cast< T& >(static_cast< const FwdList& >(*this).front());
  }

  template< typename T >
  const T& FwdList< T >::front() const noexcept
  {
    assert(!empty());
    return fake_->next_->data_;
  }

  template< typename T >
  T& FwdList< T >::back() noexcept
  {
    return const_cast< T& >(static_cast< const FwdList& >(*this).back());
  }

  template< typename T >
  const T& FwdList< T >::back() const noexcept
  {
    assert(!empty());
    Node< T >* current = fake_;
    while (current->next_ != fake_)
    {
      current = current->next_;
    }
    return current->data_;
  }

  template< typename T >
  size_t FwdList< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  bool FwdList< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  template< typename InputIterator, typename >
  void FwdList< T >::assign(InputIterator first, InputIterator last)
  {
    while (!empty())
    {
      pop_front();
    }
    for (auto it = first; it != last; ++it)
    {
      push_front(*it);
    }
    reverse();
  }

  template< typename T >
  void FwdList< T >::assign(size_t size, const T& value)
  {
    while (!empty())
    {
      pop_front();
    }
    for (size_t i = 0; i < size; ++i)
    {
      push_front(value);
    }
    reverse();
  }

  template< typename T >
  void FwdList< T >::assign(std::initializer_list< T > init)
  {
    assign(init.begin(), init.end());
  }

  template< typename T >
  void FwdList< T >::push_front(const T& value)
  {
    Node< T >* newNode = new Node< T >(value, fake_->next_);
    fake_->next_ = newNode;
    ++size_;
  }

  template< typename T >
  void FwdList< T >::pop_front() noexcept
  {
    assert(!empty());
    Node< T >* todelete = fake_->next_;
    fake_->next_ = todelete->next_;
    delete todelete;
    --size_;
  }

  template< typename T >
  void FwdList< T >::swap(FwdList& other) noexcept
  {
    std::swap(fake_, other.fake_);
    std::swap(size_, other.size_);
  }

  template< typename T >
  void FwdList< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_front();
    }
  }

  template< typename T >
  Iterator< T > FwdList< T >::insert_after(Iterator< T > position, const T& value)
  {
    Node< T >* newNode = new Node< T >(value);
    Node< T >* posNode = position.node_;
    newNode->next_ = posNode->next_;
    posNode->next_ = newNode;
    ++size_;
    return Iterator< T >(newNode);
  }

  template< typename T >
  Iterator< T > FwdList< T >::insert_after(Iterator< T > position, T&& value)
  {
    return insert_after(position, value);
  }

  template< typename T >
  Iterator< T > FwdList< T >::insert_after(Iterator< T > position, size_t size, const T& value)
  {
    for (size_t i = 0; i < size - 1; ++i)
    {
      insert_after(position, value);
    }
    Iterator< T > tmp = insert_after(position, value);
    return Iterator< T >(tmp);
  }

  template< typename T >
  template< typename InputIterator, typename >
  Iterator< T > FwdList< T >::insert_after(Iterator< T > position, InputIterator first, InputIterator last)
  {
    Iterator< T > tmp = Iterator< T >(position.node_);
    for (auto it = first; it != last; ++it)
    {
      tmp = insert_after(position, *it);
      ++position;
    }
    return tmp;
  }

  template< typename T >
  Iterator< T > FwdList< T >::insert_after(Iterator< T > position, std::initializer_list< T > il)
  {
    return insert_after(position, il.begin(), il.end());
  }

  template< typename T >
  Iterator< T > FwdList< T >::erase_after(Iterator< T > position)
  {
    assert(!empty());
    Node< T >* todelete = position.node_->next;
    if (todelete == fake_)
    {
      return end();
    }
    Node< T >* next = todelete->next_;
    position.node_->next_ = next;
    delete todelete;
    --size_;
    return Iterator< T >(next);
  }

  template < typename T >
  Iterator< T > FwdList< T >::erase_after(Iterator< T > position, Iterator< T > last)
  {
    assert(!empty());
    while (position.node_->next_ != last.node_ && position.node_->next_ != fake_)
    {
      erase_after(position);
    }
    return Iterator< T >(last.node_);
  }

  template< typename T >
  void FwdList< T >::reverse() noexcept
  {
    if (empty() || size() == 1)
    {
      return;
    }
    Node< T >* current = fake_->next_;
    Node< T >* prev = fake_;
    Node< T >* next = nullptr;
    while (current != fake_)
    {
      next = current->next_;
      current->next_ = prev;
      prev = current;
      current = next;
    }
    fake_->next_ = prev;
  }

  template< typename T >
  void FwdList< T >::remove(const T& value)
  {
    remove_if([&value] (const T& elem)
      {
        return elem == value;
      }
    );
  }

  template< typename T >
  template< typename Predicate >
  void FwdList< T >::remove_if(Predicate predicate)
  {
    Node< T >* current = fake_;
    while (current->next_ != fake_)
    {
      if (predicate(current->next_->data_))
      {
        Node< T >* todelete = current->next_;
        current->next_ = current->next_->next_;
        delete todelete;
        --size_;
      }
      else
      {
        current = current->next_;
      }
    }
  }

  template< typename T >
  void FwdList< T >::splice_after(Iterator< T > position, FwdList& other)
  {
    assert(position != end());
    auto otherFirst = other.begin().node_;
    assert(otherFirst != nullptr);
    auto otherLast = otherFirst;
    while (otherLast->next_ != other.fake_)
    {
      otherLast = otherLast->next_;
    }
    otherLast->next_ = position.node_->next_;
    position.node_->next_ = otherFirst;
    size_ += other.size_;
    other.size_ = 0;
    other.fake_->next_ = other.fake_;
  }

  template< typename T >
  void FwdList< T >::splice_after(Iterator< T > position, FwdList&& other)
  {
    splice_after(position, other);
  }

  template< typename T >
  void FwdList< T >::splice_after(Iterator< T > position, FwdList& other, Iterator< T > it)
  {
    assert(position != end());
    assert(it != other.end());
    auto itPrev = other.begin().node_;
    while (itPrev->next_ != it.node_)
    {
      itPrev = itPrev->next_;
    }
    if (itPrev != nullptr)
    {
      itPrev->next_ = it.node_->next_;
    }
    else
    {
      other.begin().node_ = it.node_->next_;
    }
    it.node_->next_ = position.node_->next_;
    position.node_->next_ = it.node_;
    ++size_;
    --other.size_;
  }

  template< typename T >
  void FwdList< T >::splice_after(Iterator< T > position, FwdList&& other, Iterator< T > it)
  {
    splice_after(position, other);
  }

  template< typename T >
  void FwdList< T >::splice_after(Iterator< T > position, FwdList& other, Iterator< T > first, Iterator< T > last)
  {
    assert(position != end());
    assert(last != other.end());
    if (first == last)
    {
      splice_after(position, other, first);
    }
    size_t deltaSize = 0;
    auto lastPrev = first.node_;
    while (lastPrev->next_ != last)
    {
      lastPrev = lastPrev->next_;
      ++deltaSize;
    }
    lastPrev->next = position.node_->next_;
    position.node_->next_ = first.node_->next_;
    first.node_->next = last;
    size_ += deltaSize;
    other.size_ -= deltaSize;
  }

  template< typename T >
  void FwdList< T >::splice_after(Iterator< T > position, FwdList&& other, Iterator< T > first, Iterator< T > last)
  {
    splice_after(position, other, first, last);
  }
}

#endif
