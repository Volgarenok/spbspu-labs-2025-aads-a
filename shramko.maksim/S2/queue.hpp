#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>

namespace shramko
{
  template <typename T>
  class Queue
  {
  public:
    Queue() noexcept : head_(nullptr), tail_(nullptr), size_(0) {}
    ~Queue();
    Queue(const Queue&);
    Queue(Queue&&) noexcept;
    Queue& operator=(const Queue&);
    Queue& operator=(Queue&&) noexcept;

    void push(const T& data);
    void push(T&& data);
    void pop();
    size_t size() const noexcept;
    bool empty() const noexcept;
    T& front();
    const T& front() const;
    void swap(Queue& other) noexcept;

  private:
    struct Node
    {
      T data;
      Node* next;
      explicit Node(const T& d, Node* n = nullptr) : data(d), next(n) {}
      explicit Node(T&& d, Node* n = nullptr) : data(std::move(d)), next(n) {}
    };
    Node* head_;
    Node* tail_;
    size_t size_;
  };

  template <typename T>
  Queue<T>::~Queue()
  {
    while (head_)
    {
      Node* temp = head_;
      head_ = head_->next;
      delete temp;
    }
  }

  template <typename T>
  Queue<T>::Queue(const Queue& other)
    : head_(nullptr), tail_(nullptr), size_(0)
  {
    Node** current = &head_;
    for (Node* src = other.head_; src; src = src->next)
    {
      *current = new Node(src->data);
      if (!tail_) tail_ = *current;
      current = &(*current)->next;
      ++size_;
    }
  }

  template <typename T>
  Queue<T>& Queue<T>::operator=(const Queue& other)
  {
    if (this != &other)
    {
      Queue temp(other);
      swap(temp);
    }
    return *this;
  }

  template <typename T>
  Queue<T>::Queue(Queue&& other) noexcept
    : head_(other.head_), tail_(other.tail_), size_(other.size_)
  {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }

  template <typename T>
  Queue<T>& Queue<T>::operator=(Queue&& other) noexcept
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
      tail_ = other.tail_;
      size_ = other.size_;
      other.head_ = nullptr;
      other.tail_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  template <typename T>
  void Queue<T>::push(const T& data)
  {
    Node* newNode = new Node(data);
    if (tail_)
    {
      tail_->next = newNode;
    }
    else
    {
      head_ = newNode;
    }
    tail_ = newNode;
    ++size_;
  }

  template <typename T>
  void Queue<T>::push(T&& data)
  {
    Node* newNode = new Node(std::move(data));
    if (tail_)
    {
      tail_->next = newNode;
    }
    else
    {
      head_ = newNode;
    }
    tail_ = newNode;
    ++size_;
  }

  template <typename T>
  void Queue<T>::pop()
  {
    if (!head_)
    {
      throw std::logic_error("Queue is empty");
    }
    Node* temp = head_;
    head_ = head_->next;
    if (!head_)
    {
      tail_ = nullptr;
    }
    delete temp;
    --size_;
  }

  template <typename T>
  size_t Queue<T>::size() const noexcept
  {
    return size_;
  }

  template <typename T>
  bool Queue<T>::empty() const noexcept
  {
    return size_ == 0;
  }

  template <typename T>
  T& Queue<T>::front()
  {
    if (!head_)
    {
      throw std::logic_error("Queue is empty");
    }
    return head_->data;
  }

  template <typename T>
  const T& Queue<T>::front() const
  {
    if (!head_)
    {
      throw std::logic_error("Queue is empty");
    }
    return head_->data;
  }

  template <typename T>
  void Queue<T>::swap(Queue& other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }
}

#endif
