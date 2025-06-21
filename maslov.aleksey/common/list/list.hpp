#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <initializer_list>
#include "node.hpp"
#include "iterator.hpp"

namespace maslov::detail
{
  template< typename T >
  struct IsEqual
  {
    IsEqual(const T & value);
    bool operator()(const T & element) const;
   private:
    const T & value_;
  };

  template< typename T >
  IsEqual< T >::IsEqual(const T & value):
    value_(value)
  {}

  template< typename T >
  bool IsEqual< T >::operator()(const T & element) const
  {
    return element == value_;
  }
}

namespace maslov
{
  template< typename T >
  struct FwdList
  {
    using cIterator = FwdListConstIterator< T >;
    using iterator = FwdListIterator< T >;
    template< typename InputIterator >
    using enableIf = std::enable_if_t< std::is_convertible< decltype(*std::declval< InputIterator >()),
        T >::value >;

    FwdList();
    FwdList(size_t k, const T & value);
    template< typename InputIterator, typename = enableIf< InputIterator > >
    FwdList(InputIterator first, InputIterator last);
    FwdList(std::initializer_list< T > il);
    FwdList(const FwdList< T > & rhs);
    FwdList(FwdList< T > && rhs) noexcept;
    ~FwdList();

    FwdList< T > & operator=(const FwdList< T > & rhs);
    FwdList< T > & operator=(FwdList< T > && rhs) noexcept;

    bool operator==(const FwdList< T > & rhs) const;
    bool operator!=(const FwdList< T >& rhs) const;
    bool operator<(const FwdList< T > & rhs) const noexcept;
    bool operator>(const FwdList< T > & rhs) const noexcept;
    bool operator<=(const FwdList< T > & rhs) const noexcept;
    bool operator>=(const FwdList< T > & rhs) const noexcept;

    iterator begin() noexcept;
    cIterator begin() const noexcept;
    cIterator cbegin() const noexcept;
    iterator end() noexcept;
    cIterator end() const noexcept;
    cIterator cend() const noexcept;
    iterator beforeBegin() noexcept;
    cIterator beforeBegin() const noexcept;
    cIterator cbeforeBegin() const noexcept;

    T & front();
    const T & front() const;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void pushFront(const T & value);
    void popFront();
    void swap(FwdList< T > & rhs) noexcept;
    void clear() noexcept;
    template< typename InputIterator, typename = enableIf< InputIterator > >
    void assign(InputIterator first, InputIterator last);
    void assign(size_t n, const T & val);
    void assign(std::initializer_list< T > il);
    iterator eraseAfter(cIterator position);
    iterator eraseAfter(cIterator position, cIterator last);
    iterator insertAfter(cIterator position, const T & val);
    iterator insertAfter(cIterator position, T && val);
    iterator insertAfter(cIterator position, size_t n, const T & val);
    template< typename InputIterator, typename = enableIf< InputIterator > >
    iterator insertAfter(cIterator position, InputIterator first, InputIterator last);
    iterator insertAfter(cIterator position, std::initializer_list< T > il);

    void reverse() noexcept;
    void remove(const T & value);
    template< typename Predicate >
    void removeIf(Predicate pred);
    void spliceAfter(cIterator position, FwdList< T > & fwdlst);
    void spliceAfter(cIterator position, FwdList< T > && fwdlst);
    void spliceAfter(cIterator position, FwdList< T > & fwdlst, cIterator i);
    void spliceAfter(cIterator position, FwdList< T > && fwdlst, cIterator i);
    void spliceAfter(cIterator position, FwdList< T > & fwdlst, cIterator first, cIterator last);
    void spliceAfter(cIterator position, FwdList< T > && fwdlst, cIterator first, cIterator last);
   private:
    FwdListNode< T > * fake_;
    size_t size_;
  };

  template< typename T >
  FwdList< T >::FwdList():
    fake_(new FwdListNode< T >{T(), nullptr}),
    size_(0)
  {
    fake_->next = fake_;
  }

  template< typename T >
  FwdList< T >::FwdList(size_t k, const T & value):
    FwdList()
  {
    for (size_t i = 0; i < k; ++i)
    {
      pushFront(value);
    }
  }

  template< typename T >
  template< typename InputIterator, typename >
  FwdList< T >::FwdList(InputIterator first, InputIterator last):
    FwdList()
  {
    for (auto it = first; it != last; ++it)
    {
      pushFront(*it);
    }
    reverse();
  }

  template< typename T >
  FwdList< T >::FwdList(std::initializer_list< T > il):
    FwdList(il.begin(), il.end())
  {}

  template< typename T >
  FwdList< T >::~FwdList()
  {
    clear();
    delete fake_;
  }

  template< typename T >
  FwdList< T >::FwdList(const FwdList< T > & rhs):
    FwdList()
  {
    fake_->next = fake_;
    if (!rhs.empty())
    {
      FwdListNode< T > * current = rhs.fake_->next;
      while (current != rhs.fake_)
      {
        pushFront(current->data);
        current = current->next;
      }
      reverse();
    }
  }

