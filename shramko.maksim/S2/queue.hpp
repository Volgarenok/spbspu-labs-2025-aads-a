#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <FwdList.hpp>

namespace shramko
{
  template< typename T >
  class Queue
  {
  public:
    Queue() noexcept = default;
    ~Queue() = default;
    Queue(const Queue& other): container_(other.container_) {}
    Queue(Queue&& other) noexcept: container_(std::move(other.container_)) {}
    Queue& operator=(const Queue& other)
    {
      if (this != &other)
      {
        container_ = other.container_;
      }
      return *this;
    }
    Queue& operator=(Queue&& other) noexcept
    {
      if (this != &other)
      {
        container_ = std::move(other.container_);
      }
      return *this;
    }

    void push(const T& data)
    {
      container_.addToBack(data);
    }
    void push(T&& data)
    {
      container_.addToBack(std::move(data));
    }
    void pop()
    {
      if (container_.isEmpty())
      {
        throw std::logic_error("Queue is empty");
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
    T& front()
    {
      if (container_.isEmpty())
      {
        throw std::logic_error("Queue is empty");
      }
      return container_.getFront();
    }
    const T& front() const
    {
      if (container_.isEmpty())
      {
        throw std::logic_error("Queue is empty");
      }
      return container_.getFront();
    }
    void swap(Queue& other) noexcept
    {
      container_.swapLists(other.container_);
    }

  private:
    ForwardList< T > container_;
  };
}

#endif
