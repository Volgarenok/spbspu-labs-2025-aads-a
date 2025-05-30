#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <stdexcept>

namespace karnauhova
{
  template< typename T >
  class Queue
  {
  public:
    Queue();
    Queue(const Queue& rhs);
    Queue(Queue&& rhs);
    Queue< T >& operator=(const Queue< T >& rhs);
    ~Queue();

    bool empty() const;
    size_t size() const;

    void pop();
    void push(const T& val);
    T& back();
    const T& back() const;
    T& front();
    const T& front() const;

    void swap(Queue< T >& other) noexcept;
  private:
    size_t size_;
    size_t count_element_;
    T* data_;

    void resize(size_t add_size);
  };

  template< typename T >
  Queue< T >::Queue():
    size_(5),
    count_element_(0),
    data_(new T[5])
  {}


  template< typename T >
  Queue< T >::Queue(const Queue& rhs):
    size_(rhs.size_),
    count_element_(rhs.count_element_),
    data_(new T[rhs.size_])
  {
    for (size_t i = 0; i < size_; i++)
    {
      data_[i] = rhs.data_[i];
    }
  }

  template< typename T >
  Queue< T >::Queue(Queue&& rhs):
    size_(rhs.size_),
    count_element_(rhs.count_element_),
    data_(rhs.data_)
  {
    rhs.size_ = 0;
    rhs.count_element_ = 0;
    rhs.data_ = nullptr;
  }

  template< typename T >
  Queue< T >& Queue< T >::operator=(const Queue< T >& rhs)
  {
    if (this != &rhs)
    {
      Queue< T > temp(rhs);
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  Queue< T >::~Queue()
  {
    delete[] data_;
  }

  template< typename T >
  bool Queue< T >::empty() const
  {
    return count_element_ == 0;
  }

  template< typename T >
  size_t Queue< T >::size() const
  {
    return count_element_;
  }

  template< typename T >
  void Queue< T >::pop()
  {
    if (empty())
    {
      throw std::logic_error("empty queue for pop");
    }
    for (size_t i = 1; i < count_element_; i++)
    {
      data_[i - 1] = data_[i];
    }
    count_element_--;
  }

  template< typename T >
  void Queue< T >::push(const T& val)
  {
    if (count_element_ == size_)
    {
      resize(1);
    }
      data_[count_element_++] = val;
    }

  template< typename T >
  T& Queue< T >::back()
  {
    if (empty())
    {
      throw std::logic_error("empty queue for back");
    }
    return data_[count_element_ - 1];
  }

  template< typename T >
  const T& Queue< T >::back() const
  {
    return back();
  }

  template< typename T >
  T& Queue< T >::front()
  {
    if (empty())
    {
      throw std::logic_error("empty queue for front");
    }
    return data_[0];
  }

  template< typename T >
  const T& Queue< T >::front() const
  {
    return front();
  }

  template< typename T >
  void Queue< T >::swap(Queue< T >& other) noexcept
  {
    std::swap(size_, other.size_);
    std::swap(count_element_, other.count_element_);
    std::swap(data_, other.data_);
  }

  template< typename T >
  void Queue< T >::resize(size_t add_size)
  {
    T* new_data = new T[size_ + add_size];
    for (size_t i = 0; i < size_; i++)
    {
      new_data[i] = data_[i];
    }
    size_ += add_size;
    delete[] data_;
    data_ = new_data;
  }
}

#endif
