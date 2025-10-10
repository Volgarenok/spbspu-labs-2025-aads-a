#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <stdexcept>

namespace kushekbaev
{
  template< typename T >
  class Vector
  {
    public:
      Vector();
      Vector(const Vector& other);
      Vector(Vector&& other) noexcept;
      ~Vector();

      Vector& operator=(const Vector& other);
      Vector& operator=(Vector&& other) noexcept;

      T& front() noexcept;
      T& back() noexcept;
      const T& front() const noexcept;
      const T& back() const noexcept;

      bool empty() const noexcept;
      size_t size() const noexcept;

      void pushBack(const T& value);
      void popFront();
      void popBack();

      void swap(Vector& other) noexcept;

      using iterator = T*;
      using const_iterator = const T*;

      iterator begin() noexcept;
      iterator end() noexcept;
      const_iterator begin() const noexcept;
      const_iterator end() const noexcept;
      const_iterator cbegin() const noexcept;
      const_iterator cend() const noexcept;

      T& operator[](size_t index) noexcept;
      const T& operator[](size_t index) const noexcept;

      void clear() noexcept;
      bool contains(const T& value) const;
      size_t count(const T& value) const;
      iterator find(const T& value);
      const_iterator find(const T& value) const;
      void insert(const T& value);
      template< typename InputIterator >
      void insert(InputIterator first, InputIterator last);
      void erase(iterator position);
      size_t erase(const T& value);

    private:
      T* data_;
      size_t size_;
      size_t capacity_;
      void extendVector();
      class RAII
      {
        public:
          explicit RAII(T* p):
            ptr(p)
          {}

          ~RAII()
          {
            delete[] ptr;
          }

          RAII(RAII&& other) noexcept:
            ptr(other.ptr)
          {
            other.ptr = nullptr;
          }

          T* ptr;
      };
  };

  template< typename T >
  Vector< T >::Vector():
    data_(new T[1]),
    size_(0),
    capacity_(1)
  {}

