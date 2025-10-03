#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <FwdList.hpp>

namespace shramko
{
  template< typename T >
  class Stack
  {
  public:
    Stack() noexcept = default;
    ~Stack() = default;
    Stack(const Stack& other):
      container_(other.container_)
    {}
    Stack(Stack&& other) noexcept:
      container_(std::move(other.container_))
    {}
    Stack& operator=(const Stack& other)
    {
      if (this != &other)
      {
        container_ = other.container_;
      }
      return *this;
    }
    Stack& operator=(Stack&& other) noexcept
    {
      if (this != &other)
      {
        container_ = std::move(other.container_);
      }
      return *this;
    }

    void push(const T& data)
    {
      container_.addToFront(data);
    }
    void push(T&& data)
    {
      container_.addToFront(std::move(data));
    }
    void pop()
    {
      if (container_.isEmpty())
      {
        throw std::logic_error("Stack is empty");
      }
      container_.removeFront();
    }
    size_t size() const noexcept
    {
      return container_.getSize();
    }
    bool empty() const noexcept
    {
      return container_.isEmpty();
    }
    T& top()
    {
      if (container_.isEmpty())
      {
        throw std::logic_error("Stack is empty");
      }
      return container_.getFront();
    }
    const T& top() const
    {
      if (container_.isEmpty())
      {
        throw std::logic_error("Stack is empty");
      }
      return container_.getFront();
    }
    void swap(Stack& other) noexcept
    {
      container_.swapLists(other.container_);
    }

  private:
    ForwardList< T > container_;
  };
}

#endif
