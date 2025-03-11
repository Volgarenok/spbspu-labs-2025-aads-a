#ifndef FWDLIST_HPP
#define FWDLIST_HPP

#include <utility>
#include "iterator.hpp"
#include "node.hpp"

namespace kushekbaev
{
  template< typename T >
  struct FwdList
  {
    FwdList();
    FwdList(const FwdList& other);
    FwdList(FwdList&& other) noexcept;
    ~FwdList();
    FwdList& operator=(const FwdList& other) noexcept;
    FwdList& operator=(FwdList&& other) noexcept;

    Iterator< T > begin() const noexcept;
    Iterator< T > end() const noexcept;

    T& front() const noexcept;
    T& back() const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    void push_front(const T value);
    void push_back(const T value);
    void pop_front() noexcept;
    void pop_back() noexcept;
    void swap(FwdList& other) noexcept;
    void clear() noexcept;
    void remove(const T value);

    private:
      Node< T >* fake_;
      Node< T >* tail_;
      size_t size_;
  };

  template< typename T >
  FwdList< T >::FwdList():
    size_(0)
  {
    fake_ = new Node< T >(T());
    fake_ -> next_ = fake_;
  }

  template< typename T >
  FwdList< T >::FwdList(const FwdList& other):
    FwdList()
  {
    Node< T >* current = other.fake_->next_;
    while (current != other.fake_)
    {
      push_back(current->data_);
      current = current->next_;
    }
  }

  template< typename T >
  FwdList< T >::FwdList(FwdList&& other) noexcept:
    tail_(std::exchange(other.tail_, nullptr)),
    size_(std::exchange(other.size_, 0))
  {}

  template< typename T >
  FwdList< T >::~FwdList()
  {
    clear();
    delete fake_;
  }

  template< typename T >
  typename FwdList< T >::FwdList& FwdList< T >::operator=(const FwdList& other) noexcept
  {
    FwdList< T > copy(other);
    swap(copy);
    return *this;
  }

  template< typename T >
  typename FwdList< T >::FwdList& FwdList< T >::operator=(FwdList&& other) noexcept
  {
    FwdList< T > copy(other);
    swap(copy);
    return *this;
  }

  template< typename T >
  Iterator< T > FwdList< T >::begin() const noexcept
  {
    return Iterator< T >(fake_ -> next_);
  }

  template< typename T >
  Iterator< T > FwdList< T >::end() const noexcept
  {
    return Iterator< T >(fake_);
  }

  template< typename T >
  T& FwdList< T >::front() const noexcept
  {
    assert(!empty());
    return fake_ -> next_ -> data;
  }

  template< typename T >
  T& FwdList< T >::back() const noexcept
  {
    assert(!empty());
    Node< T >* current = fake_;
    while (current -> next_ != fake_)
    {
      current = current -> next_;
    }
    return current -> data_;
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
  void FwdList< T >::push_front(const T value)
  {
    assert(!empty());
    Node< T >* newNode = new Node< T >{ value, nullptr };
    newNode -> next_ = fake_ -> next;
    fake_ -> next_ = newNode;
    ++size_;
  }

  template< typename T >
  void FwdList< T >::push_back(const T value)
  {
    assert(!empty());
    Node< T >* newNode = new Node< T >{ value, nullptr };
    newNode -> next_ = fake_;
    Node< T >* current = fake_;
    while (current -> next_ != fake_)
    {
      current = current -> next_;
    }
    current -> next_ = newNode;
    ++size_;
  }

  template< typename T >
  void FwdList< T >::pop_front() noexcept
  {
    assert(!empty());
    Node< T >* todelete = fake_ -> next_;
    fake_ -> next_ = todelete -> next_;
    delete todelete;
    --size_;
  }

  template < class T >
  void FwdList< T >::pop_back() noexcept
  {
    assert(!empty());
    Node< T >* todelete = fake_;
    while (todelete -> next_ -> next_ != fake_)
    {
      todelete = todelete -> next_;
    }
    delete todelete -> next_;
    todelete -> next_ = fake_;
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
  void FwdList< T >::remove(const T value)
  {
    Node< T >* current = fake_;
    while (current -> next_ != fake_)
    {
      if (current -> next_ -> data_ = value)
      {
        Node< T >* todelete = current -> next;
        current -> next = current -> next -> next;
        delete todelete;
        --size;
      }
      else
      {
        current = current -> next_;
      }
    }
  }
}

#endif
