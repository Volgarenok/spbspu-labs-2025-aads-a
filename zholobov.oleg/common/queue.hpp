#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "array.hpp"

namespace zholobov {

  template < typename T >
  class Queue {
  public:
    void push(const T& value)
    {
      container_.push_back(value);
    }

    void push(T&& value)
    {
      container_.push_back(std::move(value));
    }

    const T& front() const
    {
      return container_.front();
    }

    T& front()
    {
      return container_.front();
    }

    void pop()
    {
      container_.pop_front();
    }

    bool empty() const noexcept
    {
      return container_.empty();
    }

    void clear() noexcept
    {
      container_.clear();
    }

    void swap(Queue& other) noexcept
    {
      container_.swap(other.container_);
    };

  private:
    Array< T > container_;
  };

}

#endif
