#ifndef FORWARD_LIST_HPP
#define FORWARD_LIST_HPP
#include <memory>
#include <stdexcept>

namespace asafov
{
  template< class T , class Comparator = std::less< T > >
  class ForwardList
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

    struct Equal
    {
      Equal(const T& value, Comparator cmp = Comparator()):
        value_(value),
        cmp_(cmp)
      {}

      bool operator()(const T& other) const
      {
        return !cmp_(value_, other) && !cmp_(other, value_);
      }

      const T& value_;
      Comparator cmp_;
    };

    template< class value_t>
    class BasicIterator
    {
      friend class ForwardList;
    public:
      using iterator_category = std::forward_iterator_tag;

      BasicIterator() noexcept:
        current_(nullptr),
        last_(nullptr)
      {}

      value_t& operator*() const noexcept
      {
        return current_->data_;
      }
      value_t* operator->() const noexcept
      {
        return std::addressof(current_->data_);
      }

      BasicIterator& operator++() noexcept
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
      BasicIterator& operator++(int) noexcept
      {
        BasicIterator temp(*this);
        ++(*this);
        return temp;
      }

      bool operator==(const BasicIterator& rhs) const noexcept
      {
        return current_ == rhs.current_ && last_ == rhs.last_;
      }
      bool operator!=(const BasicIterator& rhs) const noexcept
      {
       return !(*this == rhs);
      }
    private:
      BasicIterator(Node* node, Node* last) noexcept:
        current_(node),
        last_(last)
      {}

      Node* current_;
      Node* last_;
    };

  public:
    ForwardList() noexcept:
      head_(nullptr),
      tail_(nullptr),
      size_(0)
    {}
    ForwardList(const ForwardList& list):
      ForwardList()
    {
      for (auto i = list.cbegin(); i != list.cend(); ++i)
      {
        push_back(*i);
      }
    }
    ForwardList(ForwardList&& list) noexcept:
      head_(list.head_),
      tail_(list.tail_),
      size_(list.size_)
    {
      list.head_ = nullptr;
      list.tail_ = nullptr;
      list.size_ = 0;
    }
    ForwardList(size_t count, const T& value):
      ForwardList()
    {
      for (size_t i = 0; i < count; ++i)
      {
        push_back(value);
      }
    }
    ~ForwardList() noexcept
    {
      clear();
    }

    ForwardList& operator=(const ForwardList& list)
    {
      if (this != std::addressof(list))
      {
        ForwardList temp(list);
        swap(temp);
      }
      return *this;
    }
    ForwardList& operator=(ForwardList&& list) noexcept
    {
      if (this != std::addressof(list))
      {
        ForwardList temp(std::move(list));
        swap(temp);
      }
      return *this;
    }

    using Iterator = BasicIterator< T >;
    using ConstIterator = BasicIterator< const T >;

    Iterator begin() noexcept
    {
      return Iterator(head_, tail_);
    }
    Iterator end() noexcept
    {
      return Iterator(nullptr, tail_);
    }
    ConstIterator cbegin() const noexcept
    {
      return ConstIterator(head_, tail_);
    }
    ConstIterator cend() const noexcept
    {
      return ConstIterator(nullptr, tail_);
    }

    T& front()
    {
      if (!head_)
      {
        throw std::logic_error("list is empty!");
      }
      return head_->data_;
    }
    const T& front() const
    {
      if (!head_)
      {
        throw std::logic_error("list is empty!");
      }
      return head_->data_;
    }
    T& back()
    {
      if (!tail_)
      {
        throw std::logic_error("list is empty!");
      }
      return tail_->data_;
    }
    const T& back() const
    {
      if (!tail_)
      {
        throw std::logic_error("list is empty!");
      }
      return tail_->data_;
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

    ForwardList& swap(ForwardList& list) noexcept
    {
      std::swap(head_, list.head_);
      std::swap(tail_, list.tail_);
      std::swap(size_, list.size_);
      return *this;
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
      bool changed;

      do
      {
        changed = false;
        do
        {
          if (condition(current->data_))
          {
            Node* toDelete = current;
            if (current == head_)
            {
              head_ = head_->next_;
              tail_->next_ = head_;
              if (head_ == tail_ && condition(tail_->data_))
              {
                prev = tail_;
                current = head_;
                continue;
              }
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
            changed = true;
            if (size_ == 0)
            {
              head_ = tail_ = nullptr;
              return;
            }
          }
          else
          {
            prev = current;
            current = current->next_;
          }
        }
        while (current != head_);
      }
      while (changed);
    }

    void remove(const T& value) noexcept
    {
      remove_if(Equal(value));
    }
    void assign(size_t count, const T& value)
    {
      ForwardList temp;
      for (size_t i = 0; i < count; ++i)
      {
        temp.push_back(value);
      }
      swap(temp);
    }

  private:
    Node* head_;
    Node* tail_;
    size_t size_;
  };
}
#endif
