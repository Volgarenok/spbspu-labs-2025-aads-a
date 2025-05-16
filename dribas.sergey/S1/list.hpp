#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <list>
#include "node.hpp"
#include "iterator.hpp"


namespace dribas
{
  template< class T >
  class List final
  {
  public:
    using iter = Iterator< T, false >;
    using citer = Iterator< T, true >;
    List();
    List(const List< T >& rhs);
    List(List< T >&& rhs) noexcept;
    ~List();

    iter begin() noexcept;
    citer begin() const noexcept;
    iter end() noexcept;
    citer end() const noexcept;
    citer cbegin() const noexcept;
    citer cend() const noexcept;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    List< T >& operator=(const List< T >& other);
    List< T >& operator=(List< T >&& other) noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    template< class Predicate >
    void remove_if(Predicate) noexcept;
    void remove(const T&) noexcept;
    void splice(citer pos, List< T >& other) noexcept;
    void splice(citer pos, List< T >&& other) noexcept;
    void splice(citer pos, List< T >& other, citer it) noexcept;
    void splice(citer pos, List< T >&& other, citer it) noexcept;
    void splice(citer pos, List< T >& other, citer first, citer last) noexcept;
    void splice(citer pos, List< T >&& other, citer first, citer last) noexcept;
    template< class InputIt >
    void assign(InputIt first, InputIt last);
    template< class R >
    void assign_range(R&& rg );
    void assign(size_t count, const T& value);
    void push_front(const T& value);
    void push_back(const T& value);
    void pop_front() noexcept;
    void pop_back() noexcept;
    void swap(List< T >& rhs) noexcept;
    void clear() noexcept;

  private:
    Node < T >* head_;
    Node < T >* tail_;
    size_t size_;
  };
  template < class T >
  void List< T >::splice(citer pos, List< T >& other, citer first, citer last) noexcept
  {
    if ((std::addressof(other) == this || first == last)) {
      return;
    }
    Node< T >* first_node = first.node_;
    Node< T >* last_node = last.node_;
    Node< T >* pos_node = pos.node_;

    if (first_node->prev_) {
      first_node->prev_->next_ = last_node;
    } else {
      other.head_ = last_node;
    }
    if (last_node) {
      last_node->prev_ = first_node->prev_;
    } else {
      other.tail_ = first_node->prev_;
    }
    if (pos_node) {
      first_node->prev_ = pos_node->prev_;
      if (pos_node->prev_) {
        pos_node->prev_->next_ = first_node;
      } else {
        head_ = first_node;
      }
      pos_node->prev_ = last_node ? last_node->prev_ : other.tail_;
    } else {
      first_node->prev_ = tail_;
      if (tail_) {
        tail_->next_ = first_node;
      } else {
        head_ = first_node;
      }
      tail_ = last_node ? last_node->prev_ : other.tail_;
    }
    size_t count = 0;
    Node< T >* current = first_node;
    while (current != last_node) {
      ++count;
      current = current->next_;
    }
    size_ += count;
    other.size_ -= count;
  }

  template < class T >
  void dribas::List< T >::splice(citer pos, List< T >& other, citer it) noexcept
  {
    if (it != other.end()) {
      splice(pos, other, it, citer(it.node_->next_));
    }
  }

  template < class T >
  void dribas::List< T >::splice(citer pos, List< T >& other) noexcept
  {
    if (!other.empty()) {
      splice(pos, other, other.begin(), other.end());
    }
  }

  template < class T >
  void dribas::List< T >::splice(citer pos, List< T >&& other) noexcept
  {
    splice(pos, other);
  }

  template < class T >
  void dribas::List< T >::splice(citer pos, List< T >&& other, citer it) noexcept
  {
    splice(pos, other, it);
  }

  template < class T >
  void dribas::List< T >::splice(citer pos, List< T >&& other,
    citer first, citer last) noexcept
  {
    splice(pos, other, first, last);
  }

  template< class T >
  Iterator< T, false > dribas::List< T >::begin() noexcept
  {
    return iter(head_);
  }

  template< class T >
  Iterator< T, false > dribas::List< T >::end() noexcept
  {
    if (tail_) {
      return iter(tail_->next_);
    } else {
      return iter(nullptr);
    }
  }

  template< class T >
  Iterator< T, true > dribas::List< T >::begin() const noexcept
  {
    return citer(head_);
  }

  template< class T >
  Iterator< T, true > dribas::List< T >::end() const noexcept
  {
    if (tail_) {
      return citer(tail_->next_);
    } else {
      return citer(nullptr);
    }
  }

  template< class T >
  Iterator< T, true > dribas::List< T >::cbegin() const noexcept
  {
    return citer(head_);
  }

  template< class T >
  Iterator< T, true > List< T >::cend() const noexcept
  {
    if (tail_) {
      return citer(tail_->next_);
    } else {
      return citer(nullptr);
    }
  }

  template< class T >
  void dribas::List< T >::remove(const T& value) noexcept
  {
    remove_if([&value](const T& element) {
      return element == value;
    });
  }

