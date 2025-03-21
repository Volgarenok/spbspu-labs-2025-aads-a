#ifndef FORWARD_RING_LIST_HPP
#define FORWARD_RING_LIST_HPP

#include <cstddef>
#include <cassert>
#include <memory>

namespace petrov
{
  template< typename T >
  struct ForwardRingList
  {
  public:
    struct ForwardRingListIterator
    {
    public:
      friend ForwardRingList< T >;
      using this_t = ForwardRingListIterator;
      ForwardRingListIterator(): node_(nullptr) {};
      ForwardRingListIterator(const this_t & rhs) = default;
      ~ForwardRingListIterator() = default;
      this_t & operator=(const this_t & rhs) = default;
      this_t & operator++();
      this_t operator++(int);
      T & operator*();
      T * operator->();
      const T & operator*() const;
      const T * operator->() const;
      bool operator!=(const this_t &) const;
      bool operator==(const this_t &) const;
    private:
      ForwardRingList< T > * node_;
    };

    using this_t = ForwardRingList< T >;
    using iterator_this_t = ForwardRingListIterator;
    ForwardRingList() = default;
    ForwardRingList(this_t & rhs);
    ForwardRingList(this_t && rhs);
    ~ForwardRingList();
    this_t & operator=(this_t & rhs);
    this_t & operator=(this_t && rhs);
    iterator_this_t begin();
    iterator_this_t end();
    T front();
    T back();
    bool empty() noexcept;
    size_t size() noexcept;
    void push_front(const T & element);
    void push_front(T && element);
    void pop_front();
    void pop_back();
    void swap() noexcept;
    void clear();
    void reverse();

  private:
    T data_;
    ForwardRingListIterator first_;
    ForwardRingListIterator last_;
    ForwardRingList< T > * next_;
  };

  template< typename T >
  typename ForwardRingList< T >::ForwardRingListIterator::this_t & ForwardRingList< T >::iterator_this_t::operator++()
  {
    assert(node_ != nullptr);
    this->node_ = node_->next_;
    return *this;
  }

  template< typename T >
  typename ForwardRingList< T >::ForwardRingListIterator::this_t ForwardRingList< T >::iterator_this_t::operator++(int)
  {
    assert(node_ != nullptr);
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  T & ForwardRingList< T >::ForwardRingListIterator::operator*()
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  T * ForwardRingList< T >::ForwardRingListIterator::operator->()
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool ForwardRingList< T >::ForwardRingListIterator::operator==(const this_t & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool ForwardRingList< T >::ForwardRingListIterator::operator!=(const this_t & rhs) const
  {
    return !(rhs == *this);
  }

  template< typename T >
  ForwardRingList< T >::ForwardRingList(this_t & rhs)
  {
    if (rhs.empty())
    {
      return;
    }
    try
    {
      for (auto it = rhs.begin(); it != rhs.end(); ++it)
      {
        this->push_front(*it);
      }
      this->push_front(*rhs.end());
    }
    catch(const std::bad_alloc & e)
    {
      this->clear();
    }
    this->reverse();
  }

  template< typename T >
  ForwardRingList< T >::ForwardRingList(this_t && rhs)
  {
    if (rhs.empty())
    {
      return;
    }
    try
    {
      for (auto it = rhs.begin(); it != rhs.end(); ++it)
      {
        this->push_front(*it);
      }
      this->push_front(*rhs.end());
    }
    catch(const std::bad_alloc & e)
    {
      this->clear();
    }
    this->reverse();
  }

  template< typename T >
  ForwardRingList< T >::~ForwardRingList()
  {
    this->clear();
  }

  template< typename T >
  typename ForwardRingList< T >::this_t & ForwardRingList< T >::operator=(this_t & rhs)
  {
    this_t cpy(rhs);
    this->clear();
    std::swap(this->first_, cpy.first_);
    std::swap(this->last_, cpy.last_);
    return *this;
  }

  template< typename T >
  void ForwardRingList< T >::clear()
  {
    if (this->empty())
    {
      return;
    }
    for (auto it = this->begin(); it != this->end(); ++it)
    {
      delete it.node_;
    }
    delete this->end().node_;
    first_.node_ = nullptr;
    last_.node_ = nullptr;
  }

  template< typename T >
  typename ForwardRingList< T >::iterator_this_t ForwardRingList< T >::begin()
  {
    return first_;
  }

  template< typename T >
  typename ForwardRingList< T >::iterator_this_t ForwardRingList< T >::end()
  {
    return last_;
  }

  template< typename T >
  T ForwardRingList< T >::front()
  {
    return *(first_);
  }

  template< typename T >
  T ForwardRingList< T >::back()
  {
    return *(last_);
  }

  template< typename T >
  bool ForwardRingList< T >::empty() noexcept
  {
    return (!this->begin().node_);
  }

  template< typename T >
  size_t ForwardRingList< T >::size() noexcept
  {
    size_t count = 0;
    for (auto it = this->begin(); it != this->end(); ++it)
    {
      count++;
    }
    return ++count;
  }

  template< typename T >
  void ForwardRingList< T >::push_front(const T & element)
  {
    if (this->empty())
    {
      first_.node_ = new this_t;
      first_.node_->data_ = element;
      last_ = first_;
    }
    else
    {
      auto temp = first_.node_;
      last_.node_->next_ = new this_t;
      last_.node_->next_->data_ = element;
      first_.node_ = last_.node_->next_;
      first_.node_->next_ = temp;
    }
  }

  template< typename T >
  void ForwardRingList< T >::push_front(T && element)
  {
    if (this->empty())
    {
      first_.node_ = new this_t;
      first_.node_->data_ = element;
      first_.node_->next_ = first_.node_;
      last_ = first_;
    }
    else
    {
      auto temp = first_.node_;
      last_.node_->next_ = new this_t;
      last_.node_->next_->data_ = element;
      first_.node_ = last_.node_->next_;
      first_.node_->next_ = temp;
    }
  }

  template< typename T >
  void ForwardRingList< T >::pop_front()
  {
    auto todelete = first_;
    first_ = first_.node_->next_;
    delete todelete;
  }

  template< typename T >
  void ForwardRingList< T >::reverse()
  {
    if (this->empty())
    {
      return;
    }
    auto prev = this->end();
    auto next = ++this->begin();
    auto it = this->begin();
    while (it != this->end())
    {
      it.node_->next_ = prev.node_;
      prev = it;
      it = next++;
    }
    it.node_->next_ = prev.node_;
    prev = it;
    auto temp = this->begin();
    first_ = this->end();
    last_ = temp;
  }
}

#endif
