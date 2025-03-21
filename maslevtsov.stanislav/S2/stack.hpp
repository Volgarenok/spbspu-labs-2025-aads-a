#ifndef STACK_HPP
#define STACK_HPP

#include <list>

namespace maslevtsov {
  template< class T >
  class Stack
  {
  public:
    Stack() noexcept = default;
    Stack(const Stack& rhs) = default;
    Stack(Stack&& rhs) noexcept = default;
    ~Stack() = default;

    Stack& operator=(const Stack& rhs) = default;
    Stack& operator=(Stack&& rhs) noexcept = default;

    T& top() noexcept;
    const T& top() const noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    void push(const T& value);
    void push(T&& value);
    void pop() noexcept;

  private:
    std::list< T > data_;
  };
}

template< class T >
T& maslevtsov::Stack< T >::top() noexcept
{
  return data_.back();
}

template< class T >
const T& maslevtsov::Stack< T >::top() const noexcept
{
  return data_.back();
}

template< class T >
bool maslevtsov::Stack< T >::empty() const noexcept
{
  return data_.empty();
}

template< class T >
std::size_t maslevtsov::Stack< T >::size() const noexcept
{
  return data_.size();
}

template< class T >
void maslevtsov::Stack< T >::push(const T& value)
{
  data_.push_back(value);
}

template< class T >
void maslevtsov::Stack< T >::push(T&& value)
{
  data_.push_back(std::move(value));
}

template< class T >
void maslevtsov::Stack< T >::pop() noexcept
{
  data_.pop_back();
}

#endif
