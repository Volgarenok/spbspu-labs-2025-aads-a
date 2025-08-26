#ifndef DYNAMICARR_HPP
#define DYNAMICARR_HPP
#include <cstddef>
#include <stdexcept>

namespace finaev
{
  template< class T >
  class DynamicArr
  {
  public:
    DynamicArr();
    explicit DynamicArr(size_t size);
    ~DynamicArr();
    DynamicArr(const DynamicArr< T >&);
    DynamicArr(DynamicArr< T >&&) noexcept;

    void swap(DynamicArr< T >&) noexcept;

    DynamicArr& operator=(const DynamicArr< T >& other);
    DynamicArr& operator=(DynamicArr< T >&& other) noexcept;

    void push(const T&);
    void pop_back();
    void pop_front();
    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    size_t size() const noexcept;
    bool empty() const noexcept;

    const T& front() const;
    T& front();

    const T& back() const;
    T& back();
  private:
    size_t capacity_;
    size_t size_;
    size_t head_;
    T* data_;

    void resize();
  };

  template< class T >
  DynamicArr< T >::DynamicArr():
    capacity_(10),
    size_(0),
    head_(0),
    data_(new T[capacity_])
  {}

  template< class T >
  DynamicArr< T >::DynamicArr(size_t size):
    capacity_(size + 10),
    size_(size),
    head_(0),
    data_(new T[size])
  {}

  template< class T >
  DynamicArr< T >::~DynamicArr()
  {
    delete[] data_;
  }

  template< class T >
  DynamicArr< T >::DynamicArr(const DynamicArr< T >& other):
    capacity_(other.capacity_),
    size_(other.size_),
    head_(other.head_),
    data_(new T[other.capacity_])
  {
    if (capacity_ > 0)
    {
      try
      {
        for (size_t i = 0; i < size_; ++i)
        {
          data_[i] = other.data_[i];
        }
      }
      catch (...)
      {
        delete[] data_;
        throw;
      }
    }
  }

  template< class T >
  DynamicArr< T >::DynamicArr(DynamicArr< T >&& other) noexcept:
    capacity_(other.capacity_),
    size_(other.size_),
    head_(other.head_),
    data_(other.data_)
  {
    other.capacity_ = 0;
    other.size_ = 0;
    other.data_ = nullptr;
  }

  template< class T >
  void DynamicArr< T >::swap(DynamicArr< T >& other) noexcept
  {
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
    std::swap(head_, other.head_);
    std::swap(data_, other.data_);
  }

  template< class T >
  DynamicArr< T >& DynamicArr< T >::operator=(const DynamicArr< T >& other)
  {
    if (this != std::addressof(other))
    {
      DynamicArr< T > temp(other);
      swap(temp);
    }
    return *this;
  }

  template< class T >
  DynamicArr< T >& DynamicArr< T >::operator=(DynamicArr< T >&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      DynamicArr< T > temp(std::move(other));
      swap(temp);
    }
    return *this;
  }

  template< class T >
  void DynamicArr< T >::resize()
  {
    size_t newCapacity = capacity_ * 2;
    T* newData = nullptr;
    try
    {
      newData = new T[newCapacity];
      for (size_t i = 0; i < size_; ++i)
      {
        size_t index = (head_ + i) % capacity_;
        newData[i] = data_[index];
      }
    }
    catch (...)
    {
      delete[] newData;
      throw;
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
    head_ = 0;
  }

  template< class T >
  void DynamicArr< T >::push(const T& value)
  {
    if (size_ == capacity_)
    {
      resize();
    }
    data_[size_ + head_] = value;
    ++size_;
  }

  template< class T >
  void DynamicArr< T >::pop_back()
  {
    if (empty())
    {
      throw std::out_of_range("can not pop, cont is empty!");
    }
    --size_;
  }

  template< class T >
  void DynamicArr< T >::pop_front()
  {
    if (empty())
    {
      throw std::out_of_range("can not pop, cont is empty!");
    }
    head_ = (head_ + 1) % capacity_;
    --size_;
  }

  template< class T >
  T& DynamicArr< T >::operator[](size_t index)
  {
    if (index >= size_)
    {
      throw std::out_of_range("out of range");
    }
    return data_[index];
  }

  template< class T >
  const T& DynamicArr< T >::operator[](size_t index) const
  {
    if (index >= size_)
    {
      throw std::out_of_range("out of range");
    }
    return data_[index];
  }

  template< class T >
  bool DynamicArr< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  size_t DynamicArr< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  const T& DynamicArr< T >::front() const
  {
    return data_[head_];
  }

  template< class T >
  T& DynamicArr< T >::front()
  {
    return data_[head_];
  }

  template< class T >
  const T& DynamicArr< T >::back() const
  {
    return data_[size_ + head_ - 1];
  }

  template< class T >
  T& DynamicArr< T >::back()
  {
    return data_[size_ + head_ - 1];
  }
}

#endif
