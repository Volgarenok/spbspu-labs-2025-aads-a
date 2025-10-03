#ifndef LIST_HPP
#define LIST_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>
#include "iterator.hpp"
#include "node.hpp"

namespace ivanova
{
  template < typename T >
  class List
  {
  public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using node_type = ListNode< T >;
    using iterator = ListIterator< List, false >;
    using const_iterator = ListIterator< List, true >;

    friend iterator;
    friend const_iterator;

    List() noexcept;
    List(const List& other);
    List(size_type n, const_reference value);
    List(List&& moved) noexcept;
    ~List();

    List& operator=(const List& other);
    List& operator=(List&& moved) noexcept;

    void assign(size_type n, const_reference value);

    template < typename IterType >
    void assign(IterType first, IterType last);

    void push_back(const_reference value);
    void push_front(const_reference value);
    void pop_back();
    void pop_front();
    void swap(List& other) noexcept;
    void clear();

    void splice(iterator position, List& other);
    void splice(iterator position, List& other, iterator i);
    void splice(iterator position, List& other, iterator first, iterator last);

    void remove(const_reference value);

    template < typename Predicate >
    void removeIf(Predicate pred);

    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    size_type size() const;
    bool empty() const;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

  private:
    node_type* head_;
    node_type* tail_;
    size_type size_;

    void savePush(const_reference value, bool back);
    void pop(bool back);

    template < typename... Args >
    node_type* createNode(Args&&... args);

    void deleteNode(node_type* node);
    void cutNodes(node_type* first, node_type* last);
    void linkNodes(node_type* first, node_type* second);
    size_type getDistance(iterator first, iterator last) const;

    struct EqualValue
    {
      const_reference value_;
      EqualValue(const_reference value):
        value_(value)
      {}
      bool operator()(const_reference x) const
      {
        return x == value_;
      }
    };
  };

  template < typename T >
  List< T >::List() noexcept:
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template < typename T >
  List< T >::List(const List& other):
    List()
  {
    assign(other.begin(), other.end());
  }

  template < typename T >
  List< T >::List(size_type n, const_reference value):
    List()
  {
    assign(n, value);
  }

  template < typename T >
  List< T >::List(List&& moved) noexcept:
    head_(moved.head_),
    tail_(moved.tail_),
    size_(moved.size_)
  {
    moved.head_ = nullptr;
    moved.tail_ = nullptr;
    moved.size_ = 0;
  }

  template < typename T >
  List< T >::~List()
  {
    clear();
  }

  template < typename T >
  List< T >& List< T >::operator=(const List& other)
  {
    if (this != &other)
    {
      assign(other.begin(), other.end());
    }
    return *this;
  }

  template < typename T >
  List< T >& List< T >::operator=(List&& moved) noexcept
  {
    swap(moved);
    moved.clear();
    return *this;
  }