  template< typename T >
  FwdList< T >::FwdList(FwdList< T > && rhs) noexcept:
    fake_(rhs.fake_),
    size_(rhs.size_)
  {
    rhs.fake_ = nullptr;
    rhs.size_ = 0;
  }

  template< typename T >
  FwdList< T > & FwdList< T >::operator=(const FwdList< T > & rhs)
  {
    if (this != std::addressof(rhs))
    {
      FwdList temp(rhs);
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  FwdList< T > & FwdList< T >::operator=(FwdList< T > && rhs) noexcept
  {
    if (this != std::addressof(rhs))
    {
      clear();
      delete fake_;
      fake_ = rhs.fake_;
      size_ = rhs.size_;
      rhs.fake_ = nullptr;
      rhs.size_ = 0;
    }
    return *this;
  }

  template< typename T >
  T & FwdList< T >::front()
  {
    return const_cast< T & >(static_cast< const FwdList & >(*this).front());
  }

  template< typename T >
  const T & FwdList< T >::front() const
  {
    if (empty())
    {
      throw std::runtime_error("ERROR:: empty list");
    }
    return fake_->next->data;
  }

  template< typename T >
  bool FwdList< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  size_t FwdList< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  void FwdList< T >::swap(FwdList< T > & rhs) noexcept
  {
    std::swap(fake_, rhs.fake_);
    std::swap(size_, rhs.size_);
  }

  template< typename T >
  void FwdList< T >::pushFront(const T & value)
  {
    FwdListNode< T > * newNode = new FwdListNode< T >{value, nullptr};
    if (empty())
    {
      newNode->next = fake_;
      fake_->next = newNode;
    }
    else
    {
      newNode->next = fake_->next;
      fake_->next = newNode;
    }
    size_++;
  }

  template< typename T >
  void FwdList< T >::popFront()
  {
    if (empty())
    {
      throw std::runtime_error("ERROR: empty list");
    }
    FwdListNode< T > * node = fake_->next;
    fake_->next = node->next;
    delete node;
    size_--;
  }

  template< typename T >
  void FwdList< T >::clear() noexcept
  {
    while (!empty())
    {
      popFront();
    }
  }

  template< typename T >
  FwdListIterator< T > FwdList< T >::begin() noexcept
  {
    return FwdListIterator< T >(fake_->next);
  }

  template< typename T >
  FwdListConstIterator< T > FwdList< T >::begin() const noexcept
  {
    return FwdListConstIterator< T >(fake_->next);
  }

  template< typename T >
  FwdListConstIterator< T > FwdList< T >::cbegin() const noexcept
  {
    return FwdListConstIterator< T >(fake_->next);
  }

  template< typename T >
  FwdListIterator< T > FwdList< T >::end() noexcept
  {
    return FwdListIterator< T >(fake_);
  }

  template< typename T >
  FwdListConstIterator< T > FwdList< T >::end() const noexcept
  {
    return FwdListConstIterator< T >(fake_);
  }

  template< typename T >
  FwdListConstIterator< T > FwdList< T >::cend() const noexcept
  {
    return FwdListConstIterator< T >(fake_);
  }

  template< typename T >
  FwdListIterator< T > FwdList< T >::beforeBegin() noexcept
  {
    return FwdListIterator< T >(fake_);
  }

  template< typename T >
  FwdListConstIterator< T > FwdList< T >::beforeBegin() const noexcept
  {
    return FwdListConstIterator< T >(fake_);
  }
  template< typename T >
  FwdListConstIterator< T > FwdList< T >::cbeforeBegin() const noexcept
  {
    return FwdListConstIterator< T >(fake_);
  }

  template< typename T >
  void FwdList< T >::remove(const T & value)
  {
    removeIf(detail::IsEqual< T >(value));
  }

  template< typename T >
  template< typename Predicate >
  void FwdList< T >::removeIf(Predicate pred)
  {
    FwdListNode< T > * current = fake_->next;
    FwdListNode< T > * prev = fake_;
    while (current != fake_)
    {
      if (pred(current->data))
      {
        prev->next = current->next;
        delete current;
        current = prev->next;
        size_--;
      }
      else
      {
        prev = current;
        current = current->next;
      }
    }
  }

  template< typename T >
  void FwdList< T >::reverse() noexcept
  {
    if (empty() || size() == 1)
    {
      return;
    }
    FwdListNode< T > * current = fake_->next;
    FwdListNode< T > * prev = fake_;
    FwdListNode< T > * next = nullptr;
    while (current != fake_)
    {
      next = current->next;
      current->next = prev;
      prev = current;
      current = next;
    }
    fake_->next = prev;
  }

  template< typename T >
  bool FwdList< T >::operator==(const FwdList< T > & rhs) const
  {
    if (size_ != rhs.size_)
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
  bool FwdList< T >::operator!=(const FwdList< T > & rhs) const
  {
    return !(*this == rhs);
  }

  template< typename T >
  bool FwdList< T >::operator<(const FwdList< T > & rhs) const noexcept
  {
    auto it1 = begin();
    auto it2 = rhs.begin();
    for (; it1 != end() && it2 != rhs.end(); ++it1, ++it2)
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
    return (it1 == end() && it2 != rhs.end());
  }

  template< typename T >
  bool FwdList< T >::operator<=(const FwdList< T > & rhs) const noexcept
  {
    return !(*this > rhs);
  }

  template< typename T >
  bool FwdList< T >::operator>(const FwdList< T > & rhs) const noexcept
  {
    return rhs < *this;
  }

  template< typename T >
  bool FwdList< T >::operator>=(const FwdList< T > & rhs) const noexcept
  {
    return !(*this < rhs);
  }

  template< typename T >
  void FwdList< T >::spliceAfter(cIterator position, FwdList< T > & fwdlst)
  {
    insertAfter(position, fwdlst.cbegin(), fwdlst.cend());
    fwdlst.clear();
  }

  template< typename T >
  void FwdList< T >::spliceAfter(cIterator position, FwdList< T > && fwdlst)
  {
    spliceAfter(position, fwdlst);
  }

  template< typename T >
  void FwdList< T >::spliceAfter(cIterator position, FwdList< T > & fwdlst, cIterator i)
  {
    auto nextIt = i;
    ++nextIt;
    auto value = *nextIt;
    insertAfter(position, value);
    fwdlst.eraseAfter(i);
  }

  template< typename T >
  void FwdList< T >::spliceAfter(cIterator position, FwdList< T > && fwdlst, cIterator i)
  {
    spliceAfter(position, fwdlst, i);
  }

  template< typename T >
  void FwdList< T >::spliceAfter(cIterator position,
      FwdList< T > & fwdlst, cIterator first, cIterator last)
  {
    cIterator temp = first;
    insertAfter(position, ++temp, last);
    fwdlst.eraseAfter(first, last);
  }

  template< typename T >
  void FwdList< T >::spliceAfter(cIterator position,
      FwdList< T > && fwdlst, cIterator first, cIterator last)
  {
    spliceAfter(position, fwdlst, first, last);
  }

  template< typename T >
  template< typename InputIterator, typename >
  void FwdList< T >::assign(InputIterator first, InputIterator last)
  {
    FwdList temp(first, last);
    swap(temp);
  }

  template< typename T >
  void FwdList< T >::assign(size_t n, const T & val)
  {
    FwdList temp(n, val);
    swap(temp);
  }

  template< typename T >
  void FwdList< T >::assign(std::initializer_list< T > il)
  {
    FwdList temp(il);
    swap(temp);
  }

  template< typename T >
  typename FwdList< T >::iterator FwdList< T >::eraseAfter(cIterator position)
  {
    assert(!empty());
    FwdListNode< T > * removeNode = position.node_->next;
    if (removeNode == fake_)
    {
      return end();
    }
    FwdListNode< T > * nextNode = removeNode->next;
    position.node_->next = nextNode;
    delete removeNode;
    --size_;
    return iterator(nextNode);
  }

  template< typename T >
  typename FwdList< T >::iterator FwdList< T >::eraseAfter(cIterator position, cIterator last)
  {
    assert(!empty());
    if (position == last)
    {
      return iterator(last.node_);
    }
    FwdListNode< T > * lastNode = last.node_;
    FwdListNode< T > * firstNode = position.node_;
    FwdListNode< T > * currentNode = firstNode->next;
    FwdListNode< T > * temp = nullptr;
    while (currentNode != lastNode && currentNode != fake_)
    {
      temp = currentNode->next;
      delete currentNode;
      currentNode = temp;
      --size_;
    }
    firstNode->next = lastNode;
    return iterator(lastNode);
  }

  template< typename T >
  typename FwdList< T >::iterator FwdList< T >::insertAfter(cIterator position, const T & val)
  {
    FwdListNode< T > * newNode = new FwdListNode< T >{val, nullptr};
    FwdListNode< T > * positionNode = position.node_;
    newNode->next = positionNode->next;
    positionNode->next = newNode;
    ++size_;
    return iterator(newNode);
  }

  template< typename T >
  typename FwdList< T >::iterator FwdList< T >::insertAfter(cIterator position, T && val)
  {
    return insertAfter(position, val);
  }

  template< typename T >
  typename FwdList< T >::iterator FwdList< T >::insertAfter(cIterator position, size_t n, const T & val)
  {
    for (size_t i = 0; i < n - 1; ++i)
    {
      insertAfter(position, val);
    }
    iterator temp = insertAfter(position, val);
    return temp;
  }

  template< typename T >
  template< typename InputIterator, typename >
  typename FwdList< T >::iterator FwdList< T >::insertAfter(cIterator position, InputIterator first, InputIterator last)
  {
    iterator temp = iterator(position.node_);
    for (auto it = first; it != last; ++it)
    {
      temp = insertAfter(position, *it);
      position++;
    }
    return temp;
  }

  template< typename T >
  typename FwdList< T >::iterator FwdList< T >::insertAfter(cIterator position, std::initializer_list< T > il)
  {
    return insertAfter(position, il.begin(), il.end());
  }
}

#endif
