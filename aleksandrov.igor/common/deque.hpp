#ifndef DEQUE_HPP
#define DEQUE_HPP

#include <cstddef>
#include <cassert>
#include <limits>
#include <utility>
#include <stdexcept>

namespace aleksandrov
{
  constexpr size_t minDequeCapacity = 64;

  template< class T >
  class Deque
  {
  public:
    Deque();
    Deque(const Deque&);
    Deque(Deque&&) noexcept;
    ~Deque() noexcept;

    Deque& operator=(const Deque&);
    Deque& operator=(Deque&&) noexcept;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t maxSize() const noexcept;
    size_t capacity() const noexcept;
    void shrinkToFit();

    void clear() noexcept;
    void pushFront(const T&);
    void pushFront(T&&);
    void pushBack(const T&);
    void pushBack(T&&);
    template< class... Args >
    void emplaceFront(Args&&...);
    template< class... Args >
    void emplaceBack(Args&&...);
    void popFront();
    void popBack();
    void swap(Deque&) noexcept;

    bool operator==(const Deque&) const;
    bool operator!=(const Deque&) const;

  private:
    T* data_;
    size_t first_;
    size_t last_;
    size_t size_;
    size_t capacity_;

    T* copyData(const Deque&);
    void resize();
    size_t getBiggerCapacity(size_t) const noexcept;
  };

  template< class T >
  Deque< T >::Deque():
    data_(nullptr),
    first_(0),
    last_(0),
    size_(0),
    capacity_(0)
  {}

  template< class T >
  Deque< T >::Deque(const Deque& rhs):
    data_(copyData(rhs)),
    first_(rhs.first_),
    last_(rhs.last_),
    size_(rhs.size_),
    capacity_(rhs.capacity_)
  {}

  template< class T >
  Deque< T >::Deque(Deque&& rhs) noexcept:
    data_(std::exchange(rhs.data_, nullptr)),
    first_(std::exchange(rhs.first_, 0)),
    last_(std::exchange(rhs.last_, 0)),
    size_(std::exchange(rhs.size_, 0)),
    capacity_(std::exchange(rhs.capacity_, 0))
  {}

  template< class T >
  Deque< T >::~Deque() noexcept
  {
    clear();
    operator delete(data_);
  }

  template< class T >
  Deque< T >& Deque< T >::operator=(const Deque& rhs)
  {
    Deque copy(rhs);
    swap(copy);
    return *this;
  }

  template< class T >
  Deque< T >& Deque< T >::operator=(Deque&& rhs) noexcept
  {
    Deque copy(std::move(rhs));
    swap(copy);
    return *this;
  }

  template< class T >
  T& Deque< T >::front()
  {
    assert(!empty());
    return const_cast< T& >(static_cast< const Deque& >(*this).front());
  }

  template< class T >
  const T& Deque< T >::front() const
  {
    assert(!empty());
    return data_[first_];
  }

  template< class T >
  T& Deque< T >::back()
  {
    assert(!empty());
    return const_cast< T& >(static_cast< const Deque& >(*this).back());
  }

  template< class T >
  const T& Deque< T >::back() const
  {
    assert(!empty());
    return data_[last_];
  }

  template< class T >
  bool Deque< T >::empty() const noexcept
  {
    return !size_;
  }

  template< class T >
  size_t Deque< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  size_t Deque< T >::capacity() const noexcept
  {
    return capacity_;
  }

  template< class T >
  size_t Deque< T >::maxSize() const noexcept
  {
    return std::numeric_limits< size_t >::max();
  }

  template< class T >
  void Deque< T >::shrinkToFit()
  {
    if (capacity_ == size_)
    {
      return;
    }
    else if (capacity_ > size_)
    {
      T* newData = static_cast< T* >(operator new(size_ * sizeof(T)));
      size_t i = first_;
      try
      {
        for (; i < size_ + first_; ++i)
        {
          new (newData + i % size_) T(std::move_if_noexcept(data_[i % capacity_]));
        }
      }
      catch (...)
      {
        for (size_t j = first_; j < i + first_; ++j)
        {
          newData[j % i].~T();
        }
        operator delete(newData);
        throw;
      }
      for (size_t i = first_; i < size_ + first_; ++i)
      {
        data_[i % capacity_].~T();
      }
      operator delete(data_);
      data_ = newData;
      capacity_ = size_;
    }
  }

  template< class T >
  void Deque< T >::clear() noexcept
  {
    while (!empty())
    {
      popBack();
    }
  }

  template< class T >
  void Deque< T >::pushFront(const T& value)
  {
    emplaceFront(value);
  }