  template < typename T >
  void List< T >::assign(size_type n, const_reference value)
  {
    if (n == 0)
    {
      clear();
      return;
    }
    try
    {
      for (size_type i = 0; i < n; ++i)
      {
        push_back(value);
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  template < typename T >
  template < typename IterType >
  void List< T >::assign(IterType first, IterType last)
  {
    bool same = std::is_same< IterType, iterator >::value || std::is_same< IterType, const_iterator >::value;
    if (!same || first.list_ != this)
    {
      clear();
      try
      {
        for (IterType x = first; x != last; ++x)
        {
          push_back(*x);
        }
      }
      catch (...)
      {
        clear();
        throw;
      }
    }
    else
    {
      if (first.node_ == nullptr)
      {
        clear();
        return;
      }
      while (!empty() && head_ != first.node_)
      {
        pop_front();
      }
      if (last.node_ == nullptr)
      {
        return;
      }
      while (!empty() && tail_ != last.node_)
      {
        pop_back();
      }
      pop_back();
    }
  }

  template < typename T >
  void List< T >::push_back(const_reference value)
  {
    savePush(value, true);
  }

  template < typename T >
  void List< T >::push_front(const_reference value)
  {
    savePush(value, false);
  }

  template < typename T >
  void List< T >::pop_back()
  {
    pop(true);
  }

  template < typename T >
  void List< T >::pop_front()
  {
    pop(false);
  }

  template < typename T >
  void List< T >::swap(List& other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }

  template < typename T >
  void List< T >::clear()
  {
    if (empty())
    {
      return;
    }
    node_type* node = head_;
    while (node != nullptr)
    {
      node_type* x = node;
      node = node->next;
      deleteNode(x);
    }
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
  }

  template < typename T >
  void List< T >::splice(iterator position, List& other)
  {
    if (empty())
    {
      swap(other);
      return;
    }
    splice(position, other, other.begin(), other.end());
  }

  template < typename T >
  void List< T >::splice(iterator position, List& other, iterator i)
  {
    iterator n = i;
    ++n;
    splice(position, other, i, n);
  }

  template < typename T >
  void List< T >::splice(iterator position, List& other, iterator first, iterator last)
  {
    if (other.empty())
    {
      return;
    }
    size_type diff = getDistance(first, last);
    if (diff == 0)
    {
      return;
    }
    iterator preLast = last;
    --preLast;
    other.cutNodes(first.node_, preLast.node_);
    if (empty())
    {
      head_ = first.node_;
      tail_ = preLast.node_;
    }
    else if (position == begin())
    {
      linkNodes(preLast.node_, head_);
      head_ = first.node_;
    }
    else if (position == end())
    {
      linkNodes(tail_, first.node_);
      tail_ = preLast.node_;
    }
    else
    {
      node_type* curr = position.node_;
      node_type* prev = curr->prev;
      linkNodes(prev, first.node_);
      linkNodes(preLast.node_, curr);
    }
    other.size_ -= diff;
    size_ += diff;
  }

  template < typename T >
  void List< T >::remove(const_reference value)
  {
    removeIf(EqualValue(value));
  }

  template < typename T >
  template < typename Predicate >
  void List< T >::removeIf(Predicate pred)
  {
    while (!empty() && pred(front()))
    {
      pop_front();
    }
    while (!empty() && pred(back()))
    {
      pop_back();
    }
    for (node_type* x = head_; x != tail_; )
    {
      if (pred(x->next->value))
      {
        node_type* y = x->next;
        linkNodes(x, y->next);
        deleteNode(y);
        --size_;
      }
      else
      {
        x = x->next;
      }
    }
  }

  template < typename T >
  typename List< T >::reference List< T >::front()
  {
    return *begin();
  }

  template < typename T >
  typename List< T >::const_reference List< T >::front() const
  {
    return *begin();
  }

  template < typename T >
  typename List< T >::reference List< T >::back()
  {
    return *(--end());
  }

  template < typename T >
  typename List< T >::const_reference List< T >::back() const
  {
    return *(--end());
  }

  template < typename T >
  typename List< T >::size_type List< T >::size() const
  {
    return size_;
  }

  template < typename T >
  bool List< T >::empty() const
  {
    return size_ == 0;
  }

  template < typename T >
  typename List< T >::iterator List< T >::begin()
  {
    return iterator(this, head_);
  }

  template < typename T >
  typename List< T >::const_iterator List< T >::begin() const
  {
    return const_iterator(this, head_);
  }

  template < typename T >
  typename List< T >::const_iterator List< T >::cbegin() const
  {
    return const_iterator(this, head_);
  }

  template < typename T >
  typename List< T >::iterator List< T >::end()
  {
    return iterator(this, nullptr);
  }

  template < typename T >
  typename List< T >::const_iterator List< T >::end() const
  {
    return const_iterator(this, nullptr);
  }

  template < typename T >
  typename List< T >::const_iterator List< T >::cend() const
  {
    return const_iterator(this, nullptr);
  }

  template < typename T >
  void List< T >::savePush(const_reference value, bool back)
  {
    node_type* node = createNode(value);

    if (empty())
    {
      head_ = tail_ = node;
    }
    else if (back)
    {
      linkNodes(tail_, node);
      tail_ = node;
    }
    else
    {
      linkNodes(node, head_);
      head_ = node;
    }
    ++size_;
  }

  template < typename T >
  void List< T >::pop(bool back)
  {
    if (empty())
    {
      return;
    }
    if (size_ == 1)
    {
      deleteNode(head_);
      head_ = nullptr;
      tail_ = nullptr;
    }
    else if (back)
    {
      tail_ = tail_->prev;
      deleteNode(tail_->next);
      tail_->next = nullptr;
    }
    else
    {
      head_ = head_->next;
      deleteNode(head_->prev);
      head_->prev = nullptr;
    }
    --size_;
  }

  template < typename T >
  template < typename... Args >
  typename List< T >::node_type* List< T >::createNode(Args&&... args)
  {
    node_type* node = static_cast< node_type* >(operator new(sizeof(node_type)));
    try
    {
      new (node) node_type(std::forward< Args >(args)...);
    }
    catch (...)
    {
      operator delete(node);
      throw;
    }
    return node;
  }

  template < typename T >
  void List< T >::deleteNode(node_type* node)
  {
    if (node != nullptr)
    {
      node->value.~value_type();
      operator delete(node);
    }
  }

  template < typename T >
  void List< T >::cutNodes(node_type* first, node_type* last)
  {
    node_type* preFirst = first->prev;
    node_type* postLast = last->next;
    if (preFirst != nullptr)
    {
      preFirst->next = postLast;
    }
    else
    {
      head_ = postLast;
    }
    if (postLast != nullptr)
    {
      postLast->prev = preFirst;
    }
    else
    {
      tail_ = preFirst;
    }
    first->prev = nullptr;
    last->next = nullptr;
  }

  template < typename T >
  void List< T >::linkNodes(node_type* first, node_type* second)
  {
    first->next = second;
    second->prev = first;
  }

  template < typename T >
  typename List< T >::size_type List< T >::getDistance(iterator first, iterator last) const
  {
    if (first == begin() && last == end())
    {
      return size_;
    }
    size_type count = 0;
    for (iterator it = first; it != last; ++it)
    {
      ++count;
    }
    return count;
  }
}

#endif
