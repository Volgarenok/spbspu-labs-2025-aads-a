#ifndef STACK_HPP
#define STACK_HPP
#include <iostream>
#include <stdexcept>
namespace asafov
{
  template<typename T>
  class stack
  {
  public:
    stack():
      data(nullptr),
      capacity(0),
      count(0)
    {}

    ~stack()
    {
      delete[] data;
    }

    void push(const T& value)
    {
      if (count == capacity) resize(capacity == 0 ? 1 : capacity * 2);
      data[count++] = value;
    }

    void pop()
    {
      if (empty()) throw std::out_of_range("stack::pop(): empty stack");
      --count;
    }

    T& top()
    {
      if (empty()) throw std::out_of_range("stack::top(): empty stack");
      return data[count - 1];
    }

    const T& top() const
    {
      if (empty()) throw std::out_of_range("stack::top(): empty stack");
      return data[count - 1];
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
      count = 0;
    }
  private:
    T* data;
    size_t capacity;
    size_t count;

    void resize(size_t new_capacity)
    {
      T* new_data = new T[new_capacity];
      for (size_t i = 0; i < count; ++i) new_data[i] = std::move(data[i]);
      delete[] data;
      data = new_data;
      capacity = new_capacity;
    }
  };
}
#endif
