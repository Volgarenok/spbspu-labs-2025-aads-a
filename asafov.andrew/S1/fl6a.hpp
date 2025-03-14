#ifndef FL6A_HPP
#define FL6A_HPP
#include <memory>

template <typename T>
class forward_list
{
  struct Node
  {
    T data_;
    Node* next_;
    explicit Node(const T& value, Node* ptr = nullptr) noexcept:
    data_(value),
    next_(ptr)
    {}
  };
  Node* head_;
  Node* tail_;
  size_t size_;
public:
  forward_list() noexcept:
  head_(nullptr),
  tail_(nullptr),
  size_(0)
  {}

  class const_iterator
  {
    friend class forward_list;
  public:
    const_iterator():
    current_(nullptr),
    last_(nullptr)
    {}
    const_iterator(Node* node, Node* last):
    current_(node),
    last_(last)
    {}
    const_iterator(const const_iterator& data) noexcept:
    current_(data.current_),
    last_(data.last_)
    {}
    const_iterator& operator=(const const_iterator& data) noexcept
    {
      if (this != &data)
      {
        current_ = data.current_;
        last_ = data.last_;
      }
      return *this;
    }
    ~const_iterator() = default;

    const T& operator*() const
    {
      return current_->data_;
    }

    T* operator->() const
    {
      return std::addressof(current_->data_);
    }

    const_iterator& operator++()
    {
      if (current_ && current_ != last_)
      {
        current_ = current_->next_;
      }
      else
      {
        current_ = nullptr;
      }
      return *this;
    }

    bool operator==(const const_iterator& rhs) const
    {
      return current_ == rhs.current_;
    }

    bool operator!=(const const_iterator& rhs) const
    {
      return !(*this == rhs);
    }
  private:
    Node* current_;
    Node* last_;
  };

  const_iterator cbegin() const
  {
    return const_iterator(head_, tail_);
  }
  const_iterator cend() const
  {
    return const_iterator(nullptr, tail_);
  }

  bool empty() const
  {
    return !head_;
  }
  size_t size() const
  {
    return size_;
  }

  void pop_front()
  {
    if (head_ == nullptr)
    {
      return;
    }
    Node* temp = head_;
    if (head_ == tail_)
    {
      head_ = nullptr;
      tail_ = nullptr;
    }
    else
    {
      head_ = head_->next_;
      tail_->next_ = head_;
    }
    delete temp;
    size_--;
  }

  void push_back(const T& value)
  {
    Node* new_node = new Node(value);
    if (!head_)
    {
      head_ = new_node;
      tail_ = new_node;
      new_node->next_ = head_;
    }
    else
    {
      tail_->next_ = new_node;
      new_node->next_ = head_;
      tail_ = new_node;
    }
    size_++;
  }
};
#endif
