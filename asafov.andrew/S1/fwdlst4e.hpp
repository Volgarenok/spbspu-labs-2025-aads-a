#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <memory>
#include <stdexcept>

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
        if (!current_) throw std::runtime_error("Dereferencing null iterator");
        return current_->data_;
      }
      T* operator->() const
      {
        if (!current_) throw std::runtime_error("Dereferencing null iterator");
        return std::addressof(current_->data_);
      }
      const_iterator& operator++()
      {
        if (current_)
        {
          if (current_ == last_)
          {
            current_ = nullptr; // Конец списка
          }
          else
          {
            current_ = current_->next_;
          }
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
        head_ = new_node;
        last_ = new_node;
      }
      else
      {
        last_->next_ = new_node;
        last_ = new_node;
      }
    }

    void push_back(T&& value)
    {
      Node* new_node = new Node(std::move(value));
      if (!head_)
      {
        head_ = new_node;
        last_ = new_node;
      }
      else
      {
        last_->next_ = new_node;
        last_ = new_node;
      }
    }

    size_t size() const
    {
      size_t i = 0;
      for (const_iterator it = cbegin(); it != cend(); ++it)
      {
        ++i;
      }
      return i;
    }

    bool empty() const
    {
      return head_ == nullptr;
    }

    void pop_front()
    {
      if (!head_) return;
      Node* temp = head_;
      head_ = head_->next_;  // Move head to the next node
      if (!head_) last_ = nullptr; // If list is now empty, update last_
      delete temp;
    }

    T& front()
    {
      if (!head_) throw std::runtime_error("List is empty");
      return head_->data_;
    }

    T& back()
    {
      if (!last_) throw std::runtime_error("List is empty");
      return last_->data_;
    }

    void clear()
    {
      while (!empty())
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
