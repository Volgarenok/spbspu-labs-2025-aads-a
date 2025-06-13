#include "queue.hpp"
#include <stdexcept>

namespace shramko
{
  QueueString::Node::Node(const std::string& d, Node* n):
    data(d),
    next(n)
  {}

  QueueString::Node::Node(std::string&& d, Node* n):
    data(std::move(d)),
    next(n)
  {}

  QueueString::QueueString() noexcept:
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  QueueString::~QueueString()
  {
    while (head_)
    {
      Node* temp = head_;
      head_ = head_->next;
      delete temp;
    }
  }

  void QueueString::push(const std::string& data)
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
    size_++;
  }

  void QueueString::push(std::string&& data)
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
    size_++;
  }

  void QueueString::pop()
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
    size_--;
  }

  size_t QueueString::size() const noexcept
  {
    return size_;
  }

  bool QueueString::empty() const noexcept
  {
    return size_ == 0;
  }

  std::string& QueueString::front()
  {
    if (!head_)
    {
      throw std::logic_error("Queue is empty");
    }
    return head_->data;
  }

  const std::string& QueueString::front() const
  {
    if (!head_)
    {
      throw std::logic_error("Queue is empty");
    }
    return head_->data;
  }

  void QueueString::swap(QueueString& other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }

  QueueOfQueueString::Node::Node(QueueString& d, Node* n):
    data(),
    next(n)
  {
    shramko::QueueString temp;
    while (!d.empty())
    {
      temp.push(d.front());
      d.pop();
    }
    data.swap(temp);
  }

  QueueOfQueueString::QueueOfQueueString() noexcept:
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  QueueOfQueueString::~QueueOfQueueString()
  {
    while (head_)
    {
      Node* temp = head_;
      head_ = head_->next;
      delete temp;
    }
  }

  void QueueOfQueueString::push(QueueString& data)
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
    size_++;
  }

  void QueueOfQueueString::pop()
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
    size_--;
  }

  size_t QueueOfQueueString::size() const noexcept
  {
    return size_;
  }

  bool QueueOfQueueString::empty() const noexcept
  {
    return size_ == 0;
  }

  QueueString& QueueOfQueueString::front()
  {
    if (!head_)
    {
      throw std::logic_error("Queue is empty");
    }
    return head_->data;
  }

  const QueueString& QueueOfQueueString::front() const
  {
    if (!head_)
    {
      throw std::logic_error("Queue is empty");
    }
    return head_->data;
  }

  void QueueOfQueueString::swap(QueueOfQueueString& other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }
}
