#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>

namespace shramko
{
  template < typename T >
  class Stack
  {
  public:
    Stack() noexcept : head_(nullptr), size_(0) {}
    ~Stack()
    {
      while (head_)
      {
        Node* temp = head_;
        head_ = head_->next;
        delete temp;
      }
    }
    Stack(const Stack& other)
      : head_(nullptr), size_(0)
    {
      Node** current = &head_;
      for (Node* src = other.head_; src; src = src->next)
      {
        *current = new Node(src->data);
        current = &(*current)->next;
        ++size_;
      }
    }
    Stack(Stack&& other) noexcept
      : head_(other.head_), size_(other.size_)
    {
      other.head_ = nullptr;
      other.size_ = 0;
    }
    Stack& operator=(const Stack& other)
    {
      if (this != &other)
      {
        Stack temp(other);
        swap(temp);
      }
      return *this;
    }
    Stack& operator=(Stack&& other) noexcept
    {
      if (this != &other)
      {
        while (head_)
        {
          Node* temp = head_;
          head_ = head_->next;
          delete temp;
        }
        head_ = other.head_;
        size_ = other.size_;
        other.head_ = nullptr;
        other.size_ = 0;
      }
      return *this;
    }

    void push(const T& data)
    {
      head_ = new Node(data, head_);
      ++size_;
    }
    void push(T&& data)
    {
      head_ = new Node(std::move(data), head_);
      ++size_;
    }
    void pop()
    {
      if (!head_)
      {
        throw std::logic_error("Stack is empty");
      }
      Node* temp = head_;
      head_ = head_->next;
      delete temp;
      --size_;
    }
    size_t size() const noexcept
    {
      return size_;
    }
    bool empty() const noexcept
    {
      return size_ == 0;
    }
    T& top()
    {
      if (!head_)
      {
        throw std::logic_error("Stack is empty");
      }
      return head_->data;
    }
    const T& top() const
    {
      if (!head_)
      {
        throw std::logic_error("Stack is empty");
      }
      return head_->data;
    }
    void swap(Stack& other) noexcept
    {
      std::swap(head_, other.head_);
      std::swap(size_, other.size_);
    }

  private:
    struct Node
    {
      T data;
      Node* next;
      explicit Node(const T& d, Node* n = nullptr) : data(d), next(n) {}
      explicit Node(T&& d, Node* n = nullptr) : data(std::move(d)), next(n) {}
    };
    Node* head_;
    size_t size_;
  };
}

#endif
