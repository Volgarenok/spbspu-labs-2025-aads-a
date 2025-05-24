#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include "node.hpp"
#include "iterator.hpp"

namespace dribas
{
  template< class T >
  class List final
  {
  public:
    using Iterator = dribas::Iterator< T, false >;
    using constItartor = dribas::Iterator< T, true >;

    List();
    List(const List< T >& rhs);
    List(List< T >&& rhs) noexcept;
    ~List();

    Iterator begin() noexcept;
    constItartor begin() const noexcept;
    Iterator end() noexcept;
    constItartor end() const noexcept;
    constItartor cbegin() const noexcept;
    constItartor cend() const noexcept;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    List< T >& operator=(const List< T >& other);
    List< T >& operator=(List< T >&& other) noexcept;
    Node< T >* at(size_t id);
    Node< T >* operator[](size_t id);
    const Node< T >* at(size_t id) const;
    const Node< T >* operator[](size_t id) const;

    bool empty() const noexcept;
    size_t size() const noexcept;

    template< class Predicate >
    void remove_if(Predicate) noexcept;
    void remove(const T&) noexcept;
    void splice(constItartor pos, List< T >& other) noexcept;
    void splice(constItartor pos, List< T >&& other) noexcept;
    void splice(constItartor pos, List< T >& other, constItartor it) noexcept;
    void splice(constItartor pos, List< T >&& other, constItartor it) noexcept;
    void splice(constItartor pos, List< T >& other, constItartor first, constItartor last) noexcept;
    void splice(constItartor pos, List< T >&& other, constItartor first, constItartor last) noexcept;
    template< class InputIt >
    void assign(InputIt first, InputIt last);
    template< class R >
    void assign_range( R&& rg );
    void assign(size_t count, const T& value);
    void push_front(const T& value);
    void push_front(T&& value);
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_front() noexcept;
    void pop_back() noexcept;
    void swap(List< T >& rhs) noexcept;
    void clear() noexcept;

  private:
    Node < T >* head_;
    Node < T >* tail_;
    size_t size_;
  };
}

