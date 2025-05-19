#ifndef FORWARD_LIST_HPP
#define FORWARD_LIST_HPP
#include <memory>
#include <stdexcept>

namespace asafov
{
  template< typename T >
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
    Forward_list(Forward_list&& list) noexcept:
      head_(list.head_),
      tail_(list.tail_),
      size_(list.size_)
    {
      list.head_ = nullptr;
      list.tail_ = nullptr;
      list.size_ = 0;
    }
    Forward_list(const size_t count, const T value):
      Forward_list()
    {
      for (size_t i = 0; i < count; ++i)
      {
        push_back(value);
      }
    }
    ~Forward_list() noexcept
    {
      clear();
    }

    Forward_list& operator=(const Forward_list& list)
    {
      Forward_list temp;
      for (auto&& data: list)
      {
        temp.push_back(data);
      }
      clear();
      swap(temp);
      delete temp;
      return *this;
    }
    Forward_list& operator=(Forward_list&& list) noexcept
    {
      if (this == &list)
      {
        return *this;
      }
      clear();
      head_ = list.head_;
      tail_ = list.tail_;
      size_ = list.size_;
      list.head_ = nullptr;
      list.tail_ = nullptr;
      list.size_ = 0;
      return *this;
    }

    class const_iterator
    {
      friend class Forward_list;
      const_iterator(Node* node, Node* last):
        current_(node),
        last_(last)
      {}
    public:
      const_iterator():
        current_(nullptr),
        last_(nullptr)
      {}
      const_iterator(const const_iterator& data) = default;
      ~const_iterator() = default;

      const T& operator*()
      {
        return current_->data_;
      }
      const T* operator->()
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
      const_iterator& operator++(int)
      {
        current_ = current_ ? current_->next_ : nullptr;
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
      iterator(Node* node, Node* last):
        current_(node),
        last_(last)
      {}
    public:
      iterator():
        current_(nullptr),
        last_(nullptr)
      {}
      iterator(const iterator& data) = default;
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
      iterator& operator++(int)
      {
        current_ = current_ ? current_->next_ : nullptr;
        return *this;
      }

      bool operator==(const iterator& rhs)
      {
        return current_ == rhs.current_ && last_ == rhs.last_;
      }
      bool operator!=(const iterator& rhs)
      {
        return !(*this == rhs);
      }
    private:
      Node* current_;
      Node* last_;
    };

    const_iterator cbegin() const noexcept
    {
      return const_iterator(head_, tail_);
    }
    const_iterator cend() const noexcept
    {
      return const_iterator(nullptr, tail_);
    }
    iterator begin() noexcept
    {
      return iterator(head_, tail_);
    }
    iterator end() noexcept
    {
      return iterator(nullptr, tail_);
    }

    T& front()
    {
      if (head_->data_)
      {
        return head_->data_;
      }
      else
      {
        throw std::logic_error("list is empty!");
      }
    }
    const T& front() const
    {
      if (head_->data_)
      {
        return head_->data_;
      }
      else
      {
        throw std::logic_error("list is empty!");
      }
    }
    T& back()
    {
      if (tail_->data_)
      {
        return tail_->data_;
      }
      else
      {
        throw std::logic_error("list is empty!");
      }
    }
    const T& back() const
    {
      if (tail_->data_)
      {
        return tail_->data_;
      }
      else
      {
        throw std::logic_error("list is empty!");
      }
    }
    bool empty() const noexcept
    {
      return !head_;
    }
    size_t size() const noexcept
    {
      return size_;
    }
    void pop_front() noexcept
    {
      size_--;
      Node* temp = head_;
      if (head_ != nullptr)
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

    Forward_list& swap(Forward_list& list) noexcept
    {
      std::swap(head_, list.head_);
      std::swap(tail_, list.tail_);
      std::swap(size_, list.size_);
      return *this;
    }
    void remove(const T& value) noexcept
    {
      if (!head_)
      {
        return;
      }
      Node* current = head_;
      Node* prev = tail_;
      bool found = false;
      do
      {
        if (current->data_ == value)
        {
          found = true;
          Node* toDelete = current;
          if (current == head_)
          {
            head_ = head_->next_;
            tail_->next_ = head_;
          }
          else if (current == tail_)
          {
            tail_ = prev;
            tail_->next_ = head_;
          }
          else
          {
            prev->next_ = current->next_;
          }
          current = current->next_;
          delete toDelete;
          size_--;
        }
        else
        {
          prev = current;
          current = current->next_;
        }
      }
      while (current != head_ && found && size_ > 0);
      if (size_ == 0)
      {
        head_ = tail_ = nullptr;
      }
    }
    template< class C >
    void remove_if(C condition) noexcept
    {
      if (!head_)
      {
        return;
      }
      Node* current = head_;
      Node* prev = tail_;
      bool found = false;
      do
      {
        if (condition(current->data_))
        {
          found = true;
          Node* toDelete = current;
          if (current == head_)
          {
            head_ = head_->next_;
            tail_->next_ = head_;
          }
          else if (current == tail_)
          {
            tail_ = prev;
            tail_->next_ = head_;
          }
          else
          {
            prev->next_ = current->next_;
          }
          current = current->next_;
          delete toDelete;
          size_--;
        }
        else
        {
          prev = current;
          current = current->next_;
        }
      }
      while (current != head_ && found && size_ > 0);
      if (size_ == 0)
      {
        head_ = tail_ = nullptr;
      }
    }
    void assign(size_t count, const T& value)
    {
      Forward_list temp;
      for (size_t i = 0; i < count; ++i)
      {
        temp.push_back(value);
      }
      clear();
      swap(temp);
    }

  private:
    Node* head_;
    Node* tail_;
    size_t size_;
  };
}
#endif
