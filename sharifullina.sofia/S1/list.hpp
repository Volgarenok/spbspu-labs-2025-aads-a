#ifndef LIST_HPP
#define LIST_HPP
#include <iterator>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include "node.hpp"
#include "iterator.hpp"
#include "constIterator.hpp"
namespace sharifullina
{
  template< typename T >
  class List
  {
  public:
    using Iterator = ::sharifullina::Iterator< T >;
    using ConstIterator = ::sharifullina::ConstIterator< T >;
    List();
    List(const List & other);
    List(List && other) noexcept;
    ~List();
    List & operator=(const List & other);
    List & operator=(List && other) noexcept;
    bool empty() const noexcept;
    std::size_t size() const noexcept;
    void pushBack(const T & value);
    void pushBack(T && value);
    void pushFront(const T & value);
    void pushFront(T && value);
    void popBack();
    void popFront();
    void clear() noexcept;
    void swap(List & other) noexcept;
    T & front();
    const T & front() const;
    T & back();
    const T & back() const;
    Iterator begin() noexcept;
    Iterator end() noexcept;
    ConstIterator begin() const noexcept;
    ConstIterator end() const noexcept;
    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;
  private:
    Node< T > * fakeNode_;
    std::size_t size_;
  };
  template< typename T >
  List< T >::List():
    fakeNode_(new Node< T >(T())),
    size_(0)
  {
    fakeNode_->prev_ = fakeNode_;
    fakeNode_->next_ = fakeNode_;
  }
  template< typename T >
  List< T >::List(const List & other):
    List()
  {
    try
    {
      for (ConstIterator it = other.begin(); it != other.end(); ++it)
      {
        pushBack(*it);
      }
    }
    catch (...)
    {
      clear();
      delete fakeNode_;
      throw;
    }
  }
  template< typename T >
  List< T >::List(List && other) noexcept:
    fakeNode_(other.fakeNode_),
    size_(other.size_)
  {
    other.fakeNode_ = new Node< T >(T());
    other.fakeNode_->prev_ = other.fakeNode_;
    other.fakeNode_->next_ = other.fakeNode_;
    other.size_ = 0;
  }
  template< typename T >
  List< T >::~List()
  {
    if (fakeNode_)
    {
      clear();
      delete fakeNode_;
    }
  }
  template< typename T >
  List< T > & List< T >::operator=(const List & other)
  {
    if (this != &other)
    {
      List temp(other);
      swap(temp);
    }
    return *this;
  }
  template< typename T >
  List< T > & List< T >::operator=(List && other) noexcept
  {
    if (this != &other)
    {
      clear();
      delete fakeNode_;
      fakeNode_ = other.fakeNode_;
      size_ = other.size_;
      other.fakeNode_ = new Node< T >(T());
      other.fakeNode_->prev_ = other.fakeNode_;
      other.fakeNode_->next_ = other.fakeNode_;
      other.size_ = 0;
    }
    return *this;
  }
  template< typename T >
  bool List< T >::empty() const noexcept
  {
    return fakeNode_->next_ == fakeNode_;
  }
  template< typename T >
  std::size_t List< T >::size() const noexcept
  {
    return size_;
  }
  template< typename T >
  void List< T >::pushBack(const T & value)
  {
    Node< T > * newNode = new Node< T >(value);
    newNode->prev_ = fakeNode_->prev_;
    newNode->next_ = fakeNode_;
    fakeNode_->prev_->next_ = newNode;
    fakeNode_->prev_ = newNode;
    ++size_;
  }
  template< typename T >
  void List< T >::pushBack(T && value)
  {
    Node< T > * newNode = new Node< T >(std::move(value));
    newNode->prev_ = fakeNode_->prev_;
    newNode->next_ = fakeNode_;
    fakeNode_->prev_->next_ = newNode;
    fakeNode_->prev_ = newNode;
    ++size_;
  }
  template< typename T >
  void List< T >::pushFront(const T & value)
  {
    Node< T > * newNode = new Node< T >(value);
    newNode->prev_ = fakeNode_;
    newNode->next_ = fakeNode_->next_;
    fakeNode_->next_->prev_ = newNode;
    fakeNode_->next_ = newNode;
    ++size_;
  }
  template< typename T >
  void List< T >::pushFront(T && value)
  {
    Node< T > * newNode = new Node< T >(std::move(value));
    newNode->prev_ = fakeNode_;
    newNode->next_ = fakeNode_->next_;
    fakeNode_->next_->prev_ = newNode;
    fakeNode_->next_ = newNode;
    ++size_;
  }
  template< typename T >
  void List< T >::popBack()
  {
    if (empty())
    {
      return;
    }
    Node< T > * toDelete = fakeNode_->prev_;
    toDelete->prev_->next_ = fakeNode_;
    fakeNode_->prev_ = toDelete->prev_;
    delete toDelete;
    --size_;
  }
  template< typename T >
  void List< T >::popFront()
  {
    if (empty())
    {
      return;
    }
    Node< T > * toDelete = fakeNode_->next_;
    toDelete->next_->prev_ = fakeNode_;
    fakeNode_->next_ = toDelete->next_;
    delete toDelete;
    --size_;
  }
  template< typename T >
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      popBack();
    }
  }
  template< typename T >
  void List< T >::swap(List & other) noexcept
  {
    std::swap(fakeNode_, other.fakeNode_);
    std::swap(size_, other.size_);
  }
  template< typename T >
  T & List< T >::front()
  {
    if (empty())
    {
      throw std::runtime_error("Attempt to access front of empty list");
    }
    return fakeNode_->next_->data_;
  }
  template< typename T >
  const T & List< T >::front() const
  {
    if (empty())
    {
      throw std::runtime_error("Attempt to access front of empty list");
    }
    return fakeNode_->next_->data_;
  }
  template< typename T >
  T & List< T >::back()
  {
    if (empty())
    {
      throw std::runtime_error("Attempt to access front of empty list");
    }
    return fakeNode_->prev_->data_;
  }
  template< typename T >
  const T & List< T >::back() const
  {
    if (empty())
    {
      throw std::runtime_error("Attempt to access front of empty list");
    }
    return fakeNode_->prev_->data_;
  }
  template< typename T >
  typename List< T >::Iterator List< T >::begin() noexcept
  {
    return Iterator(fakeNode_->next_);
  }
  template< typename T >
  typename List< T >::Iterator List< T >::end() noexcept
  {
    return Iterator(fakeNode_);
  }
  template< typename T >
  typename List< T >::ConstIterator List< T >::begin() const noexcept
  {
    return ConstIterator(fakeNode_->next_);
  }
  template< typename T >
  typename List< T >::ConstIterator List< T >::end() const noexcept
  {
    return ConstIterator(fakeNode_);
  }
  template< typename T >
  typename List< T >::ConstIterator List< T >::cbegin() const noexcept
  {
    return ConstIterator(fakeNode_->next_);
  }
  template< typename T >
  typename List< T >::ConstIterator List< T >::cend() const noexcept
  {
    return ConstIterator(fakeNode_);
  }
}
#endif
