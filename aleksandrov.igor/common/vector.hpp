#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include "../S7/vector-iterator.hpp"

namespace aleksandrov
{
  constexpr size_t minVectorCapacity = 64;

  template< class T, bool isConst >
  class VectorIterator;

  template< class T >
  class Vector
  {
  public:
    using Iter = VectorIterator< T, false >;
    using ConstIter = VectorIterator< T, true >;

    Vector();
    Vector(size_t, const T&);
    template< class InputIt >
    Vector(InputIt, InputIt);
    Vector(std::initializer_list< T >);
    Vector(const Vector&);
    Vector(Vector&&);
    ~Vector() noexcept;

    Vector& operator=(const Vector&);
    Vector& operator=(Vector&&);
    Vector& operator=(std::initializer_list< T >);

    void assign(size_t, const T&);
    template< class InputIt >
    void assign(InputIt, InputIt);
    void assign(std::initializer_list< T >);

    T& at(size_t);
    const T& at(size_t) const;
    T& operator[](size_t);
    const T& operator[](size_t) const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    Iter begin() noexcept;
    ConstIter begin() const noexcept;
    ConstIter cbegin() const noexcept;
    Iter end() noexcept;
    ConstIter end() const noexcept;
    ConstIter cend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;

    void clear();
    void pushBack(const T&);
    void pushBack(T&&);
    template< class... Args >
    void emplaceBack(Args&&...);
    void popBack();
    void swap(Vector&) noexcept;

    bool operator==(const Vector&);
    bool operator!=(const Vector&);

  private:
    T* data_;
    size_t size_;
    size_t capacity_;

    T* copyData(const Vector&);
    void resize();
  };

  template< class T >
  Vector< T >::Vector():
    data_(nullptr),
    size_(0),
    capacity_(0)
  {}

  template< class T >
  Vector< T >::Vector(size_t count, const T& value):
    Vector()
  {
    for (size_t i = 0; i < count; ++i)
    {
      emplaceBack(value);
    }
  }

  template< class T >
  template< class InputIt >
  Vector< T >::Vector(InputIt first, InputIt last):
    Vector()
  {
    for (auto it = first; it != last; ++it)
    {
      emplaceBack(*it);
    }
  }

  template< class T >
  Vector< T >::Vector(std::initializer_list< T > ilist):
    Vector(ilist.begin(), ilist.end())
  {}

  template< class T >
  Vector< T >::Vector(const Vector& rhs):
    data_(copyData(rhs)),
    size_(rhs.size_),
    capacity_(rhs.capacity_)
  {}

  template< class T >
  Vector< T >::Vector(Vector&& rhs):
    data_(std::exchange(rhs.data_, nullptr)),
    size_(std::exchange(rhs.size_, 0)),
    capacity_(std::exchange(rhs.capacity_, 0))
  {}

  template< class T >
  Vector< T >::~Vector() noexcept
  {
    clear();
    operator delete[](data_);
  }

  template< class T >
  Vector< T >& Vector< T >::operator=(const Vector& rhs)
  {
    Vector copy(rhs);
    swap(copy);
    return *this;
  }

  template< class T >
  Vector< T >& Vector< T >::operator=(Vector&& rhs)
  {
    Vector copy(std::move(rhs));
    swap(copy);
    return *this;
  }

  template< class T >
  Vector< T >& Vector< T >::operator=(std::initializer_list< T > ilist)
  {
    Vector copy(ilist);
    swap(copy);
    return *this;
  }

  template< class T >
  void Vector< T >::assign(size_t count, const T& value)
  {
    clear();
    for (size_t i = 0; i < count; ++i)
    {
      emplaceBack(value);
    }
  }

  template< class T >
  template< class InputIt >
  void Vector< T >::assign(InputIt first, InputIt last)
  {
    clear();
    for (auto it = first; it != last; ++it)
    {
      emplaceBack(*it);
    }
  }

  template< class T >
  void Vector< T >::assign(std::initializer_list< T > ilist)
  {
    assign(ilist.begin(), ilist.end());
  }

  template< class T >
  T& Vector< T >::at(size_t pos)
  {
    return const_cast< T& >(static_cast< const Vector& >(*this).at(pos));
  }

