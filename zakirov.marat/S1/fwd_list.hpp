#ifndef MY_LIST_HPP
#define MY_LIST_HPP
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
    void push_front(const T && data);
    void pop_front() noexcept;
    void insert_after(FwdIterator< T > element, const T & data);
    void erase_after(FwdIterator< T > element) noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void swap(FwdList & other);
    void splice_after(FwdIterator< T > pos, const FwdList & fwdlst);
    void splice_after(FwdIterator< T > pos, const FwdList & fwdlst, FwdIterator< T > i);
    void splice_after(FwdIterator< T > pos, const FwdList & fwdlst, FwdIterator< T > first, FwdIterator< T > last);
    void splice_after(FwdIterator< T > pos, FwdList && fwdlst);
    void splice_after(FwdIterator< T > pos, FwdList && fwdlst, FwdIterator< T > i);
    void splice_after(FwdIterator< T > pos, FwdList && fwdlst, FwdIterator< T > first, FwdIterator< T > last);
    void assign(size_t n, const T & val);
    template< typename InputIterator >
    void assign(InputIterator first, InputIterator last);
    void assign(std::initializer_list< T > init_list);
    void clear() noexcept;
    void remove(T data) noexcept;
    template< typename P>
    void remove_if(P predicate) noexcept;
  private:
    FwdListNode< T > * fake_node_;
  };

  template< typename T >
  FwdList< T >::FwdList():
    fake_node_(reinterpret_cast< FwdListNode< T >* >(new char[sizeof(FwdListNode< T >)]))
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
    FwdList()
  {
    FwdIterator< T > inserter = begin();
    for (auto i = init_list.begin(); i != init_list.end(); ++i, ++inserter)
    {
      insert_after(inserter, *i);
    }
  }

  template< typename T >
  FwdList< T >::FwdList(const FwdList & other):
    FwdList()
  {
    FwdIterator< T > inserter = before_begin();
    for (CFwdIterator< T > i = other.cbegin(); i != other.cend(); ++i, ++inserter)
    {
      insert_after(inserter, *i);
    }
  }

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
    clear();
    FwdIterator<T> inserter = begin();
    CFwdIterator<T> left = fwdlst.cbegin();
    CFwdIterator<T> right = fwdlst.cend();
    for (; left != right; ++left, ++inserter)
    {
      insert_after(inserter, *left);
    }
    return *this;
  }

  template< typename T >
  FwdList< T > & FwdList< T >::operator=(FwdList< T > && fwdlst) noexcept
  {
    if (this != &fwdlst)
    {
      FwdListNode<T> * temporary_node = fake_node_;
      fake_node_ = fwdlst.fake_node_;
      fwdlst.fake_node_ = temporary_node;
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
    return fake_node_->next_;
  }

  template< typename T >
  const T & FwdList< T >::front() const noexcept
  {
    return fake_node_->next_;
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
  }

  template< typename T >
  void FwdList< T >::push_front(const T & data)
  {
    FwdListNode< T > * new_element = new FwdListNode< T >(data);
    new_element->next_ = fake_node_->next_;
    fake_node_->next_ = new_element;
  }

  template< typename T >
  void FwdList< T >::push_front(const T && data)
  {
    FwdListNode< T > * new_element = new FwdListNode< T >(std::move(data));
    new_element->next_ = fake_node_->next_;
    fake_node_->next_ = new_element;
  }

  template< typename T >
  void FwdList< T >::erase_after(FwdIterator< T > element) noexcept
  {
    FwdListNode< T > * temp_element = element.node_->next_->next_;
    delete element.node_->next_;
    element.node_->next_ = temp_element;
  }

  template< typename T >
  void FwdList< T >::insert_after(FwdIterator< T > element, const T & data)
  {
    FwdListNode< T > * new_element = new FwdListNode< T >(data);
    new_element->next_ = element.node_->next_;
    element.node_->next_ = new_element;
  }

  template< typename T >
  bool FwdList< T >::empty() const noexcept
  {
    if (fake_node_->next_ == fake_node_)
    {
      return 1;
    }

    return 0;
  }

  template< typename T >
  size_t FwdList< T >::size() const noexcept
  {
    size_t counter = 0;
    FwdListNode< T > * nailed_ptr = fake_node_;
    FwdListNode< T > * fillable_ptr = fake_node_->next_;
    while (nailed_ptr != fillable_ptr)
    {
      ++counter;
      fillable_ptr = fillable_ptr->next_;
    }

    return counter;
  }

  template< typename T >
  void FwdList< T >::swap(FwdList & other)
  {
    FwdListNode< T > * temporary_node = other.fake_node_;
    other.fake_node_ = fake_node_;
    fake_node_ = temporary_node;
  }

  template< typename T >
  void FwdList< T >::splice_after(FwdIterator< T > pos, const FwdList & fwdlst)
  {
    splice_after(pos, fwdlst, fwdlst.begin(), fwdlst.end());
  }

  template< typename T >
  void FwdList< T >::splice_after(FwdIterator< T > pos, const FwdList & fwdlst, FwdIterator< T > i)
  {
    FwdList< T > * next_base = pos.node_->next_;
    FwdList< T > * next_new = i.node_->next_->next_;
    pos.node_->next_ = i.node_->next_;
    pos.node_->next_->next_ = next_base;
    i.node_->next_ = next_new;
  }

  template< typename T >
  void FwdList< T >::splice_after(FwdIterator< T > pos, const FwdList & fwdlst, FwdIterator< T > first, FwdIterator< T > last)
  {
    FwdIterator< T > after_first = first;
    ++after_first;
    for (; after_first != last; ++after_first, ++first)
    {
      splice_after(pos, fwdlst, first);
    }
  }

  template< typename T >
  void FwdList< T >::splice_after(FwdIterator< T > pos, FwdList && fwdlst)
  {
    FwdList & l_value_fwdlst = fwdlst;
    splice_after(pos, l_value_fwdlst);
  }

  template< typename T >
  void FwdList< T >::splice_after(FwdIterator< T > pos, FwdList && fwdlst, FwdIterator< T > i)
  {
    FwdList & l_value_fwdlst = fwdlst;
    splice_after(pos, l_value_fwdlst, i);
  }

  template< typename T >
  void FwdList< T >::splice_after(FwdIterator< T > pos, FwdList && fwdlst, FwdIterator< T > first, FwdIterator< T > last)
  {
    FwdList & l_value_fwdlst = fwdlst;
    splice_after(pos, l_value_fwdlst, first, last);
  }

  template< typename T >
  void FwdList< T >::assign(size_t n, const T & val)
  {
    clear();
    for (size_t i = 0; i < n; ++i)
    {
      push_front(val);
    }
  }

  template< typename T >
  template< typename InputIterator >
  void FwdList< T >::assign(InputIterator first, InputIterator last)
  {
    clear();
    FwdIterator< T > inserter = begin();
    for (; first != last; ++first, ++inserter)
    {
      insert_after(inserter, *first);
    }
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
  }

  template< typename T >
  void FwdList< T >::remove(T data) noexcept
  {
    FwdIterator< T > prev = end();
    FwdIterator< T > real = begin();
    for (FwdIterator< T > i = begin(); i != end(); ++i)
    {
      if (i.node_->data_ == data)
      {
        erase_after(prev);
        ++real;
      }
      else
      {
        ++prev;
        ++real;
      }
    }
  }

  template< typename T >
  template< typename P >
  void FwdList< T >::remove_if(P predicate) noexcept
  {
    FwdIterator< T > prev = end();
    FwdIterator< T > real = begin();
    for (FwdIterator< T > i = begin(); i != end(); ++i)
    {
      if (predicate(*real))
      {
        erase_after(prev);
        ++real;
      }
      else
      {
        ++prev;
        ++real;
      }
    }
  }
}

#endif
