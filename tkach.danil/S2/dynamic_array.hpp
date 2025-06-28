#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <utility>
#include <cstddef>

namespace
{
  template< typename T >
  T* getDataFromOther(const T* data, const size_t begin, const size_t size, const size_t capacity)
  {
    T* new_data = reinterpret_cast< T* >(new char[capacity * sizeof(T)]);
    size_t i = 0;
    try
    {
      for (; i < size; ++i)
      {
        new (new_data + i) T(data[i + begin]);
      }
    }
    catch (...)
    {
      for (size_t j = 0; j < i; ++j)
      {
        new_data[j].~T();
      }
      delete [] reinterpret_cast< char* >(new_data);
      throw;
    }
    return new_data;
  }
}

namespace tkach
{
  template< typename T >
  class DynArray
  {
  public:
    DynArray();
    DynArray(const DynArray& other);
    DynArray(DynArray&& other) noexcept;
    ~DynArray();
    DynArray< T >& operator=(DynArray< T >&&) noexcept;
    DynArray< T >& operator=(const DynArray< T >&);
    void pushBack(T&& data);
    void pushBack(const T& data);
    void popBack();
    void popFront();
    size_t size() const noexcept;
    bool empty() const noexcept;
    T& back();
    const T& back() const;
    T& front();
    const T& front() const;
    void swap(DynArray& other) noexcept;
  private:
    size_t size_;
    size_t capacity_;
    size_t begin_;
    T* data_;
    template< class... Args >
    void push(Args&&... data);
    void realloc();
    void clear();
  };

  template< typename T >
  DynArray< T >::DynArray():
    size_(0),
    capacity_(5),
    begin_(0),
    data_(reinterpret_cast< T* >(new char[capacity_ * sizeof(T)]))
  {}

  template< typename T >
  void DynArray< T >::clear()
  {
    if (data_ == nullptr)
    {
      return;
    }
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i + begin_].~T();
    }
    if (data_)
    {
      delete [] reinterpret_cast< char* >(data_);
    }
    data_ = nullptr;
  }

  template< typename T >
  void DynArray< T >::realloc()
  {
    T* temp = reinterpret_cast< T* >(new char[capacity_ * 2 * sizeof(T)]);
    for (size_t i = 0; i < size_; ++i)
    {
      new (temp + i) T(std::move(data_[i + begin_]));
    }
    clear();
    data_ = temp;
    capacity_ *= 2;
    begin_ = 0;
  }

  template< typename T >
  DynArray< T >::DynArray(const DynArray& other):
    size_(other.size_),
    capacity_(other.capacity_),
    begin_(0),
    data_(getDataFromOther(other.data_, other.begin_, other.size_, other.capacity_))
  {}

  template< typename T >
  DynArray< T >::DynArray(DynArray&& other) noexcept:
    size_(other.size_),
    capacity_(other.capacity_),
    begin_(other.begin_),
    data_(other.data_)
  {
    other.capacity_ = 0;
    other.begin_ = 0;
    other.size_ = 0;
    other.data_ = nullptr;
  }

  template< typename T >
  DynArray< T >::~DynArray()
  {
    clear();
  }

  template< typename T >
  DynArray< T >& DynArray< T >::operator=(DynArray< T >&& other) noexcept
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    DynArray< T > temp(std::move(other));
    swap(temp);
    return *this;
  }

  template< typename T >
  DynArray< T >& DynArray< T >::operator=(const DynArray< T >& other)
  {
    if (this == std::addressof(other))
    {
      return *this;
    }
    DynArray< T > temp(other);
    swap(temp);
    return *this;
  }

  template< typename T >
  void DynArray< T >::pushBack(T&& data)
  {
    push(std::move(data));
  }

  template< typename T >
  void DynArray< T >::pushBack(const T& data)
  {
    push(data);
  }

  template< typename T >
  template< class... Args >
  void DynArray< T >::push(Args&&... data)
  {
    if (size_ + begin_ == capacity_)
    {
      realloc();
    }
    new (data_ + size_ + begin_) T(std::forward< Args >(data)...);
    size_++;
  }

  template< typename T >
  bool DynArray< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  size_t DynArray< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  void DynArray< T >::swap(DynArray< T >& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(begin_, other.begin_);
  }

  template< typename T >
  T& DynArray< T >::back()
  {
    return const_cast< T& >(static_cast< const DynArray< T >* >(this)->back());
  }

  template< typename T >
  const T& DynArray< T >::back() const
  {
    return data_[begin_ + size_ - 1];
  }

  template< typename T >
  T& DynArray< T >::front()
  {
    return const_cast< T& >(static_cast< const DynArray< T >* >(this)->front());
  }

  template< typename T >
  void DynArray< T >::popFront()
  {
    data_[begin_].~T();
    size_--;
    begin_++;
  }

  template< typename T >
  void DynArray< T >::popBack()
  {
    data_[begin_ + size_ - 1].~T();
    size_--;
  }

  template< typename T >
  const T& DynArray< T >::front() const
  {
    return data_[begin_];
  }

}
#endif