  template< class T >
  template< class R >
  void dribas::List< T >::assign_range(R&& rg)
  {
    clear();
    for (auto&& item: rg) {
      push_back(std::forward< decltype(item) >(item));
    }
  }

  template< class T >
  void dribas::List< T >::assign(size_t count, const T& value)
  {
    clear();
    List< T > temp;
    for (size_t i = 0; i < count; ++i) {
      temp.push_back(value);
    }
    swap(temp);
  }

  template< class T >
  template< class InputIt >
  void dribas::List< T >::assign(InputIt first, InputIt last)
  {
    clear();
    List< T > temp;
    for (auto it = first; it != last; ++it) {
      temp.push_back(*it);
    }
    swap(temp);
  }

  template< class T >
  template< class Predicate >
  void dribas::List< T >::remove_if(Predicate predicat) noexcept
  {
    Node< T >* current = head_;
    Node< T >* prev = nullptr;
    while (current) {
      if (predicat(current->data_)) {
        Node< T >* toDelete = current;
        if (prev) {
          prev->next_ = current->next_;
        } else {
          head_ = current->next_;
        }
        if (current->next_) {
          current->next_->prev_ = prev;
        } else {
          tail_ = prev;
        }
        current = current->next_;
        delete toDelete;
        size_--;
      } else {
        prev = current;
        current = current->next_;
      }
    }
  }

  template< class T >
  void dribas::List< T >::push_back(const T& value)
  {
    Node< T >* newNode = new Node< T >{ value };
    if (!tail_) {
      head_ = newNode;
      tail_ = newNode;
    } else {
      tail_->next_ = newNode;
      newNode->prev_ = tail_;
      tail_ = newNode;
    }
    size_++;
  }

  template< class T >
  void dribas::List< T >::pop_back() noexcept
  {
    if (empty()) {
      return;
    }
    if (head_ == tail_) {
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
    } else {
      tail_ = tail_->prev_;
      delete tail_->next_;
      tail_->next_ = nullptr;
    }
    size_--;
  }

  template< class T >
  void dribas::List< T >::clear() noexcept
  {
    while (!empty()) {
      pop_back();
    }
  }

  template< class T >
  dribas::List< T >::~List()
  {
    clear();
  }

  template< class T >
  void dribas::List< T >::push_front(const T& value)
  {
    Node< T >* newNode = new Node< T >{ value };
    newNode->next_ = head_;
    if (head_) {
      head_->prev_ = newNode;
    }
    head_ = newNode;
    if (!tail_) {
      tail_ = head_;
    }
    size_++;
  }

  template< class T >
  T& dribas::List< T >::front()
  {
    return head_->data_;
  }

  template< class T >
  T& dribas::List< T >::back()
  {
    return tail_->data_;
  }

  template< class T >
  const T& dribas::List< T >::back() const
  {
    return tail_->data_;
  }

  template< class T >
  const T& dribas::List< T >::front() const
  {
    return head_->data_;
  }

  template< class T >
  dribas::List< T >::List():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< class T >
  dribas::List< T >::List(const List< T >& rhs):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    Node< T >* current = rhs.head_;
    try {
      while (current) {
        Node< T >* temp = new Node< T >{ current->data_ };
        if (head_ == nullptr) {
          head_ = temp;
          tail_ = temp;
        } else {
          tail_->next_ = temp;
          temp->prev_ = tail_;
          tail_ = temp;
        }
        size_++;
        current = current->next_;
      }
    } catch (const std::bad_alloc&) {
      clear();
      throw;
    }
}

  template< class T >
  void dribas::List< T >::swap(List< T >& rhs) noexcept
  {
    std::swap(head_, rhs.head_);
    std::swap(size_, rhs.size_);
    std::swap(tail_, rhs.tail_);
  }

  template< class T >
  bool dribas::List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  size_t dribas::List< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  dribas::List< T >& dribas::List< T >::operator=(const List< T >& other)
  {
    if (this != std::addressof(other)) {
      List< T > copy(other);
      swap(copy);
    }
    return *this;
  }

  template < class T >
  void dribas::List< T >::pop_front() noexcept
  {
    if (size_) {
      Node< T >* forDelete = head_;
      head_ = head_->next_;
      if (head_) {
        head_->prev_ = nullptr;
      } else {
        tail_ = nullptr;
      }
      delete forDelete;
      size_--;
    }
  }

  template< class T >
  dribas::List< T >& dribas::List< T >::operator=(List< T >&& other) noexcept
  {
    if (this != std::addressof(other)) {
      clear();
      swap(other);
    }
    return *this;
  }

  template< class T >
  dribas::List< T >::List(List< T >&& rhs) noexcept:
    head_(rhs.head_),
    tail_(rhs.tail_),
    size_(rhs.size_)
  {
    rhs.size_ = 0;
    rhs.tail_ = nullptr;
    rhs.head_ = nullptr;
  }
}
#endif
