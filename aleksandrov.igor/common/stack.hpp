#ifndef STACK_HPP
#define STACK_HPP

#include "deque.hpp"

namespace aleksandrov
{
  template< class T >
  class Stack
  {
  public:
    T& top();
    const T& top() const;

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
    void swap(Stack&) noexcept;

    bool operator==(const Stack&) const;
    bool operator!=(const Stack&) const;

  private:
    Deque< T > container_;
  };

  template< class T >
  T& Stack< T >::top()
  {
    assert(!empty() && "Cannot access to element in empty stack!");
    return const_cast< T& >(static_cast< const Stack& >(*this).top());
  }

  template< class T >
  const T& Stack< T >::top() const
  {
    assert(!empty() && "Cannot access to element in empty stack!");
    return container_.back();
  }

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return container_.empty();
  }

  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return container_.size();
  }

  template< class T >
  size_t Stack< T >::maxSize() const noexcept
  {
    return container_.maxSize();
  }

  template< class T >
  size_t Stack< T >::capacity() const noexcept
  {
    return container_.capacity();
  }

  template< class T >
  void Stack< T >::shrinkToFit()
  {
    return container_.shrinkToFit();
  }

  template< class T >
  void Stack< T >::clear() noexcept
  {
    container_.clear();
  }

  template< class T >
  void Stack< T >::push(const T& value)
  {
    container_.pushBack(value);
  }

  template< class T >
  void Stack< T >::push(T&& value)
  {
    container_.pushBack(std::move(value));
  }

  template< class T >
  template< class... Args >
  void Stack< T >::emplace(Args&&... args)
  {
    container_.emplaceBack(std::forward< Args >(args)...);
  }

  template< class T >
  void Stack< T >::pop() noexcept
  {
    assert(!empty() && "Trying to delete from empty stack!");
    container_.popBack();
  }

  template< class T >
  void Stack< T >::swap(Stack& other) noexcept
  {
    container_.swap(other.container_);
  }

  template< class T >
  bool Stack< T >::operator==(const Stack& rhs) const
  {
    return container_ == rhs.container_;
  }

  template< class T >
  bool Stack< T >::operator!=(const Stack& rhs) const
  {
    return !operator==(rhs);
  }
}

#endif

