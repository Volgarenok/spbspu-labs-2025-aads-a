#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <algorithm>
#include <cstddef>
#include <memory>

namespace ivanova
{
  template < typename T >
  class Array
  {
  public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using iterator = pointer;
    using const_iterator = const_pointer;

    Array() noexcept:
      size_(0),
      capacity_(0),
      data_(nullptr)
    {}

    Array(const Array& other):
      size_(other.size_),
      capacity_(other.capacity_),
      data_(allocate(capacity_))
    {
      size_type constructed = 0;
      try
      {
        for (; constructed < size_; ++constructed)
        {
          new (data_ + constructed) value_type(other.data_[constructed]);
        }
      }
      catch (...)
      {
        destruct(data_, constructed);
        deallocate(data_);
        throw;
      }
    }

    Array(Array&& other) noexcept:
      Array()
    {
      swap(other);
    }

    ~Array()
    {
      destruct(data_, size_);
      deallocate(data_);
    }

    Array& operator=(const Array& other)
    {
      if (this != &other)
      {
        Array temp(other);
        swap(temp);
      }
      return *this;
    }

    Array& operator=(Array&& other) noexcept
    {
      Array temp(std::move(other));
      swap(temp);
      return *this;
    }

    void push_back(const_reference value)
    {
      if (size_ != capacity_)
      {
        new (data_ + size_) value_type(value);
        ++size_;
        return;
      }
      size_type new_capacity = size_ * 2 + 1;
      pointer new_data = allocate(new_capacity);
      try
      {
        new (new_data + size_) value_type(value);
      }
      catch (...)
      {
        deallocate(new_data);
        throw;
      }
      size_type constructed = 0;
      try
      {
        for (; constructed < size_; ++constructed)
        {
          new (new_data + constructed) value_type(data_[constructed]);
        }
      }
      catch (...)
      {
        destruct(new_data, constructed);
        deallocate(new_data);
        throw;
      }
      destruct(data_, size_);
      deallocate(data_);
      capacity_ = new_capacity;
      data_ = new_data;
      ++size_;
    }

    void pop_back()
    {
      if (size_ > 0)
      {
        back().~value_type();
        --size_;
      }
    }

    reference front()
    {
      return data_[0];
    }

    reference back()
    {
      return data_[size_ - 1];
    }

    const_reference front() const
    {
      return data_[0];
    }

    const_reference back() const
    {
      return data_[size_ - 1];
    }

    reference operator[](size_type index)
    {
      return data_[index];
    }

    const_reference operator[](size_type index) const
    {
      return data_[index];
    }

    pointer data() noexcept
    {
      return data_;
    }

    const_pointer data() const noexcept
    {
      return data_;
    }

    size_type size() const noexcept
    {
      return size_;
    }

    size_type capacity() const noexcept
    {
      return capacity_;
    }

    bool empty() const noexcept
    {
      return size_ == 0;
    }

    void swap(Array& other) noexcept
    {
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
      std::swap(data_, other.data_);
    }

    iterator begin() noexcept
    {
      return data_;
    }

    iterator end() noexcept
    {
      return data_ + size_;
    }

    const_iterator begin() const noexcept
    {
      return data_;
    }

    const_iterator end() const noexcept
    {
      return data_ + size_;
    }

  private:
    size_type size_;
    size_type capacity_;
    pointer data_;

    static pointer allocate(size_type size)
    {
      return size > 0 ? static_cast< pointer >(operator new(size * sizeof(value_type))) : nullptr;
    }

    static void deallocate(pointer data)
    {
      if (data != nullptr)
      {
        operator delete(data);
      }
    }

    static void destruct(pointer data, size_type size)
    {
      if (data != nullptr)
      {
        for (size_type i = 0; i < size; ++i)
        {
          data[i].~value_type();
        }
      }
    }
  };
}

#endif