  template< class T >
  void Deque< T >::pushFront(T&& value)
  {
    emplaceFront(std::move(value));
  }

  template< class T >
  void Deque< T >::pushBack(const T& value)
  {
    emplaceBack(value);
  }

  template< class T >
  void Deque< T >::pushBack(T&& value)
  {
    emplaceBack(std::move(value));
  }

  template< class T >
  template< class... Args >
  void Deque< T >::emplaceFront(Args&&... args)
  {
    if (size_ == capacity_)
    {
      resize();
    }
    size_t shift = capacity_ - 1;
    if (!empty())
    {
      shift = first_ ? --first_ : first_ = capacity_ - 1;
    }
    new (data_ + shift) T(std::forward< Args >(args)...);
    ++size_;
  }

  template< class T >
  template< class... Args >
  void Deque< T >::emplaceBack(Args&&... args)
  {
    if (size_ == capacity_)
    {
      resize();
    }
    size_t shift = 0;
    if (!empty())
    {
      shift = last_ = (last_ + 1) % capacity_;
    }
    new (data_ + shift) T(std::forward< Args >(args)...);
    ++size_;
  }

  template< class T >
  void Deque< T >::popFront()
  {
    assert(!empty());
    data_[first_].~T();
    if (size_ != 1)
    {
      first_ = (first_ + 1) % capacity_;
    }
    else
    {
      first_ = 0;
      last_ = 0;
    }
    --size_;
  }

  template< class T >
  void Deque< T >::popBack()
  {
    assert(!empty());
    data_[last_].~T();
    if (size_ != 1)
    {
      last_ = last_ ? last_ - 1 : capacity_ - 1;
    }
    else
    {
      first_ = 0;
      last_ = 0;
    }
    --size_;
  }

  template< class T >
  void Deque< T >::swap(Deque& rhs) noexcept
  {
    std::swap(data_, rhs.data_);
    std::swap(first_, rhs.first_);
    std::swap(last_, rhs.last_);
    std::swap(size_, rhs.size_);
    std::swap(capacity_, rhs.capacity_);
  }

  template< class T >
  bool Deque< T >::operator==(const Deque& other) const
  {
    if (size_ != other.size_)
    {
      return false;
    }
    size_t i = first_;
    size_t j = other.first_;
    while (i < size_ + first_ && j < size_ + other.first_)
    {
      if (data_[i % capacity_] != other.data_[j % other.capacity_])
      {
        return false;
      }
      ++i;
      ++j;
    }
    return true;
  }

  template< class T >
  bool Deque< T >::operator!=(const Deque& other) const
  {
    return !operator==(other);
  }

  template< class T >
  T* Deque< T >::copyData(const Deque& other)
  {
    if (!other.capacity_)
    {
      return nullptr;
    }
    T* newData = static_cast< T* >(operator new(other.capacity_ * sizeof(T)));
    size_t i = other.first_;
    try
    {
      for (; i < other.size_ + other.first_; ++i)
      {
        new (newData + i % other.capacity_) T(other.data_[i % other.capacity_]);
      }
    }
    catch (...)
    {
      for (size_t j = other.first_; j < i + other.first_; ++j)
      {
        newData[j % other.capacity_].~T();
      }
      operator delete(newData);
      throw;
    }
    return newData;
  }

  template< class T >
  void Deque< T >::resize()
  {
    if (!capacity_)
    {
      data_ = static_cast< T* >(operator new(minDequeCapacity * sizeof(T)));
      capacity_ = minDequeCapacity;
      return;
    }
    size_t newCapacity = getBiggerCapacity(capacity_);
    T* newData = static_cast< T* >(operator new(newCapacity * sizeof(T)));
    size_t i = 0;
    try
    {
      for (size_t j = first_; i < size_; ++i, j = (j + 1) % capacity_)
      {
        new (newData + i) T(std::move_if_noexcept(data_[j]));
      }
    }
    catch (...)
    {
      for (size_t j = 0; j < i; ++j)
      {
        newData[j].~T();
      }
      operator delete(newData);
      throw;
    }
    i = 0;
    for (size_t j = first_; j < first_ + size_; ++j)
    {
      data_[j % capacity_].~T();
    }
    operator delete(data_);
    data_ = newData;
    first_ = 0;
    last_ = size_ - 1;
    capacity_ = newCapacity;
  }

  template< class T >
  size_t Deque< T >::getBiggerCapacity(size_t capacity) const noexcept
  {
    if (!capacity)
    {
      return minDequeCapacity;
    }
    if (capacity > maxSize() / 2)
    {
      return capacity;
    }
    return capacity * 2;
  }
}

#endif

