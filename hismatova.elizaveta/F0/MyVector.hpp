#ifndef MYVECTOR_HPP
#define MYVECTOR_HPP
#include <iostream>
#include <algorithm>
#include <cassert>
#include <utility>

namespace hismatova
{
  template < typename T >
  class Vector
  {
  public:
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;

    iterator begin() noexcept
    {
      return buffer_;
    }
    iterator end() noexcept
    {
      return buffer_ + size_;
    }
    const_iterator begin() const noexcept
    {
      return buffer_;
    }
    const_iterator end() const noexcept
    {
      return buffer_ + size_;
    }
    const_iterator cbegin() const noexcept
    {
      return buffer_;
    }
    const_iterator cend() const noexcept
    {
      return buffer_ + size_;
    }

    Vector() noexcept = default;

    template < typename InputIterator >
    Vector(InputIterator first, InputIterator last)
    {
      while (first != last)
      {
        PushBack(*first);
        ++first;
      }
    }

    explicit Vector(size_t size):
      buffer_(new T[size]),
      size_(size),
      capacity_(size)
    {
      std::uninitialized_value_construct_n(buffer_, size);
    }

    Vector(const Vector& other):
      buffer_(new T[other.size_]),
      size_(other.size_),
      capacity_(other.size_)
    {
      std::uninitialized_copy_n(other.buffer_, size_, buffer_);
    }

    Vector(Vector&& other) noexcept:
      buffer_(std::exchange(other.buffer_, nullptr)),
      size_(std::exchange(other.size_, 0)),
      capacity_(std::exchange(other.capacity_, 0))
    {
    }

    Vector& operator=(const Vector& rhs)
    {
      if (this != &rhs)
      {
        Vector temp(rhs);
        Swap(temp);
      }
      return *this;
    }

    Vector& operator=(Vector&& rhs) noexcept
    {
      Swap(rhs);
      return *this;
    }

    ~Vector()
    {
      std::destroy_n(buffer_, size_);
      delete[] buffer_;
    }

    void Swap(Vector& other) noexcept
    {
      std::swap(buffer_, other.buffer_);
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
    }

    size_t Size() const noexcept
    {
      return size_;
    }
    size_t Capacity() const noexcept
    {
      return capacity_;
    }

    void Reserve(size_t new_capacity)
    {
      if (new_capacity <= capacity_)
      {
        return;
      }

      T* new_buffer = new T[new_capacity];
      std::uninitialized_move_n(buffer_, size_, new_buffer);
      std::destroy_n(buffer_, size_);
      delete[] buffer_;

      buffer_ = new_buffer;
      capacity_ = new_capacity;
    }

    void Resize(size_t new_size)
    {
      if (new_size < size_)
      {
        std::destroy_n(buffer_ + new_size, size_ - new_size);
      }
      else if (new_size > size_)
      {
        Reserve(new_size);
        std::uninitialized_value_construct_n(buffer_ + size_, new_size - size_);
      }
      size_ = new_size;
    }

    template < typename... Args >
    T& EmplaceBack(Args&&... args)
    {
      if (size_ == capacity_)
      {
        size_t new_capacity = size_ == 0 ? 1: size_ * 2;
        Reserve(new_capacity);
      }
      new (buffer_ + size_) T(std::forward< Args >(args)...);
      ++size_;
      return buffer_[size_ - 1];
    }

    T& PushBack(const T& value)
    {
      return EmplaceBack(value);
    }
    T& PushBack(T&& value)
    {
      return EmplaceBack(std::move(value));
    }
    void push_back(const T& value)
    {
      PushBack(value);
    }
    void push_back(T&& value)
    {
      PushBack(std::move(value));
    }

    void PopBack() noexcept
    {
      assert(size_ > 0);
      std::destroy_at(buffer_ + (size_ - 1));
      --size_;
    }

    template < typename... Args >
    iterator Emplace(const_iterator pos, Args&&... args)
    {
      size_t index = pos - begin();

      if (index == size_)
      {
        EmplaceBack(std::forward<Args>(args)...);
        return begin() + index;
      }

      if (size_ == capacity_)
      {
        size_t new_capacity = size_ == 0 ? 1: size_ * 2;
        T* new_buffer = new T[new_capacity];

        std::uninitialized_move_n(buffer_, index, new_buffer);
        new (new_buffer + index) T(std::forward<Args>(args)...);
        std::uninitialized_move_n(buffer_ + index, size_ - index, new_buffer + index + 1);

        std::destroy_n(buffer_, size_);
        delete[] buffer_;

        buffer_ = new_buffer;
        capacity_ = new_capacity;
        ++size_;
        return begin() + index;
      }

      new (buffer_ + size_) T(std::move(buffer_[size_ - 1]));
      std::move_backward(begin() + index, end() - 1, end());
      buffer_[index] = T(std::forward< Args >(args)...);
      ++size_;
      return begin() + index;
    }

    iterator Insert(const_iterator pos, const T& value)
    {
      return Emplace(pos, value);
    }
    iterator Insert(const_iterator pos, T&& value)
    {
      return Emplace(pos, std::move(value));
    }

    iterator Erase(const_iterator pos) noexcept(std::is_nothrow_move_assignable_v< T >)
    {
      size_t index = pos - begin();
      std::move(begin() + index + 1, end(), begin() + index);
      PopBack();
      return begin() + index;
    }

    const T& operator[](size_t index) const noexcept
    {
      return buffer_[index];
    }
    T& operator[](size_t index) noexcept
    {
      return buffer_[index];
    }

  private:
    T* buffer_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
  };

  template< typename T >
  std::ostream& operator<<(std::ostream& out, const Vector< T >& vector)
  {
    out << "[ ";
    for (const auto& elem: vector)
    {
      out << elem << " ";
    }
    out << "]";
    return out;
  }
}
#endif
