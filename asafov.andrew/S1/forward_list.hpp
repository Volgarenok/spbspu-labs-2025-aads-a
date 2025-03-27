#ifndef FORWARD_LIST_6A_HPP
#define FORWARD_LIST_6A_HPP
#include <memory>

namespace asafov
{
  template <typename T>
  class Forward_list
  {
    struct Node
    {
      explicit Node(const T& value, Node* ptr = nullptr) noexcept:
      data_(value),
      next_(ptr)
      {}
      T data_;
      Node* next_;
    };
  public:
    Forward_list() noexcept:
    head_(nullptr),
    tail_(nullptr),
    size_(0)
    {}
    Forward_list(const Forward_list& list):
    Forward_list()
    {
      for (auto i = list.cbegin(); i != list.cend(); ++i)
      {
        push_back(*i);
      }
    }
    Forward_list(Forward_list&& list):
    head_(list.head_),
    tail_(list.tail_),
    size_(list.size_)
    {
      list.head_ = nullptr;
      list.tail_ = nullptr;
      list.size_ = 0;
    }
    ~Forward_list() noexcept
    {
      clear();
    }

    Forward_list& operator=(const Forward_list& list)
    {
      clear();
      for (auto data : list)
      {
        push_back(data);
      }
      return *this;
    }
    Forward_list& operator=(Forward_list&& list)
    {
      Forward_list* temp = this;
      list = temp;
      return temp;
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
      const_iterator(const const_iterator& data) noexcept:
      current_(data.current_),
      last_(data.last_)
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

      const_iterator& operator=(const const_iterator& data) noexcept
      {
        if (this != &data)
        {
          current_ = data.current_;
          last_ = data.last_;
        }
        return *this;
      }

      bool operator==(const const_iterator& rhs) const
      {
        return current_ == rhs.current_ && last_ == rhs.last_;
      }
      bool operator!=(const const_iterator& rhs) const
      {
       return !(*this == rhs);
      }
    private:
      Node* current_;
      Node* last_;
    };
    class iterator
    {
      friend class Forward_list;
    public:
      iterator():
      current_(nullptr),
      last_(nullptr)
      {}
      iterator(Node* node, Node* last):
      current_(node),
      last_(last)
      {}
      iterator(const iterator& data) noexcept:
      current_(data.current_),
      last_(data.last_)
      {}
      ~iterator() = default;

      T& operator*()
      {
        return current_->data_;
      }
      T* operator->()
      {
        return std::addressof(current_->data_);
      }

      iterator& operator++()
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

      iterator& operator=(const iterator& data) noexcept
      {
        if (this != &data)
        {
          current_ = data.current_;
          last_ = data.last_;
        }
        return *this;
      }

      bool operator==(const iterator& rhs) const
      {
        return current_ == rhs.current_ && last_ == rhs.last_;
      }
      bool operator!=(const iterator& rhs) const
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
    iterator begin() const
    {
      return iterator(head_, tail_);
    }
    iterator end() const
    {
      return iterator(nullptr, tail_);
    }

    T& front()
    {
      return head_->data_;
    }
    T& back()
    {
      return tail_->data_;
    }
    bool empty() const
    {
      return !head_;
    }
    size_t size() const
    {
      return size_;
    }
    void swap(const iterator& a, const iterator& b)
    {
      Node* temp1 = head_;
      while (temp1->next_ != a.current_) temp1 = temp1->next;
      Node* temp2 = head_;
      while (temp1->next_ != b.current_) temp1 = temp1->next;
      temp1->next_ = b;
      temp2->next_ = a;
      Node* temp3 = a.current_->next_;
      a.current_->next_ = b.current_->next_;
      b.current_->next_ = temp3;
    }
    void pop_front() noexcept
    {
      size_--;
      Node* temp = head_;
      if (head_ == nullptr)
      {}
      else
      {
        if (head_ == tail_)
        {
          delete head_;
          head_ = nullptr;
          tail_ = nullptr;
        }
        else
        {
          head_ = head_->next_;
          tail_->next_ = head_;
          delete temp;
        }
      }
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
    void clear() noexcept
    {
      while (size_ != 0)
      {
        pop_front();
      }
    }

  private:
    Node* head_;
    Node* tail_;
    size_t size_;
  };
}
#endif
