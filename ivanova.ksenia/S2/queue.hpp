#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <algorithm>
#include <cstddef>
#include "array.hpp"

namespace ivanova
{
  template < typename T >
  class Queue
  {
  public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    Queue() = default;

    Queue(const Queue& other)
      : head_(other.head_), tail_(other.tail_), size_(other.size_), buffer_(other.buffer_) {}

    Queue(Queue&& other) { swap(other); }

    ~Queue() = default;

    Queue& operator=(const Queue& other)
    {
      if (this != &other)
      {
        Queue temp(other);
        swap(temp);
      }
      return *this;
    }

    Queue& operator=(Queue&& other)
    {
      Queue temp(std::move(other));
      swap(temp);
      return *this;
    }

    reference front() { return buffer_[head_]; }
    reference back() { return buffer_[tail_]; }
    const_reference front() const { return buffer_[head_]; }
    const_reference back() const { return buffer_[tail_]; }

    void push(const_reference value)
    {
      if (buffer_.size() == 0)
      {
        buffer_.push_back(value);
        ++size_;
        return;
      }
      if (size_ == 0)
      {
        head_ = tail_ = 0;
        ++size_;
        buffer_[head_] = value;
        return;
      }
      tail_ = (tail_ + 1) % buffer_.size();
      if (tail_ == head_)
      {
        shiftBuffer();
        buffer_.push_back(value);
        ++size_;
        return;
      }
      buffer_[tail_] = value;
      ++size_;
    }

    void pop()
    {
      if (size_ == 0)
      {
        return;
      }
      head_ = (head_ + 1) % buffer_.size();
      --size_;
    }

    size_type size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }

    void swap(Queue& other)
    {
      std::swap(head_, other.head_);
      std::swap(tail_, other.tail_);
      std::swap(size_, other.size_);
      buffer_.swap(other.buffer_);
    }
  private:
    size_type head_ = 0;
    size_type tail_ = 0;
    size_type size_ = 0;
    Array< T > buffer_;

    void shiftBuffer()
    {
      if (head_ == 0)
      {
        tail_ = size_;
        return;
      }
      std::reverse(buffer_.begin(), buffer_.begin() + head_);
      std::reverse(buffer_.begin() + head_, buffer_.end());
      std::reverse(buffer_.begin(), buffer_.end());
      head_ = 0;
      tail_ = size_;
    }
  };
}

#endif