  template< class T >
  const T& Vector< T >::at(size_t pos) const
  {
    if (pos >= size_)
    {
      throw std::out_of_range("ERROR: Access beyond vector's bounds!");
    }
    return data_[pos];
  }

  template< class T >
  T& Vector< T >::operator[](size_t pos)
  {
    return data_[pos];
  }

  template< class T >
  const T& Vector< T >::operator[](size_t pos) const
  {
    return data_[pos];
  }

  template< class T >
  T& Vector< T >::front()
  {
    return data_[0];
  }

  template< class T >
  const T& Vector< T >::front() const
  {
    return data_[0];
  }

  template< class T >
  T& Vector< T >::back()
  {
    return data_[size_ - 1];
  }

  template< class T >
  const T& Vector< T >::back() const
  {
    return data_[size_ - 1];
  }

  template< class T >
  typename Vector< T >::Iter Vector< T >::begin() noexcept
  {
    return Iter(data_);
  }

  template< class T >
  typename Vector< T >::ConstIter Vector< T >::begin() const noexcept
  {
    return cbegin();
  }

  template< class T >
  typename Vector< T >::ConstIter Vector< T >::cbegin() const noexcept
  {
    return ConstIter(data_);
  }

  template< class T >
  typename Vector< T >::Iter Vector< T >::end() noexcept
  {
    return Iter(data_ + size_);
  }

  template< class T >
  typename Vector< T >::ConstIter Vector< T >::end() const noexcept
  {
    return cend();
  }

  template< class T >
  typename Vector< T >::ConstIter Vector< T >::cend() const noexcept
  {
    return ConstIter(data_ + size_);
  }

  template< class T >
  bool Vector< T >::empty() const noexcept
  {
    return !size_;
  }

  template< class T >
  size_t Vector< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  size_t Vector< T >::capacity() const noexcept
  {
    return capacity_;
  }

  template< class T >
  void Vector< T >::clear()
  {
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i].~T();
    }
    size_ = 0;
  }

  template< class T >
  void Vector< T >::pushBack(const T& value)
  {
    emplaceBack(value);
  }

  template< class T >
  void Vector< T >::pushBack(T&& value)
  {
    emplaceBack(std::move(value));
  }

  template< class T >
  template< class... Args >
  void Vector< T >::emplaceBack(Args&&... args)
  {
    if (size_ == capacity_)
    {
      resize();
    }
    new (data_ + size_) T(std::forward< Args >(args)...);
    ++size_;
  }

  template< class T >
  void Vector< T >::popBack()
  {
    if (size_ > 0)
    {
      data_[size_ - 1].~T();
      --size_;
    }
  }

  template< class T >
  void Vector< T >::swap(Vector& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  template< class T >
  bool Vector< T >::operator==(const Vector& other)
  {
    if (size_ != other.size_)
    {
      return false;
    }
    for (size_t i = 0; i < size_; ++i)
    {
      if (data_[i] != other.data_[i])
      {
        return false;
      }
    }
    return true;
  }

  template< class T >
  bool Vector< T >::operator!=(const Vector& other)
  {
    return !operator==(other);
  }

  template< class T >
  T* Vector< T >::copyData(const Vector& vector)
  {
    T* copy = static_cast< T* >(operator new[](vector.capacity_ * sizeof(T)));
    size_t i = 0;
    try
    {
      for (; i < vector.size_; ++i)
      {
        new (copy + i) T(vector.data_[i]);
      }
    }
    catch (...)
    {
      for (size_t j = 0; j < i; ++j)
      {
        copy[j].~T();
      }
      operator delete[](copy);
      throw;
    }
    return copy;
  }

  template< class T >
  void Vector< T >::resize()
  {
    size_t newCapacity = capacity_ ? capacity_ * 2 : minVectorCapacity;
    T* newData = static_cast< T* >(operator new[](newCapacity * sizeof(T)));
    size_t i = 0;
    try
    {
      for (; i < size_; ++i)
      {
        new (newData + i) T(std::move(data_[i]));
      }
    }
    catch (...)
    {
      for (size_t j = 0; j < i; ++j)
      {
        newData[j].~T();
      }
      operator delete[](newData);
      throw;
    }
    clear();
    operator delete[](data_);
    data_ = newData;
    size_ = capacity_;
    capacity_ = newCapacity;
  }
}

#endif

