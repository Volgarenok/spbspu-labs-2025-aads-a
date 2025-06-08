#ifndef FWD_LIST_HPP
#define FWD_LIST_HPP
#include <cstddef>
#include <utility>
#include "fwd_list_node.hpp"
#include "fwd_iterator.hpp"

namespace zakirov
{
  template< typename T >
  class FwdList
  {
  public:
    FwdList();
    FwdList(size_t node_quantity, const T & val);
    template< typename InputIterator >
    FwdList(InputIterator first, InputIterator second);
    FwdList(std::initializer_list< T > init_list);
    FwdList(const FwdList & other);
    FwdList(FwdList && other) noexcept;
    ~FwdList();
    FwdList< T > & operator=(const FwdList< T > & fwdlst);
    FwdList< T > & operator=(FwdList< T > && fwdlst) noexcept;
    FwdList< T > & operator=(std::initializer_list< T > init_list);
    T & front() noexcept;
    const T & front() const noexcept;
    FwdIterator< T > begin() noexcept;
    CFwdIterator< T > cbegin() const noexcept;
    FwdIterator< T > before_begin() noexcept;
    CFwdIterator< T > cbefore_begin() const noexcept;
    FwdIterator< T > end() noexcept;
    CFwdIterator< T > cend() const noexcept;
    void push_front(const T & data);
    void push_front(T && data);
    void pop_front() noexcept;
    void insert_after(FwdIterator< T > element, const T & data);
    void erase_after(FwdIterator< T > element) noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void swap(FwdList & other) noexcept;
    void splice_after(FwdIterator< T > pos, FwdList & fwdlst) noexcept;
    void splice_after(FwdIterator< T > pos, FwdList & fwdlst, FwdIterator< T > i) noexcept;
    void splice_after(FwdIterator< T > pos, FwdList & fwdlst, FwdIterator< T > first, FwdIterator< T > last) noexcept;
    void splice_after(FwdIterator< T > pos, FwdList && fwdlst) noexcept;
    void splice_after(FwdIterator< T > pos, FwdList && fwdlst, FwdIterator< T > i) noexcept;
    void splice_after(FwdIterator< T > pos, FwdList && fwdlst, FwdIterator< T > first, FwdIterator< T > last) noexcept;
    void assign(size_t n, const T & val);
    template< typename InputIterator >
    void assign(InputIterator first, InputIterator last);
    void assign(std::initializer_list< T > init_list);
    void clear() noexcept;
    template< typename P >
    void remove_if(P predicate) noexcept;
    void remove(const T & data) noexcept;
  private:
    FwdListNode< T > * fake_node_;
    size_t size_;
  };

  template< typename T >
  FwdList< T >::FwdList():
    fake_node_(reinterpret_cast< FwdListNode< T >* >(new char[sizeof(FwdListNode< T >)])),
    size_(0)
  {
    fake_node_->next_ = fake_node_;
  }

  template< typename T >
  FwdList< T >::FwdList(size_t node_quantity, const T & data):
    FwdList()
  {
    for (size_t i = 0; i < node_quantity; ++i)
    {
      push_front(data);
    }
  }

  template< typename T >
  template< typename InputIterator >
  FwdList< T >::FwdList(InputIterator first, InputIterator second):
    FwdList()
  {
    FwdIterator< T > inserter = before_begin();
    for (; first != second; ++first, ++inserter)
    {
      insert_after(inserter, *first);
    }
  }

  template< typename T >
  FwdList< T >::FwdList(std::initializer_list< T > init_list):
    FwdList(init_list.begin(), init_list.end())
  {}

  template< typename T >
  FwdList< T >::FwdList(const FwdList & other):
    FwdList(other.cbegin(), other.cend())
  {}

  template< typename T >
  FwdList< T >::FwdList(FwdList && other) noexcept:
    FwdList()
  {
    FwdListNode< T > * primal_state = fake_node_;
    fake_node_ = other.fake_node_;
    other.fake_node_ = primal_state;
  }

  template< typename T >
  FwdList< T >::~FwdList()
  {
    clear();
    delete[] reinterpret_cast< char* >(fake_node_);
  }

  template< typename T >
  FwdList< T > & FwdList< T >::operator=(const FwdList< T > & fwdlst)
  {
    if (this != std::addressof(fwdlst))
    {
      assign(fwdlst.cbegin(), fwdlst.cend());
    }

    return *this;
  }

  template< typename T >
  FwdList< T > & FwdList< T >::operator=(FwdList< T > && fwdlst) noexcept
  {
    if (this != std::addressof(fwdlst))
    {
      clear();
      swap(fwdlst);
    }

    return *this;
  }

  template< typename T >
  FwdList< T > & FwdList< T >::operator=(std::initializer_list< T > init_list)
  {
    assign(init_list);
    return *this;
  }

  template< typename T >
  T & FwdList< T >::front() noexcept
  {
    return fake_node_->next_->data_;
  }

  template< typename T >
  const T & FwdList< T >::front() const noexcept
  {
    return fake_node_->next_->data_;
  }

  template< typename T >
  FwdIterator< T > FwdList< T >::begin() noexcept
  {
    return FwdIterator< T >(fake_node_->next_);
  }

  template< typename T >
  CFwdIterator< T > FwdList< T >::cbegin() const noexcept
  {
    return CFwdIterator< T >(fake_node_->next_);
  }

