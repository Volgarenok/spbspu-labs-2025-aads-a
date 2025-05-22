#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <iostream>
#include <stdexcept>
namespace asafov
{
  template<typename T>
  class queue
  {
  public:
    queue():
      data(nullptr),
      capacity(0),
      count(0),
      front_index(0),
      back_index(0)
    {}

    ~queue()
    {
      delete[] data;
    }

    void push(const T& value)
    {
      if (count == capacity) resize(capacity == 0 ? 1 : capacity * 2);
      data[back_index] = value;
      back_index = (back_index + 1) % capacity;
      ++count;
    }

    void pop()
    {
      if (empty()) throw std::out_of_range("queue::pop(): empty queue");
      front_index = (front_index + 1) % capacity;
      --count;
    }

    T& front()
    {
      if (empty()) throw std::out_of_range("queue::front(): empty queue");
      return data[front_index];
    }

    const T& front() const
    {
      if (empty()) throw std::out_of_range("queue::front(): empty queue");
      return data[front_index];
    }

    bool empty() const
    {
      return count == 0;
    }

    size_t size() const
    {
      return count;
    }

    void clear()
    {
      front_index = 0;
      back_index = 0;
      count = 0;
    }
  private:
    T* data;
    size_t capacity;
    size_t count;
    size_t front_index;
    size_t back_index;

    void resize(size_t new_capacity)
    {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < count; ++i) new_data[i] = std::move(data[(front_index + i) % capacity]);
        delete[] data;
        data = new_data;
        front_index = 0;
        back_index = count;
        capacity = new_capacity;
    }
  };
}
#endif
