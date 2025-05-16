#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>
#include <utility>

namespace zholobov {

  template < typename T >
  class Queue {
    static constexpr size_t initial_capacity = 4;

  public:
    Queue();
    Queue(const Queue& other);
    Queue(Queue&& other) noexcept;
    Queue& operator=(const Queue& other);
    Queue& operator=(Queue&& other) noexcept;
    ~Queue();

    void push(const T& value);
    void push(T&& value);
    const T& front() const;
    T& front();
    const T& back() const;
    T& back();
    void pop();
    bool empty() const;
    void clear() noexcept;
    void swap(Queue& other) noexcept;

  private:
    T** data_;
    size_t capacity_;
    size_t head_;
    size_t size_;
  };

}

template < typename T >
zholobov::Queue< T >::Queue():
  data_(nullptr), capacity_(0), head_(0), size_(0)
{}

template < typename T >
zholobov::Queue< T >::Queue(const Queue& other):
  data_(nullptr), capacity_(0), head_(0), size_(0)
{
  if (other.size_ > 0) {
    try {
      data_ = new T*[other.capacity_];
      capacity_ = other.capacity_;
      for (; size_ < other.size_; ++size_) {
        size_t pos = (other.head_ + size_) % other.capacity_;
        data_[size_] = new T(*other.data_[pos]);
      }
    } catch (const std::bad_alloc&) {
      clear();
      throw;
    }
  }
}

template < typename T >
zholobov::Queue< T >::Queue(Queue&& other) noexcept:
  data_(nullptr), capacity_(0), head_(0), size_(0)
{
  swap(other);
}

template < typename T >
zholobov::Queue< T >& zholobov::Queue< T >::operator=(const Queue& other)
{
  if (this != std::addressof(other)) {
    Queue temp(other);
    swap(temp);
  }
  return *this;
}

template < typename T >
zholobov::Queue< T >& zholobov::Queue< T >::operator=(Queue&& other) noexcept
{
  if (this != std::addressof(other)) {
    clear();
    swap(other);
  }
  return *this;
}

template < typename T >
zholobov::Queue< T >::~Queue()
{
  clear();
}

template < typename T >
void zholobov::Queue< T >::push(const T& value)
{
  push(T(value));
}

template < typename T >
void zholobov::Queue< T >::push(T&& value)
{
  if (size_ == capacity_) {
    size_t new_capacity = (capacity_ != 0) ? capacity_ * 2 : initial_capacity;
    T** new_data = new T*[new_capacity];

    for (size_t i = 0; i < size_; ++i) {
      size_t pos = (head_ + i) % capacity_;
      new_data[i] = data_[pos];
    }

    delete[] data_;
    data_ = new_data;
    head_ = 0;
    capacity_ = new_capacity;
  }
  size_t tail = (head_ + size_) % capacity_;
  data_[tail] = new T(std::move(value));
  ++size_;
}

template < typename T >
const T& zholobov::Queue< T >::front() const
{
  if (empty()) {
    throw std::out_of_range("Stack is empty");
  }
  return *data_[head_];
}

template < typename T >
T& zholobov::Queue< T >::front()
{
  if (empty()) {
    throw std::out_of_range("Stack is empty");
  }
  return *data_[head_];
}

template < typename T >
const T& zholobov::Queue< T >::back() const
{
  if (empty()) {
    throw std::out_of_range("Stack is empty");
  }
  size_t tail = (head_ + size_ - 1) % capacity_;
  return *data_[tail];
}

template < typename T >
T& zholobov::Queue< T >::back()
{
  if (empty()) {
    throw std::out_of_range("Stack is empty");
  }
  size_t tail = (head_ + size_ - 1) % capacity_;
  return *data_[tail];
}

template < typename T >
void zholobov::Queue< T >::pop()
{
  if (empty()) {
    throw std::out_of_range("Stack is empty");
  }
  delete data_[head_];
  head_ = (head_ + 1) % capacity_;
  --size_;
}

template < typename T >
bool zholobov::Queue< T >::empty() const
{
  return size_ == 0;
}

template < typename T >
void zholobov::Queue< T >::clear() noexcept
{
  for (size_t i = 0; i < size_; ++i) {
    size_t pos = (head_ + i) % capacity_;
    delete data_[pos];
  }
  size_ = 0;
  if (data_ != nullptr) {
    delete[] data_;
    data_ = nullptr;
    capacity_ = 0;
  }
}

template < typename T >
void zholobov::Queue< T >::swap(Queue& other) noexcept
{
  std::swap(data_, other.data_);
  std::swap(capacity_, other.capacity_);
  std::swap(head_, other.head_);
  std::swap(size_, other.size_);
}

#endif
