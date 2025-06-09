#ifndef STACK_HPP
#define STACK_HPP

#include "array.hpp"

namespace zholobov {

  template < typename T >
  class Stack {
  public:
    void push(const T& value)
    {
      container_.push_back(value);
    }

    void push(T&& value)
    {
      container_.push_back(std::move(value));
    }

    const T& top() const
    {
      return container_.back();
    }

    T& top()
    {
      return container_.back();
    }

    void pop()
    {
      container_.pop_back();
    }

    bool empty() const noexcept
    {
      return container_.empty();
    }

    void clear() noexcept
    {
      container_.clear();
    }

    void swap(Stack& other) noexcept
    {
      container_.swap(other.container_);
    };

  private:
    Array< T > container_;
  };
}

#endif
