#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP
#include <cstddef>
#include <utility>
#include <iostream>

namespace savintsev
{
  template< typename T >
  class Array
  {
  public:
    class iterator
    {
    public:
      using iterator_category = std::random_access_iterator_tag;
      using difference_type   = std::ptrdiff_t;
      using value_type        = T;
      using pointer           = T*;
      using reference         = T&;

      iterator(pointer ptr) : ptr_(ptr) {}

      reference operator*() const { return *ptr_; }
      pointer operator->() { return ptr_; }

      iterator& operator++() { ++ptr_; return *this; }
      iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

      iterator& operator--() { --ptr_; return *this; }
      iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }

      iterator operator+(difference_type n) const { return iterator(ptr_ + n); }
      iterator operator-(difference_type n) const { return iterator(ptr_ - n); }
      difference_type operator-(const iterator& other) const { return ptr_ - other.ptr_; }

      iterator& operator+=(difference_type n) { ptr_ += n; return *this; }
      iterator& operator-=(difference_type n) { ptr_ -= n; return *this; }

      reference operator[](difference_type n) const { return ptr_[n]; }

      bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }
      bool operator!=(const iterator& other) const { return ptr_ != other.ptr_; }
      bool operator<(const iterator& other) const { return ptr_ < other.ptr_; }
      bool operator>(const iterator& other) const { return ptr_ > other.ptr_; }
      bool operator<=(const iterator& other) const { return ptr_ <= other.ptr_; }
      bool operator>=(const iterator& other) const { return ptr_ >= other.ptr_; }

    private:
      pointer ptr_;
    };
    ~Array();
    Array();
    Array(size_t n);
    Array(const Array & rhs);
    Array(Array && rhs) noexcept;

    Array & operator=(const Array & rhs);
    Array & operator=(Array && rhs) noexcept;

    T & operator[](size_t n);
    const T & operator[](size_t n) const;

    bool empty() const noexcept;
    size_t size() const noexcept;

    const T & front() const;
    T & front();
    const T & back() const;
    T & back();

    template< typename U >
    void push_back(U && rhs);
    void pop_front() noexcept;
    void pop_back() noexcept;

    void swap(Array & x) noexcept;

    iterator begin() { return iterator(data_ + start_); }
    iterator end() { return iterator(data_ + start_ + size_); }

    const iterator begin() const { return iterator(data_ + start_); }
    const iterator end() const { return iterator(data_ + start_ + size_); }

    template <typename InputIt>
    iterator insert(iterator pos, InputIt first, InputIt last)
    {
      size_t count = std::distance(first, last);
      size_t offset = pos - begin();

      if (size_ + count > capacity_)
      {
        size_t new_capacity = std::max(capacity_ * 2, size_ + count);
        T* new_data = new T[new_capacity];

        // Копируем до позиции вставки
        for (size_t i = 0; i < offset; ++i)
        {
          new_data[i] = std::move(data_[start_ + i]);
        }

        // Вставка новых элементов
        size_t i = offset;
        for (; first != last; ++first, ++i)
        {
          new_data[i] = *first;
        }

        // Копируем остальное
        for (size_t j = offset; j < size_; ++j, ++i)
        {
          new_data[i] = std::move(data_[start_ + j]);
        }

        delete[] data_;
        data_ = new_data;
        start_ = 0;
        capacity_ = new_capacity;
        size_ += count;

        return iterator(data_ + offset);
      }
      else
      {
        // Сдвигаем хвост вправо
        for (size_t i = size_; i > offset; --i)
        {
          data_[start_ + i + count - 1] = std::move(data_[start_ + i - 1]);
        }

        // Вставляем
        size_t i = 0;
        for (; first != last; ++first, ++i)
        {
          data_[start_ + offset + i] = *first;
        }

        size_ += count;
        return iterator(data_ + start_ + offset);
      }
    }
    iterator erase(iterator pos)
    {
      if (pos == end()) return pos;

      for (auto it = pos; it + 1 != end(); ++it)
      {
        *it = std::move(*(it + 1));
      }

      --size_;
      return pos;
    }
  private:
    T * data_ = nullptr;
    size_t size_ = 0;
    size_t start_ = 0;
    size_t capacity_ = 0;
  };

  template< typename T >
  void Array< T >::swap(Array & x) noexcept
  {
    std::swap(data_, x.data_);
    std::swap(size_, x.size_);
    std::swap(start_, x.start_);
    std::swap(capacity_, x.capacity_);
  }

  template< typename T >
  Array< T >::Array():
    Array(10)
  {}

  template< typename T >
  Array< T >::Array(size_t n):
    data_(new T[n]),
    size_(0),
    start_(0),
    capacity_(n)
  {}

  template< typename T >
  Array< T >::Array(const Array & rhs):
    data_(new T[rhs.capacity_]),
    size_(rhs.size_),
    start_(rhs.start_),
    capacity_(rhs.capacity_)
  {
    for (size_t i = 0; i < size_; ++i)
    {
      data_[start_ + i] = rhs.data_[rhs.start_ + i];
    }
  }

  template< typename T >
  Array< T >::Array(Array && rhs) noexcept:
    data_(rhs.data_),
    size_(rhs.size_),
    start_(rhs.start_),
    capacity_(rhs.capacity_)
  {
    rhs.data_ = nullptr;
    rhs.size_ = 0;
    rhs.start_ = 0;
    rhs.capacity_ = 0;
  }

  template< typename T >
  Array< T > & Array< T >::operator=(const Array & rhs)
  {
    if (this != std::addressof(rhs))
    {
      Array temp(rhs);
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  Array< T > & Array< T >::operator=(Array && rhs) noexcept
  {
    if (this != std::addressof(rhs))
    {
      Array temp(std::move(rhs));
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  Array< T >::~Array()
  {
    delete[] data_;
  }

  template< typename T >
  T & Array< T >::operator[](size_t n)
  {
    return data_[n + start_];
  }

  template< typename T >
  const T & Array< T >::operator[](size_t n) const
  {
    return data_[n + start_];
  }

  template< typename T >
  bool Array< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  size_t Array< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  const T & Array< T >::front() const
  {
    return data_[start_];
  }

  template< typename T >
  T & Array< T >::front()
  {
    return data_[start_];
  }

  template< typename T >
  const T & Array< T >::back() const
  {
    return data_[start_ + size_ - 1];
  }

  template< typename T >
  T & Array< T >::back()
  {
    return data_[start_ + size_ - 1];
  }

  template< typename T >
  template< typename U >
  void Array< T >::push_back(U && rhs)
  {
    if (size_ + start_ >= capacity_)
    {
      size_t new_capacity = capacity_ ? capacity_ * 2 : 1;
      T * new_data = new T[new_capacity];

      for (size_t i = 0; i < size_; ++i)
      {
        new_data[i] = std::move(data_[start_ + i]);
      }

      delete[] data_;
      data_ = new_data;
      capacity_ = new_capacity;
      start_ = 0;
    }

    data_[start_ + size_] = std::forward< U >(rhs);
    ++size_;
  }

  template< typename T >
  void Array< T >::pop_back() noexcept
  {
    if (size_ > 0)
    {
      --size_;
    }
  }

  template< typename T >
  void Array< T >::pop_front() noexcept
  {
    if (size_ > 0)
    {
      ++start_;
      --size_;
    }
  }
}

#endif
