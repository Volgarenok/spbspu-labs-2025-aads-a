#ifndef STACK_HPP
#define STACK_HPP
#include <stdexcept>
namespace asafov
{
  template <typename T>
  class stack
  {
  public:
    stack():
    items_(nullptr),
    size_(0)
    {}
    ~stack()
    {
      delete[] items_;
    }
    T back() const noexcept
    {
      return items_[size_-1];
    }
    T front() const noexcept
    {
      return items_[0];
    }
    void push(T value)
    {
      T* temp = items_;
      size_++;
      items_ = new T[size_];
      for (size_t i = 0; i < (size_ - 1); i++)
      {
        items_[i] = temp[i];
      }
      items_[size_ - 1] = value;
      delete[] temp;
    }
    T drop()
    {
      size_--;
      T value = items_[size_];
      if (size_ == 0)
      {
        delete[] items_;
      }
      else
      {
        T* temp = items_;
        items_ = new T[size_];
        for (size_t i = 0; i < size_; i++)
        {
          items_[i] = temp[i];
        }
        delete[] temp;
      }
      return value;
    }
    void pop()
    {
      size_--;
      T value = items_[size_];
      if (size_ == 0)
      {
        delete[] items_;
      }
      else
      {
        T* temp = items_;
        items_ = new T[size_];
        for (size_t i = 0; i < size_; i++)
        {
          items_[i] = temp[i];
        }
        delete[] temp;
      }
    }
    size_t size() const noexcept
    {
      return size_;
    }
    bool empty () const noexcept
    {
      return size_ == 0;
    }
  private:
    T* items_;
    size_t size_;
  };
}
#endif
