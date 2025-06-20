#ifndef DEQUE_HPP
#define DEQUE_HPP

#include <iostream>
#include <stdexcept>

namespace asafov
{
  template<class T>
  class Deque
  {
  public:
    Deque():
      data_(nullptr),
      capacity_(0),
      size_(0),
      front_index_(0),
      back_index_(0)
    {}

    Deque(const Deque& other):
      data_(new T[other.capacity_]),
      capacity_(other.capacity_),
      size_(other.size_),
      front_index_(0),
      back_index_(other.size_)
    {
      for (size_t i = 0; i < other.size_; ++i)
      {
        data_[i] = other.data_[(other.front_index_ + i) % other.capacity_];
      }
    }

    Deque(Deque&& other) noexcept:
      data_(other.data_),
      capacity_(other.capacity_),
      size_(other.size_),
      front_index_(other.front_index_),
      back_index_(other.back_index_)
    {
      other.data_ = nullptr;
      other.capacity_ = 0;
      other.size_ = 0;
      other.front_index_ = 0;
      other.back_index_ = 0;
    }

    ~Deque()
    {
      delete[] data_;
    }

    Deque& operator=(const Deque& other)
    {
      if (this != &other)
      {
        Deque temp(other);
        swap(temp);
      }
      return *this;
    }

    Deque& operator=(Deque&& other) noexcept
    {
      if (this != &other)
      {
        delete[] data_;
        data_ = other.data_;
        capacity_ = other.capacity_;
        size_ = other.size_;
        front_index_ = other.front_index_;
        back_index_ = other.back_index_;

        other.data_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;
        other.front_index_ = 0;
        other.back_index_ = 0;
      }
      return *this;
    }

    void push_back(const T& value)
    {
      if (size_ == capacity_)
      {
        resize(capacity_ == 0 ? 1 : capacity_ * 2);
      }
      data_[back_index_] = value;
      back_index_ = (back_index_ + 1) % capacity_;
      ++size_;
    }

    void push_front(const T& value)
    {
      if (size_ == capacity_)
      {
        resize(capacity_ == 0 ? 1 : capacity_ * 2);
      }
      front_index_ = (front_index_ - 1 + capacity_) % capacity_;
      data_[front_index_] = value;
      ++size_;
    }

    void pop_back()
    {
      if (empty())
      {
        throw std::out_of_range("Deque::pop_back(): empty Deque");
      }
      back_index_ = (back_index_ - 1 + capacity_) % capacity_;
      --size_;
    }

    void pop_front()
    {
      if (empty())
      {
        throw std::out_of_range("Deque::pop_front(): empty Deque");
      }
      front_index_ = (front_index_ + 1) % capacity_;
      --size_;
    }

    T& front()
    {
      if (empty())
      {
        throw std::out_of_range("Deque::front(): empty Deque");
      }
      return data_[front_index_];
    }

    T& back()
    {
      if (empty())
      {
        throw std::out_of_range("Deque::back(): empty Deque");
      }
      return data_[(back_index_ - 1 + capacity_) % capacity_];
    }

    bool empty() const
    {
      return size_ == 0;
    }

    size_t size() const
    {
      return size_;
    }

    void clear()
    {
      front_index_ = 0;
      back_index_ = 0;
      size_ = 0;
    }
  private:
    void resize(const size_t& new_capacity)
    {
      T* new_data = new T[new_capacity];
      for (size_t i = 0; i < size_; ++i)
      {
        new_data[i] = std::move(data_[(front_index_ + i) % capacity_]);
      }
      delete[] data_;
      data_ = new_data;
      front_index_ = 0;
      back_index_ = size_;
      capacity_ = new_capacity;
    }

    T* data_;
    size_t capacity_;
    size_t size_;
    size_t front_index_;
    size_t back_index_;
  };
}
#endif
