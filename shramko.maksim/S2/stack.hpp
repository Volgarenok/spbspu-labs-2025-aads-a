#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>

namespace shramko
{
  template <typename T>
  class Stack
  {
  public:
    Stack() noexcept : head_(nullptr), size_(0) {}
    ~Stack();
    Stack(const Stack&);
    Stack(Stack&&) noexcept;
    Stack& operator=(const Stack&);
    Stack& operator=(Stack&&) noexcept;

    void push(const T& data);
    void push(T&& data);
    void pop();
    size_t size() const noexcept;
    bool empty() const noexcept;
    T& top();
    const T& top() const;
    void swap(Stack& other) noexcept;

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

  template <typename T>
  Stack<T>::~Stack()
  {
    while (head_)
    {
      Node* temp = head_;
      head_ = head_->next;
      delete temp;
    }
  }

  template <typename T>
  Stack<T>::Stack(const Stack& other)
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

  template <typename T>
  Stack<T>& Stack<T>::operator=(const Stack& other)
  {
    if (this != &other)
    {
      Stack temp(other);
      swap(temp);
    }
    return *this;
  }

  template <typename T>
  Stack<T>::Stack(Stack&& other) noexcept
    : head_(other.head_), size_(other.size_)
  {
    other.head_ = nullptr;
    other.size_ = 0;
  }

  template <typename T>
  Stack<T>& Stack<T>::operator=(Stack&& other) noexcept
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

  template <typename T>
  void Stack<T>::push(const T& data)
  {
    head_ = new Node(data, head_);
    ++size_;
  }

  template <typename T>
  void Stack<T>::push(T&& data)
  {
    head_ = new Node(std::move(data), head_);
    ++size_;
  }

  template <typename T>
  void Stack<T>::pop()
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

  template <typename T>
  size_t Stack<T>::size() const noexcept
  {
    return size_;
  }

  template <typename T>
  bool Stack<T>::empty() const noexcept
  {
    return size_ == 0;
  }

  template <typename T>
  T& Stack<T>::top()
  {
    if (!head_)
    {
      throw std::logic_error("Stack is empty");
    }
    return head_->data;
  }

  template <typename T>
  const T& Stack<T>::top() const
  {
    if (!head_)
    {
      throw std::logic_error("Stack is empty");
    }
    return head_->data;
  }

  template <typename T>
  void Stack<T>::swap(Stack& other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(size_, other.size_);
  }
}

#endif
