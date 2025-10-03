#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>
#include <algorithm>
#include <stdexcept>

namespace abramov
{
  template< class T >
  struct Queue
  {
    Queue();
    Queue(const Queue< T > &queue);
    Queue(Queue< T > &&queue);
    ~Queue();
    Queue< T > &operator=(const Queue< T > &queue);
    Queue< T > &operator=(Queue< T > &&queue);
    void push(T rhs);
    T &front();
    const T &front() const;
    void pop();
    size_t size() const;
    bool empty() const;
    void swap(Queue< T > &queue) noexcept;
  private:
    T *data_;
    size_t size_;
    size_t capacity_;
  };

  namespace
  {
    template< class T >
    T *expandArr(T *data, size_t &capacity)
    {
      constexpr size_t k = 100;
      T *array = new T[capacity + k];
      for (size_t i = 0; i < capacity; ++i)
      {
        array[i] = data[i];
      }
      capacity += k;
      return array;
    }
  }

  template< class T >
  Queue< T >::Queue():
    data_(nullptr),
    size_(0),
    capacity_(0)
  {}

  template< class T >
  Queue< T >::Queue(const Queue< T > &queue):
    data_(nullptr),
    size_(0),
    capacity_(0)
  {
    T *data = new T[queue.capacity_];
    try
    {
      for (size_t i = 0; i < queue.size_; ++i)
      {
        data[i] = queue.data_[i];
      }
    }
    catch (const std::exception &)
    {
        delete[] data;
        throw;
    }
    data_ = data;
    size_ = queue.size_;
    capacity_ = queue.capacity_;
  }

  template< class T >
  Queue< T > &Queue< T >::operator=(const Queue< T > &queue)
  {
    Queue< T > copy(queue);
    delete[] data_;
    swap(copy);
    return *this;
  }

  template< class T >
  Queue< T >::Queue(Queue< T > &&queue):
    data_(queue.data_),
    size_(queue.size_),
    capacity_(queue.capacity_)
  {
    queue.data_ = nullptr;
  }

  template< class T >
  Queue< T > &Queue< T >::operator=(Queue< T > &&queue)
  {
    Queue< T > copy(queue);
    delete[] data_;
    swap(copy);
    return *this;
  }

  template< class T >
  Queue< T >::~Queue()
  {
    delete[] data_;
  }

  template< class T >
  void Queue< T >::push(T rhs)
  {
    if (capacity_ == size_)
    {
      T *new_data = expandArr(data_, capacity_);
      delete[] data_;
      data_ = new_data;
    }
    data_[size_] = rhs;
    ++size_;
  }

  template< class T >
  const T &Queue< T >::front() const
  {
    return data_[0];
  }

  template< class T >
  T &Queue< T >::front()
  {
    return const_cast< T& >(const_cast< const Queue< T >* >(this)->front());
  }

  template< class T >
  void Queue< T >::pop()
  {
    if (empty())
    {
      throw std::logic_error("Queue is empty\n");
    }
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i] = data_[i + 1];
    }
    --size_;
  }

  template< class T >
  size_t Queue< T >::size() const
  {
    return size_;
  }

  template< class T >
  bool Queue< T >::empty() const
  {
    return size_ == 0;
  }

  template< class T >
  void Queue< T >::swap(Queue< T > &queue) noexcept
  {
    std::swap(data_, queue.data_);
    std::swap(size_, queue.size_);
    std::swap(capacity_, queue.capacity_);
  }
}
#endif