  template< typename T >
  CFwdIterator< T > FwdList< T >::cbefore_begin() const noexcept
  {
    return CFwdIterator< T >(fake_node_);
  }

  template< typename T >
  FwdIterator< T > FwdList< T >::before_begin() noexcept
  {
    return FwdIterator< T >(fake_node_);
  }

  template< typename T >
  FwdIterator< T > FwdList< T >::end() noexcept
  {
    return FwdIterator< T >(fake_node_);
  }

  template< typename T >
  CFwdIterator< T > FwdList< T >::cend() const noexcept
  {
    return CFwdIterator< T >(fake_node_);
  }

  template< typename T >
  void FwdList< T >::pop_front() noexcept
  {
    FwdListNode< T > * new_begin = (fake_node_->next_)->next_;
    delete fake_node_->next_;
    fake_node_->next_ = new_begin;
    --size_;
  }

  template< typename T >
  void FwdList< T >::push_front(const T & data)
  {
    FwdListNode< T > * new_element = new FwdListNode< T >(data);
    new_element->next_ = fake_node_->next_;
    fake_node_->next_ = new_element;
    ++size_;
  }

  template< typename T >
  void FwdList< T >::push_front(T && data)
  {
    FwdListNode< T > * new_element = new FwdListNode< T >(std::move(data));
    new_element->next_ = fake_node_->next_;
    fake_node_->next_ = new_element;
    ++size_;
  }

  template< typename T >
  void FwdList< T >::insert_after(FwdIterator< T > element, const T & data)
  {
    FwdListNode< T > * new_element = new FwdListNode< T >(data);
    new_element->next_ = element.node_->next_;
    element.node_->next_ = new_element;
    ++size_;
  }

  template< typename T >
  void FwdList< T >::erase_after(FwdIterator< T > element) noexcept
  {
    FwdListNode< T > * temp_element = element.node_->next_->next_;
    delete element.node_->next_;
    element.node_->next_ = temp_element;
    --size_;
  }

  template< typename T >
  bool FwdList< T >::empty() const noexcept
  {
    return fake_node_->next_ == fake_node_;
  }

  template< typename T >
  size_t FwdList< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  void FwdList< T >::swap(FwdList & other) noexcept
  {
    std::swap(fake_node_, other.fake_node_);
    std::swap(size_, other.size_);
  }

  template< typename T >
  void FwdList< T >::splice_after(FwdIterator< T > pos, FwdList & fwdlst) noexcept
  {
    splice_after(pos, fwdlst, fwdlst.before_begin(), fwdlst.end());
  }

  template< typename T >
  void FwdList< T >::splice_after(FwdIterator< T > pos, FwdList & fwdlst, FwdIterator< T > i) noexcept
  {
    FwdListNode< T > * next_base = pos.node_->next_;
    FwdListNode< T > * next_new = i.node_->next_->next_;
    if (i.node_->next_ == fwdlst.fake_node_)
    {
      return;
    }

    pos.node_->next_ = i.node_->next_;
    pos.node_->next_->next_ = next_base;
    i.node_->next_ = next_new;
    --fwdlst.size_;
    ++size_;
  }

  template< typename T >
  void FwdList< T >::splice_after(FwdIterator< T > pos, FwdList & fwdlst, FwdIterator< T > first, FwdIterator< T > last) noexcept
  {
    FwdIterator< T > after_iter = first;
    ++after_iter;
    while (after_iter != last)
    {
      splice_after(pos, fwdlst, first);
      after_iter = first;
      ++after_iter;
      ++pos;
    }
  }

  template< typename T >
  void FwdList< T >::splice_after(FwdIterator< T > pos, FwdList && fwdlst) noexcept
  {
    splice_after(pos, fwdlst);
  }

  template< typename T >
  void FwdList< T >::splice_after(FwdIterator< T > pos, FwdList && fwdlst, FwdIterator< T > i) noexcept
  {
    splice_after(pos, fwdlst, i);
  }

  template< typename T >
  void FwdList< T >::splice_after(FwdIterator< T > pos, FwdList && fwdlst, FwdIterator< T > first, FwdIterator< T > last) noexcept
  {
    splice_after(pos, fwdlst, first, last);
  }

  template< typename T >
  void FwdList< T >::assign(size_t n, const T & val)
  {
    FwdList< T > inserter(n, val);
    swap(inserter);
  }

  template< typename T >
  template< typename InputIterator >
  void FwdList< T >::assign(InputIterator first, InputIterator last)
  {
    FwdList< T > inserter(first, last);
    swap(inserter);
  }

  template< typename T >
  void FwdList< T >::assign(std::initializer_list< T > init_list)
  {
    assign(init_list.begin(), init_list.end());
  }

  template< typename T >
  void FwdList< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_front();
    }

    size_ = 0;
  }

  template< typename T >
  template< typename P >
  void FwdList< T >::remove_if(P predicate) noexcept
  {
    FwdIterator< T > prev = before_begin();
    FwdIterator< T > real = begin();
    while (real != end())
    {
      if (predicate(*real))
      {
        ++real;
        erase_after(prev);
      }
      else
      {
        ++prev;
        ++real;
      }
    }
  }

  template< typename T >
  void FwdList< T >::remove(const T & data) noexcept
  {
    remove_if([&](const T & value)
    {
      return data == value;
    });
  }
}

#endif
