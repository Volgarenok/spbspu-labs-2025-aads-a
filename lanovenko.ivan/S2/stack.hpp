#ifndef STACK_HPP
#define STACK_HPP

#include <list>
#include <utility>

namespace lanovenko
{
  template < class T >
  class Stack
  {
  public:
    ~Stack() = default;
    Stack() = default;
    Stack(const Stack&) = default;
    Stack(Stack&&) = default;
    Stack& operator=(const Stack&) = default;
    Stack& operator=(Stack&&) = default;

    template < typename U >
    void push(U&& data) noexcept;
    void pop() noexcept;

    T& top() noexcept;
    const T& top() const noexcept;

    bool empty() const noexcept;
  private:
    std::list< T > data_;
  };
}


template < typename T >
template < typename U >
void lanovenko::Stack< T >::push(U&& data) noexcept
{
  data_.push_front(std::forward< U >(data));
}

template < typename T >
void lanovenko::Stack< T >::pop() noexcept
{
  data_.pop_front();
}

template < typename T >
T& lanovenko::Stack< T >::top() noexcept
{
  return data_.front();
}

template < typename T >
const T& lanovenko::Stack< T >::top() const noexcept
{
  return data_.front();
}

template< typename T >
bool lanovenko::Stack< T >::empty() const noexcept
{
  return data_.empty();
}

#endif
