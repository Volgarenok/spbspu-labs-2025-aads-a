#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include <utility>

namespace zholobov {

  template < typename T >
  class Stack {
    static constexpr size_t initial_capacity = 4;

  public:
    Stack();
    Stack(const Stack& other);
    Stack(Stack&& other) noexcept;
    Stack& operator=(const Stack& other);
    Stack& operator=(Stack&& other) noexcept;
    ~Stack();
    void push(const T& value);
    void push(T&& value);
    void pop();
    T& top();
    const T& top() const;
    bool empty() const noexcept;
    void clear() noexcept;
    void swap(Stack& other) noexcept;

  private:
    T** data_;
    size_t size_;
    size_t capacity_;
  };
}

template < typename T >
zholobov::Stack< T >::Stack():
  data_(nullptr), size_(0), capacity_(0)
{}

template < typename T >
zholobov::Stack< T >::Stack(const Stack& other):
  data_(nullptr), size_(0), capacity_(0)
{
  if (other.size_ > 0) {
    try {
      data_ = new T*[other.capacity_];
      capacity_ = other.capacity_;
      for (; size_ < other.size_; ++size_) {
        data_[size_] = new T(*other.data_[size_]);
      }
    } catch (const std::bad_alloc&) {
      clear();
      throw;
    }
  }
}

template < typename T >
zholobov::Stack< T >::Stack(Stack&& other) noexcept:
  data_(nullptr), size_(0), capacity_(0)
{
  swap(other);
}

template < typename T >
zholobov::Stack< T >& zholobov::Stack< T >::operator=(const Stack& other)
{
  if (this != std::addressof(other)) {
    Stack temp(other);
    swap(temp);
  }
  return *this;
}

template < typename T >
zholobov::Stack< T >& zholobov::Stack< T >::operator=(Stack&& other) noexcept
{
  if (this != std::addressof(other)) {
    clear();
    swap(other);
  }
  return *this;
}

template < typename T >
zholobov::Stack< T >::~Stack()
{
  clear();
}

template < typename T >
void zholobov::Stack< T >::push(const T& value)
{
  push(T(value));
}

template < typename T >
void zholobov::Stack< T >::push(T&& value)
{
  if (size_ == capacity_) {
    size_t new_capacity = (capacity_ != 0) ? capacity_ * 2 : initial_capacity;
    T** new_data = new T*[new_capacity];
    for (size_t i = 0; i < size_; ++i) {
      new_data[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
  }
  data_[size_++] = new T(std::move(value));
}

template < typename T >
void zholobov::Stack< T >::pop()
{
  if (empty()) {
    throw std::out_of_range("Stack is empty");
  }
  delete data_[--size_];
}

template < typename T >
T& zholobov::Stack< T >::top()
{
  if (empty()) {
    throw std::out_of_range("Stack is empty");
  }
  return *data_[size_ - 1];
}

template < typename T >
const T& zholobov::Stack< T >::top() const
{
  if (empty()) {
    throw std::out_of_range("Stack is empty");
  }
  return *data_[size_ - 1];
}

template < typename T >
bool zholobov::Stack< T >::empty() const noexcept
{
  return size_ == 0;
}

template < typename T >
void zholobov::Stack< T >::clear() noexcept
{
  for (size_t i = 0; i < size_; ++i) {
    delete data_[i];
  }
  size_ = 0;
  if (data_ != nullptr) {
    delete[] data_;
    data_ = nullptr;
    capacity_ = 0;
  }
}

template < typename T >
void zholobov::Stack< T >::swap(Stack& other) noexcept
{
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

#endif
