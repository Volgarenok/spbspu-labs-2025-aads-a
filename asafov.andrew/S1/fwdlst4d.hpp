#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H
#include <memory>
namespace asafov
{
  template<typename T>
  class Forward_list
  {
    class Node
    {
    public:
      T data_;
      Node* next_;
      Node() noexcept: data_(), next_(nullptr) {}
      Node(const T& smh, Node* ptr) noexcept: data_(smh), next_(ptr) {}
      Node(const T& smh) noexcept: data_(smh), next_(nullptr) {}
      Node(const Node& node) noexcept: data_(node.data_), next_(node.next_) {}
    };
  public:
    Forward_list() noexcept:
    head_(nullptr),
    last_(nullptr)
    {}
    ~Forward_list()
    {
      clear();
    }

    class const_iterator
    {
      friend class Forward_list;
    public:
      const_iterator():
      current_(nullptr),
      last_(nullptr)
      {}
      const_iterator(Node* node, Node* last):
      current_(node),
      last_(last)
      {}
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
  if (current_)
  {
    current_ = (current_ == last_) ? nullptr : current_->next_;
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
      return const_iterator(head_, last_);
    }
    const_iterator cend() const
    {
      return const_iterator(nullptr, last_);
    }
    const_iterator begin() const
    {
      return const_iterator(head_, last_);
    }
    const_iterator end() const
    {
      return const_iterator(nullptr, last_);
    }
    void push_back(const T& value)
{
  Node* new_node = new Node(value);
  if (!head_)
  {
    head_ = last_ = new_node;
    new_node->next_ = new_node; // Ensure circularity
  }
  else
  {
    last_->next_ = new_node;
    new_node->next_ = head_; // Maintain circular nature
    last_ = new_node;
  }
}
    void swap();
    size_t size() const
{
  if (!head_) return 0;
  size_t i = 1;
  for (Node* it = head_; it != last_; it = it->next_)
  {
    ++i;
  }
  return i;
}
    bool empty() const
    {
      if (size() == 0)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    void pop_front()
{
  if (!head_) return;
  if (head_ == last_)
  {
    delete head_;
    head_ = last_ = nullptr;
  }
  else
  {
    Node* temp = head_;
    head_ = head_->next_;
    last_->next_ = head_; // Ensure last node correctly points to new head
    delete temp;
  }
}
    T& front(){
      return head_->data_;
    };
    T& back(){
      return last_->data_;
    }
    void clear()
{
  while (head_)
  {
    pop_front();
  }
}
  private:
    Node* head_;
    Node* last_;
  };
}
#endif
