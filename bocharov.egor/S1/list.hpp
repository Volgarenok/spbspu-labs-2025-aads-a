#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <utility>
#include <initializer_list>
#include "node.hpp"
#include "iterator.hpp"

namespace bocharov
{
  template< typename T >
  class List
  {
  public:
    using iterator = Iterator< T, false >;
    using const_iterator = Iterator< T, true >;

    List() noexcept;
    List(const List & rhs);
    List(List && rhs) noexcept;
    List(T value, std::size_t count);
    template< class InputIt >
    List(InputIt first, InputIt last);
    List(std::initializer_list< T > init);
    ~List();

    List & operator=(const List & rhs) noexcept;
    List & operator=(List && rhs) noexcept;
    List & operator=(std::initializer_list< T > ilist);

    void assign(std::size_t count, const T & value);
    template< class InputIt >
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list< T > ilist);

    T & front();
    const T & front() const;
    T & back();
    const T & back() const;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    void clear() noexcept;
    iterator insert_after(const_iterator pos, const T & value);
    iterator insert_after(const_iterator pos, T && value);
    iterator insert_after(const_iterator pos, std::size_t count, const T & value);
    template< class InputIt >
    iterator insert_after(const_iterator pos, InputIt first, InputIt last);
    iterator insert_after(const_iterator pos, std::initializer_list< T > ilist);
    iterator erase_after(const_iterator pos);
    iterator erase_after(const_iterator first, const_iterator last);
    void push_front(T value);
    void push_back(T value);
    void pop_front() noexcept;
    void swap(List & other) noexcept;

    void splice_after(const_iterator pos, List & other);
    void splice_after(const_iterator pos, List && other);
    void splice_after(const_iterator pos, List & other, const_iterator it);
    void splice_after(const_iterator pos, List & other, const_iterator first, const_iterator last);
    void remove(const T & value);
    template< class UnaryPredicate >
    void remove_if(UnaryPredicate condition);

  private:
    Node< T > * tail_;
    std::size_t size_;
  };
}

template< typename T >
bocharov::List< T >::List() noexcept:
  tail_(nullptr),
  size_(0)
{}

template< typename T >
bocharov::List< T >::List(const List & rhs):
  List()
{
  if (!rhs.empty())
  {
    push_back(*(rhs.cbegin()));
    for (auto i = ++rhs.cbegin(); i != rhs.cend(); ++i)
    {
      push_back(*i);
    }
  }
}

template< typename T >
bocharov::List< T >::List(List && rhs) noexcept:
  tail_(std::exchange(rhs.tail_, nullptr)),
  size_(std::exchange(rhs.size_, 0))
{}

template< typename T >
bocharov::List< T >::~List()
{
  clear();
}

template< typename T >
typename bocharov::List< T >::List & bocharov::List< T >::operator=(const List & rhs) noexcept
{
  List< T > copied_rhs(rhs);
  swap(copied_rhs);
  return *this;
}

template< typename T >
typename bocharov::List< T >::List & bocharov::List< T >::operator=(List && rhs) noexcept
{
  List< T > copied_rhs(rhs);
  swap(copied_rhs);
  return *this;
}

template< typename T >
T & bocharov::List< T >::front()
{
  return tail_->next_->data_;
}

template< typename T >
const T & bocharov::List< T >::front() const
{
  return tail_->next_->data_;
}

template< typename T >
T & bocharov::List< T >::back()
{
  return tail_->data_;
}

template< typename T >
const T & bocharov::List< T >::back() const
{
  return tail_->data_;
}

template< typename T >
typename bocharov::List< T >::iterator bocharov::List< T >::begin() noexcept
{
  return iterator(tail_->next_);
}

template< typename T >
typename bocharov::List< T >::const_iterator bocharov::List< T >::begin() const noexcept
{
  return const_iterator(tail_->next_);
}

template< typename T >
typename bocharov::List< T >::const_iterator bocharov::List< T >::cbegin() const noexcept
{
  return const_iterator(tail_->next_);
}

template< typename T >
typename bocharov::List< T >::iterator bocharov::List< T >::end() noexcept
{
  return iterator(tail_->next_);
}

template< typename T >
typename bocharov::List< T >::const_iterator bocharov::List< T >::end() const noexcept
{
  return const_iterator(tail_->next_);
}

template< typename T >
typename bocharov::List< T >::const_iterator bocharov::List< T >::cend() const noexcept
{
  return const_iterator(tail_->next_);
}

template< typename T >
bool bocharov::List< T >::empty() const noexcept
{
  return size_ == 0;
}

template< typename T >
std::size_t bocharov::List< T >::size() const noexcept
{
  return size_;
}

template< typename T >
void bocharov::List< T >::clear() noexcept
{
  while (!empty())
  {
    pop_front();
  }
}

template< typename T >
void bocharov::List< T >::push_front(T value)
{
  Node< T > * new_node = new Node< T >{value, nullptr};
  if (empty())
  {
    tail_ = new_node;
    tail_->next_ = tail_;
  }
  else
  {
    new_node->next_ = tail_->next_;
    tail_->next_ = new_node;
  }
  ++size_;
}

template< typename T >
void bocharov::List< T >::push_back(T value)
{
  Node< T > * new_node = new Node< T >{value, nullptr};
  if (empty())
  {
    tail_ = new_node;
    tail_->next_ = tail_;
  }
  else
  {
    new_node->next_ = tail_->next_;
    tail_->next_ = new_node;
    tail_ = new_node;
  }
  ++size_;
}

template< typename T >
void bocharov::List< T >::pop_front() noexcept
{
  if (empty())
  {
    return;
  }
  Node< T > * to_delete = tail_->next_;
  tail_->next_ = to_delete->next_;
  delete to_delete;
  --size_;
}

template< typename T >
void bocharov::List< T >::swap(List & other) noexcept
{
  std::swap(tail_, other.tail_);
  std::swap(size_, other.size_);
}

#endif
