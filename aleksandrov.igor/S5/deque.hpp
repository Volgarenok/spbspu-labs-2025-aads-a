#ifndef DEQUE_HPP
#define DEQUE_HPP

#include <utility>
#include <cstddef>
#include <stdexcept>
#include <cassert>

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
    size_t capacity() const noexcept;

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

  private:
    T* data_;
    size_t first_;
    size_t last_;
    size_t size_;
    size_t capacity_;

    T* copyData(const Deque&);
    void resize();
  };

  template< class T >
  Deque< T >::Deque():
    data_(static_cast< T* >(operator new(minDequeCapacity * sizeof(T)))),
    first_(0),
    last_(0),
    size_(0),
    capacity_(minDequeCapacity)
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
    Deque newDeque(rhs);
    swap(newDeque);
    return *this;
  }

  template< class T >
  Deque< T >& Deque< T >::operator=(Deque&& rhs) noexcept
  {
    Deque newDeque(std::move(rhs));
    swap(newDeque);
    return *this;
  }

  template< class T >
  T& Deque< T >::front()
  {
    assert(!empty());
    return data_[first_];
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
    return data_[(last_ - 1 + capacity_) % capacity_];
  }

  template< class T >
  const T& Deque< T >::back() const
  {
    assert(!empty());
    return data_[(last_ - 1 + capacity_) % capacity_];
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
  void Deque< T >::clear() noexcept
  {
    while (!empty())
    {
      popFront();
    }
  }

  template< class T >
  void Deque< T >::pushFront(const T& el)
  {
    emplaceFront(el);
  }

  template< class T >
  void Deque< T >::pushFront(T&& el)
  {
    emplaceFront(el);
  }

  template< class T >
  void Deque< T >::pushBack(const T& el)
  {
    emplaceBack(el);
  }

  template< class T >
  void Deque< T >::pushBack(T&& el)
  {
    emplaceBack(el);
  }

  template< class T >
  template< class... Args >
  void Deque< T >::emplaceFront(Args&&... args)
  {
    if (size_ == capacity_)
    {
      resize();
    }
    first_ = (first_ - 1 + capacity_) % capacity_;
    new (data_ + first_) T(std::forward< Args >(args)...);
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
    new (data_ + last_) T(std::forward< Args >(args)...);
    last_ = (last_ + 1) % capacity_;
    ++size_;
  }

  template< class T >
  void Deque< T >::popFront()
  {
    assert(!empty());
    data_[first_].~T();
    first_ = (first_ + 1) % capacity_;
    --size_;
  }

  template< class T >
  void Deque< T >::popBack()
  {
    assert(!empty());
    last_ = (last_ - 1 + capacity_) % capacity_;
    data_[last_].~T();
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
  T* Deque< T >::copyData(const Deque& rhs)
  {
    T* copy = static_cast< T* >(operator new(rhs.capacity_ * sizeof(T)));
    size_t i = 0;
    try
    {
      for (; i < rhs.size_; ++i)
      {
        new (copy + i) T(rhs.data_[(rhs.first_ + i) % rhs.capacity_]);
      }
    }
    catch (const std::bad_alloc&)
    {
      for (size_t j = 0; j < i; ++j)
      {
        (copy + j)->~T();
      }
      operator delete(copy);
      throw;
    }
    return copy;
  }

  template< class T >
  void Deque< T >::resize()
  {
    size_t newCapacity = capacity_ * 2;
    T* newData = static_cast< T* >(operator new(newCapacity * sizeof(T)));
    size_t i = 0;
    try
    {
      for (; i < size_ - first_; ++i)
      {
        new (newData + i) T(std::move_if_noexcept(data_[i]));
      }
      for (size_t j = 0; j < first_; ++j, ++i)
      {
        new (newData + i) T(std::move_if_noexcept(data_[j]));
      }
    }
    catch (const std::bad_alloc&)
    {
      for (size_t j = 0; j < i; ++j)
      {
        newData[j].~T();
      }
      operator delete(newData);
      throw;
    }
    clear();
    operator delete(data_);
    data_ = newData;
    first_ = 0;
    last_ = capacity_;
    size_ = capacity_;
    capacity_ = newCapacity;
  }
}

#endif