template < class T >
void dribas::List< T >::splice(dribas::Iterator<T, true> pos, List< T >& other,
  dribas::Iterator<T, true> first, dribas::Iterator<T, true> last) noexcept
{
  if (!(&other == this || first == last)) {
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
}

template < class T >
void dribas::List< T >::splice(dribas::Iterator<T, true> pos, List< T >& other, dribas::Iterator<T, true> it) noexcept
{
  if (it != other.end()) {
    splice(pos, other, it, dribas::Iterator<T, true>(it.node_->next_));
  }
}

template < class T >
void dribas::List< T >::splice(dribas::Iterator<T, true> pos, List< T >& other) noexcept
{
  if (!other.empty()) {
    splice(pos, other, other.begin(), other.end());
  }
}

template < class T >
void dribas::List< T >::splice(dribas::Iterator<T, true> pos, List< T >&& other) noexcept
{
  splice(pos, other);
}

template < class T >
void dribas::List< T >::splice(dribas::Iterator<T, true> pos, List< T >&& other, dribas::Iterator<T, true> it) noexcept
{
  splice(pos, other, it);
}

template < class T >
void dribas::List< T >::splice(dribas::Iterator<T, true> pos, List< T >&& other,
  dribas::Iterator<T, true> first, dribas::Iterator<T, true> last) noexcept
{
  splice(pos, other, first, last);
}

template< class T >
dribas::Iterator< T, false > dribas::List< T >::begin() noexcept
{
  return dribas::Iterator< T, false >(head_);
}

template< class T >
dribas::Iterator< T, false > dribas::List< T >::end() noexcept
{
  if (tail_) {
    return dribas::Iterator< T, false >(tail_->next_);
  } else {
    return dribas::Iterator< T, false >(nullptr);
  }
}

template< class T >
dribas::Iterator< T, true > dribas::List< T >::begin() const noexcept
{
  return dribas::Iterator< T, true >(head_);
}

template< class T >
dribas::Iterator< T, true > dribas::List< T >::end() const noexcept
{
  if (tail_) {
    return dribas::Iterator< T, true >(tail_->next_);
  } else {
    return dribas::Iterator< T, true >(nullptr);
  }
}

template< class T >
dribas::Iterator< T, true > dribas::List< T >::cbegin() const noexcept
{
  return dribas::Iterator< T, true >(head_);
}

template< class T >
dribas::Iterator< T, true > dribas::List< T >::cend() const noexcept
{
  if (tail_) {
    return dribas::Iterator< T, true >(tail_->next_);
  } else {
    return dribas::Iterator< T, true >(nullptr);
  }
}

template< class T >
void dribas::List< T >::remove(const T& value) noexcept
{
  Node< T >* current = head_;
  Node< T >* prev = nullptr;
  while (current) {
    if (current->data_ == value) {
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
template< class R >
void dribas::List< T >::assign_range(R&& rg)
{
  clear();
  for (auto&& item : rg) {
    push_back(std::forward<decltype(item)>(item));
  }
}

template< class T >
void dribas::List< T >::assign(size_t count, const T& value)
{
  clear();
  for (size_t i = 0; i < count; ++i) {
    push_back(value);
  }
}

template< class T >
template< class InputIt >
void dribas::List< T >::assign(InputIt first, InputIt last)
{
  clear();
  while (first != last) {
    push_back(*first);
    ++first;
  }
}

template<class T>
template<class Predicate>
void dribas::List< T >::remove_if(Predicate predicator) noexcept
{
  Node< T >* current = head_;
  Node< T >* prev = nullptr;
  while (current) {
    if (predicator(current->data_)) {
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
void dribas::List< T >::push_back(T&& value)
{
  Node< T >* newNode = new Node< T >(value);
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
void dribas::List< T >::push_back(const T& value)
{
  Node< T >* newNode = new Node< T >(value);
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
  if (!empty()) {
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
}

template< class T >
void dribas::List< T >::clear() noexcept
{
  while (size_ != 0) {
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
  Node< T >* newNode = new Node< T >(value);
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
void dribas::List< T >::push_front(T&& value)
{
  Node< T >* newNode = new Node< T >(std::move(value));
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
const dribas::Node< T >* dribas::List< T >::operator[](size_t id) const
{
  float diffence = id / size_;
  Node< T >* returned = nullptr;
  if (diffence < 0.5) {
    returned = head_;
    for (size_t i = 0; i < id; i++ ) {
    returned = returned->next_;
    }
  } else {
    returned = tail_;
    for (size_t i = size_; i > id; i--) {
      returned = returned->prev_;
    }
  }
  return returned;
}

template< class T >
dribas::Node< T >* dribas::List< T >::operator[](size_t id)
{
  return const_cast< Node< T >* >(static_cast< const List< T >* >(this)->operator[](id));
}

template< class T >
const dribas::Node< T >* dribas::List< T >::at(size_t id) const
{
  if (id >= size_) {
    throw std::out_of_range("ID IS OUT OF RANGE");
  }
  return (*this)[id];
}

template< class T >
dribas::Node< T >* dribas::List< T >::at(size_t id)
{
  return const_cast< Node< T >* >(static_cast< const List< T >* >(this)->at(id));
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
  for (; current; current = current->next_) {
    push_back(current->data_);
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
    head_ = other.head_;
    tail_ = other.tail_;
    size_ = other.size_;
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }
  return *this;
}

template< class T >
dribas::List< T >::List(List< T >&& rhs) noexcept
{
  size_ = rhs.size_;
  head_ = rhs.head_;
  tail_ = rhs.tail_;
  rhs.size_ = 0;
  rhs.tail_ = nullptr;
  rhs.head_ = nullptr;
}

#endif
