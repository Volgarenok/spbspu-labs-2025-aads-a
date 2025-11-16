#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <stdexcept>
const int QUEUE_SIZE = 100;
namespace hismatova
{
  template< typename T >
  class Queue
  {
  private:
    T data[QUEUE_SIZE];
    size_t front, rear, size;
  public:
    Queue();
    Queue(const Queue& other);
    Queue& operator=(const Queue& other);
    Queue(Queue&& other) noexcept;
    Queue& operator=(Queue&& other) noexcept;
    void push(const T& value);
    T pop();
    bool empty() const;
  };
  template< typename T >
  Queue< T >::Queue() : front(0), rear(0), size(0) {}
  template< typename T >
  Queue< T >::Queue(const Queue& other)
  {
    front = other.front;
    rear = other.rear;
    size = other.size;
    for (size_t i = 0; i < size; ++i)
    {
      data[i] = other.data[i];
    }
  }
  template< typename T >
  Queue< T >& Queue< T >::operator=(const Queue& other)
  {
    if (this != &other)
    {
      front = other.front;
      rear = other.rear;
      size = other.size;
      for (size_t i = 0; i < size; ++i)
      {
        data[i] = other.data[i];
      }
    }
    return *this;
  }
  template< typename T >
  Queue< T >::Queue(Queue&& other) noexcept : front(other.front), rear(other.rear), size(other.size)
  {
    for (size_t i = 0; i < size; ++i)
    {
      data[i] = std::move(other.data[i]);
    }
    other.size = 0;
  }
  template< typename T >
  Queue< T >& Queue< T >::operator=(Queue&& other) noexcept
  {
    if (this != &other)
    {
      front = other.front;
      rear = other.rear;
      size = other.size;
      for (size_t i = 0; i < size; ++i)
      {
        data[i] = std::move(other.data[i]);
      }
      other.size = 0;
    }
    return *this;
  }
  template< typename T >
  void Queue< T >::push(const T& value)
  {
    if (size >= QUEUE_SIZE)
    {
      throw std::overflow_error("Queue overflow");
    }
    data[rear] = value;
    rear = (rear + 1) % QUEUE_SIZE;
    size++;
  }
  template< typename T >
  T Queue< T >::pop()
  {
    if (size == 0)
    {
      throw std::underflow_error("Queue underflow");
    }
    T value = data[front];
    front = (front + 1) % QUEUE_SIZE;
    size--;
    return value;
  }
  template< typename T >
  bool Queue< T >::empty() const
  {
    return size == 0;
  }
}
#endif
