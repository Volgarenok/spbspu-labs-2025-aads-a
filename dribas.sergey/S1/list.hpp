#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include "node.hpp"

namespace dribas
{
  template< class T >
  class List
  {
  public:
    //using iterator = Iterator< T >;
    //using constItertor = ConstItertor< T >;

    List();
    List(const List< T >& rhs);
    List(List< T >&& rhs) noexcept;
    ~List();

    //iterator begin() noexcept;
    //constItertor begin() const noexcept;
    //iterator end() noexcept;
    //constItertor end() const noexcept;

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

    void push_front(const T& value);
    void push_front(T&& value);
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_front();
    void pop_back();
    void swap(List< T >& rhs);
    void clear() noexcept;

  private:
    Node < T >* head_;
    Node < T >* tail_;
    size_t size_;
  };
}

template< class T >
void dribas::List< T >::push_back(T&& value) {
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
void dribas::List< T >::push_back(const T& value) {
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

template < class T >
void dribas::List<T>::pop_back()
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

template < class T >
void dribas::List < T >::clear() noexcept
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
  Node< T >* returned = head_;
  for (size_t i = 0; i < id; i++ ) {
    returned = returned->next_;
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
void dribas::List< T >::swap(List< T >& rhs)
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

template< class T>
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
void dribas::List< T >::pop_front() {
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