  template< typename T >
  Vector< T >::Vector(const Vector& other):
    data_(nullptr),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    RAII newHolder(new T[other.capacity_]);
    for (size_t i = 0; i < size_; ++i)
    {
      newHolder.ptr[i] = other.data_[i];
    }
    data_ = newHolder.ptr;
    newHolder.ptr = nullptr;
  }

  template< typename T >
  Vector< T >::Vector(Vector&& other) noexcept:
    data_(other.data_),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  template< typename T >
  Vector< T >::~Vector()
  {
    delete[] data_;
  }

  template< typename T >
  Vector< T >& Vector< T >::operator=(const Vector& other)
  {
    if (this != std::addressof(other))
    {
      RAII newHolder(new T[other.capacity_]);
      for (size_t i = 0; i < other.size_; ++i)
      {
        newHolder.ptr[i] = other.data_[i];
      }
      std::swap(data_, newHolder.ptr);
      capacity_ = other.capacity_;
      size_ = other.size_;
    }
    return *this;
  }

  template< typename T >
  Vector< T >& Vector< T >::operator=(Vector&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      Vector tmp(other);
      swap(other);
    }
    return *this;
  }

  template< typename T >
  bool Vector< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  size_t Vector< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  T& Vector< T >::front() noexcept
  {
    return data_[0];
  }

  template< typename T >
  T& Vector< T >::back() noexcept
  {
    return data_[size_ - 1];
  }

  template< typename T >
  const T& Vector< T >::front() const noexcept
  {
    return data_[0];
  }

  template< typename T >
  const T& Vector< T >::back() const noexcept
  {
    return data_[size_ - 1];
  }

  template< typename T >
  void Vector< T >::pushBack(const T& value)
  {
    if (size_ == capacity_)
    {
      extendVector();
    }
    data_[size_++] = value;
  }

  template< typename T >
  void Vector< T >::popFront()
  {
    if (empty())
    {
      throw std::out_of_range("The array is empty!");
    }
    for (size_t i = 1; i < size_; ++i)
    {
      data_[i - 1] = std::move(data_[i]);
    }
    --size_;
  }

  template< typename T >
  void Vector< T >::popBack()
  {
    if (!empty())
    {
      --size_;
    }
    else
    {
      throw std::out_of_range("The array is empty!");
    }
  }

  template< typename T >
  void Vector< T >::swap(Vector& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  template< typename T >
  typename Vector< T >::iterator Vector< T >::begin() noexcept
  {
    return data_;
  }

  template< typename T >
  typename Vector< T >::iterator Vector< T >::end() noexcept
  {
    return data_ + size_;
  }

  template< typename T >
  typename Vector< T >::const_iterator Vector< T >::begin() const noexcept
  {
    return data_;
  }

  template< typename T >
  typename Vector< T >::const_iterator Vector< T >::end() const noexcept
  {
    return data_ + size_;
  }

  template< typename T >
  typename Vector< T >::const_iterator Vector< T >::cbegin() const noexcept
  {
    return data_;
  }

  template< typename T >
  typename Vector< T >::const_iterator Vector< T >::cend() const noexcept
  {
    return data_ + size_;
  }

  template< typename T >
  T& Vector< T >::operator[](size_t index) noexcept
  {
    return data_[index];
  }

  template< typename T >
  const T& Vector< T >::operator[](size_t index) const noexcept
  {
    return data_[index];
  }

  template< typename T >
  void Vector< T >::clear() noexcept
  {
    size_ = 0;
  }

  template< typename T >
  bool Vector< T >::contains(const T& value) const
  {
    for (size_t i = 0; i < size_; ++i)
    {
      if (data_[i] == value)
      {
        return true;
      }
    }
    return false;
  }

  template< typename T >
  size_t Vector< T >::count(const T& value) const
  {
    size_t counter = 0;
    for (size_t i = 0; i < size_; ++i)
    {
      if (data_[i] == value)
      {
        ++counter;
      }
    }
    return counter;
  }

  template< typename T >
  typename Vector< T >::iterator Vector< T >::find(const T& value)
  {
    for (size_t i = 0; i < size_; ++i)
    {
      if (data_[i] == value)
      {
        return data_ + i;
      }
    }
    return end();
  }

  template< typename T >
  typename Vector< T >::const_iterator Vector< T >::find(const T& value) const
  {
    for (size_t i = 0; i < size_; ++i)
    {
      if (data_[i] == value)
      {
        return data_ + i;
      }
    }
    return cend();
  }

  template< typename T >
  void Vector< T >::insert(const T& value)
  {
    if (!contains(value))
    {
      pushBack(value);
    }
  }

  template< typename T >
  template< typename InputIterator >
  void Vector< T >::insert(InputIterator first, InputIterator last)
  {
    for (auto it = first; it != last; ++it)
    {
      insert(*it);
    }
  }

  template< typename T >
  void Vector< T >::erase(iterator position)
  {
    if (position < begin() || position >= end())
    {
      return;
    }
    size_t index = position - begin();
    for (size_t i = index; i < size_ - 1; ++i)
    {
      data_[i] = std::move(data_[i + 1]);
    }
    --size_;
  }

  template< typename T >
  size_t Vector< T >::erase(const T& value)
  {
    auto it = find(value);
    if (it != end())
    {
      erase(it);
      return 1;
    }
    return 0;
  }

  template< typename T >
  void Vector< T >::extendVector()
  {
    size_t newCapacity = (capacity_ == 0) ? 1 : capacity_ * 2;
    RAII newHolder(new T[newCapacity]);
    for (size_t i = 0; i < size_; ++i)
    {
      newHolder.ptr[i] = std::move(data_[i]);
    }
    delete[] data_;
    data_ = newHolder.ptr;
    newHolder.ptr = nullptr;
    capacity_ = newCapacity;
  }
}

#endif
