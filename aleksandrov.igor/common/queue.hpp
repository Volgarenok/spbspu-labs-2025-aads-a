#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "deque.hpp"

namespace aleksandrov
{
  template< class T >
  class Queue
  {
  public:
    T& front();
    const T& front() const;

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t maxSize() const noexcept;
    size_t capacity() const noexcept;
    void shrinkToFit();

    void clear() noexcept;
    void push(const T&);
    void push(T&&);
    template< class... Args >
    void emplace(Args&&...);
    void pop() noexcept;
    void swap(Queue&) noexcept;

    bool operator==(const Queue&) const;
    bool operator!=(const Queue&) const;

  private:
    Deque< T > container_;
  };

  template< class T >
  T& Queue< T >::front()
  {
    assert(!empty() && "Cannot access to element in empty queue!");
    return const_cast< T& >(static_cast< const Queue& >(*this).front());
  }

  template< class T >
  const T& Queue< T >::front() const
  {
    assert(!empty() && "Cannot access to element in empty queue!");
    return container_.front();
  }

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return container_.empty();
  }

  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return container_.size();
  }

  template< class T >
  size_t Queue< T >::maxSize() const noexcept
  {
    return container_.maxSize();
  }

  template< class T >
  size_t Queue< T >::capacity() const noexcept
  {
    return container_.capacity();
  }

  template< class T >
  void Queue< T >::shrinkToFit()
  {
    container_.shrinkToFit();
  }

  template< class T >
  void Queue< T >::clear() noexcept
  {
    container_.clear();
  }

  template< class T >
  void Queue< T >::push(const T& value)
  {
    container_.pushBack(value);
  }

  template< class T >
  void Queue< T >::push(T&& value)
  {
    container_.pushBack(std::move(value));
  }

  template< class T >
  template< class... Args >
  void Queue< T >::emplace(Args&&... args)
  {
    container_.emplaceBack(std::forward< Args >(args)...);
  }

  template< class T >
  void Queue< T >::pop() noexcept
  {
    assert(!empty() && "Cannot delete element from empty queue!");
    container_.popFront();
  }

  template< class T >
  void Queue< T >::swap(Queue& other) noexcept
  {
    container_.swap(other.container_);
  }

  template< class T >
  bool Queue< T >::operator==(const Queue& rhs) const
  {
    return container_ == rhs.container_;
  }

  template< class T >
  bool Queue< T >::operator!=(const Queue& rhs) const
  {
    return !operator==(rhs);
  }
}

#endif

