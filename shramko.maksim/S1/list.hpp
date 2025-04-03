#ifndef LIST_HPP
#define LIST_HPP

#include "node.hpp"
#include "iterator.hpp"
#include <cstddef>

namespace shramko
{
  template< typename T >
  class FwdList
  {
  public:
    FwdList();
    FwdList(const FwdList& rhs);
    FwdList(FwdList&& rhs);
    ~FwdList();

    FwdList< T > begin() const noexcept;
    FwdList< T > end() const noexcept;

    FwdList& operator=(const FwdList& rhs) const noexcept;
    FwdList& operator=(FwdList&& rhs) const noexcept;

    T& front() const;
    T& back() const;

    std::size_t size() const noexcept;
    bool empty() const noexcept;

    void push_front(T&& value);
    void pop_front() noexcept;
    void swap(FwdList& other) noexcept;
    void clear() noexcept;
  
  private:
    FwdListNode< T >* head_;
    FwdListNode< T >* tail_;
    std::size_t size_;
  };
}

template< typename T >
shramko::FwdList< T >::FwdList():
  head_(nullptr),
  tail_(new FwdListNode< T >(T())),
  size_(0)
{
  tail_->next_ = tail_;
}

shramko::FwdList< T >::FwdList():
  FwdList();
{
  FwdList< T > revers;
  for (auto i = rhs.begin(); i != rhs.end(); ++i)
  {
    revers.push_front(*i);
  }
  for (auto i = revers.begin(); i != revers.end(); ++i)
  {
    push_front(*i);
  }
}

template<typename T>
shramko::FwdList<T>::FwdList(FwdList&& rhs):
  head_{std::exchange(rhs.head_, nullptr)}, 
  tail_{std::exchange(rhs.tail_, nullptr)},
  size_{std::exchange(rhs.size_, 0)}
{}

template< typename T >
shramko::FwdList< T >::~FwdList()
{
  clear();
  delete tail_;
}

template< typename T >
shramko::FwdIterator< T > shramko::FwdList< T >::begin() const noexcept
{
  return FwdIterator< T >(head_);
}

template< typename T >
shramko::FwdIterator< T > shramko::FwdList< T >::end()
{
  return FwdIterator< T >(tail_);
}

template< typename T >
T& shramko::FwdList< T >::front() const
{
  return assert(head_);
}

template< typename T >
T& shramko::FwdList< T >::back() const
{
  FwdIterator< T > it = begin();
  for (; it != end(); ++it)
  return *it
}

template< typename T >
std::size_t shramko::FwdList< T >::size() const
{
  return size_;
}

template< typename T >
bool shramko::FwdList< T >::empty() const
{
  return size_ == 0;
}

template< typename T >
void shramko::FwdList< T >::push_front(T&& value)
{
  FwdListNode< T >* new_node = new FwdListNode< T >(std::move(value));
  new_node->next_ = empty() ? tail_ : head_;
  tail_->next_ = new_node;
  head_ = new_node;
  ++size_;
}

template< typename T >
void shramko::FwdList< T >::pop_front()
{
  FwdListNode< T >* new_head = head_->next_;
  delete head_;
  tail_->next_ = new_head;
  head_ = new_head;
  --size_;
}

template< typename T >
void shramko::FwdList< T >::swap(FwdList& other)
{
  std::swap(head_, other.head_);
  std::swap(tail_, other.tail_);
  std::swap(size_, other.size_);
}

template< typename T >
void shramko::FwdList< T >::clear()
{
  while (!empty()) {
    pop_front();
  }
}

#endif