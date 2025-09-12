#include "stack.hpp"
#include <stdexcept>

namespace shramko
{
  StackString::Node::Node(const std::string& d, Node* n):
    data(d),
    next(n)
  {}

  StackString::Node::Node(std::string&& d, Node* n):
    data(std::move(d)),
    next(n)
  {}

  StackString::StackString() noexcept:
    head_(nullptr),
    size_(0)
  {}

  StackString::~StackString()
  {
    while (head_)
    {
      Node* temp = head_;
      head_ = head_->next;
      delete temp;
    }
  }

  void StackString::push(const std::string& data)
  {
    head_ = new Node(data, head_);
    size_++;
  }

  void StackString::push(std::string&& data)
  {
    head_ = new Node(std::move(data), head_);
    size_++;
  }

  void StackString::pop()
  {
    if (!head_)
    {
      throw std::logic_error("Stack is empty");
    }
    Node* temp = head_;
    head_ = head_->next;
    delete temp;
    size_--;
  }

  size_t StackString::size() const noexcept
  {
    return size_;
  }

  bool StackString::empty() const noexcept
  {
    return size_ == 0;
  }

  std::string& StackString::top()
  {
    if (!head_)
    {
      throw std::logic_error("Stack is empty");
    }
    return head_->data;
  }

  const std::string& StackString::top() const
  {
    if (!head_)
    {
      throw std::logic_error("Stack is empty");
    }
    return head_->data;
  }

  void StackString::swap(StackString& other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(size_, other.size_);
  }

  StackLongLong::Node::Node(long long d, Node* n):
    data(d),
    next(n)
  {}

  StackLongLong::StackLongLong() noexcept:
    head_(nullptr),
    size_(0)
  {}

  StackLongLong::~StackLongLong()
  {
    while (head_)
    {
      Node* temp = head_;
      head_ = head_->next;
      delete temp;
    }
  }

  void StackLongLong::push(long long data)
  {
    head_ = new Node(data, head_);
    size_++;
  }

  void StackLongLong::pop()
  {
    if (!head_)
    {
      throw std::logic_error("Stack is empty");
    }
    Node* temp = head_;
    head_ = head_->next;
    delete temp;
    size_--;
  }

  size_t StackLongLong::size() const noexcept
  {
    return size_;
  }

  bool StackLongLong::empty() const noexcept
  {
    return size_ == 0;
  }

  long long& StackLongLong::top()
  {
    if (!head_)
    {
      throw std::logic_error("Stack is empty");
    }
    return head_->data;
  }

  const long long& StackLongLong::top() const
  {
    if (!head_)
    {
      throw std::logic_error("Stack is empty");
    }
    return head_->data;
  }

  void StackLongLong::swap(StackLongLong& other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(size_, other.size_);
  }
}
