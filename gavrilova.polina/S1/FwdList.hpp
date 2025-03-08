#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <algorithm>
#include "NodeFwdList.hpp"
#include "IteratorFwd.hpp"

namespace gavrilova {

  template< class T >
  class FwdList
  {
  public:
    FwdList();
    FwdList(const FwdList& other);
    FwdList(FwdList&& other) noexcept;
    ~FwdList();

    FwdList& operator=(const FwdList& other);
    FwdList& operator=(FwdList&& other) noexcept;

    bool operator==(const FwdList& other) const;
    bool operator!=(const FwdList& other) const;
    bool operator<(const FwdList& other) const;
    bool operator>(const FwdList& other) const;

    IteratorFwd< T > begin() const;
    IteratorFwd< T > end() const;
    T& front();
    // T& back();
    bool empty() const noexcept;
    size_t size() const noexcept;
    void push_front(const T& value);
    void pop_front();
    void clear();
    void remove(const T& value);
    void splice(const FwdList& other);
    void reverse();
    void swap(FwdList< T >& other) noexcept;
    void assign(size_t count, const T& value);
    void assign(IteratorFwd< T > first, IteratorFwd< T > last);
  private:
    NodeFwdList< T >* fake_;
    size_t nodeCount_;
  };

}

template< class T >
gavrilova::FwdList< T >::FwdList() :
  fake_(reinterpret_cast< NodeFwdList< T >* >(new char[sizeof(NodeFwdList< T >)])),
  nodeCount_(0)
{
  fake_->next = fake_;
}

template< class T >
gavrilova::FwdList< T >::FwdList(const FwdList &other) :
  FwdList()
{
  for (auto it = other.begin(); it != other.end(); ++it)
  {
    push_front(*it);
  }
  reverse();
}

template< class T >
gavrilova::FwdList< T >::FwdList(FwdList &&other) noexcept :
  fake_(other.fake_),
  nodeCount_(other.nodeCount_)
{
  other.fake_ = nullptr;
  other.nodeCount_ = 0;
}

template< class T >
gavrilova::FwdList< T >& gavrilova::FwdList< T >::operator=(const FwdList &other)
{
  if (this == &other)
  {
    return *this;
  }
  clear();
  for (auto it = other.begin(); it != other.end(); ++it)
  {
    push_front(*it);
  }
  reverse();
  return *this;
}

template< class T >
gavrilova::FwdList< T >& gavrilova::FwdList< T >::operator=(FwdList &&other) noexcept
{
  if (this == &other)
  {
    return *this;
  }
  clear();
  delete[] reinterpret_cast< char* >(fake_);;
  fake_ = other.fake_;
  nodeCount_ = other.nodeCount_;

  other.fake_ = nullptr;
  other.nodeCount_ = 0;

  return *this;
}

template< class T >
gavrilova::FwdList< T >::~FwdList()
{
  clear();
  delete[] reinterpret_cast< char* >(fake_);
}

template< class T >
gavrilova::IteratorFwd< T > gavrilova::FwdList< T >::begin() const
{
  return IteratorFwd< T >(fake_->next);
}

template< class T >
gavrilova::IteratorFwd< T > gavrilova::FwdList< T >::end() const
{
  return IteratorFwd< T >(fake_);
}

template< class T >
T & gavrilova::FwdList< T >::front()
{
  if (empty())
  {
    throw std::out_of_range("List is empty");
  }
  return fake_->next->data;
}

// template< class T >
// T & gavrilova::FwdList< T >::back()
// {
//   if (empty())
//   {
//     throw std::out_of_range("List is empty");
//   }

//   NodeFwdList<T> *current = fake->next;
//   while (current->next != fake->next)
//   {
//     current = current->next;
//   }
//   return current->data;
// }

template< class T >
bool gavrilova::FwdList< T >::empty() const noexcept
{
  return nodeCount_ == 0;
}

template< class T >
size_t gavrilova::FwdList< T >::size() const noexcept
{
  return nodeCount_;
}

template< class T >
void gavrilova::FwdList< T >::push_front(const T &value)
{
  NodeFwdList<T> *newNode = new NodeFwdList<T>{value, fake_->next};
  fake_->next = newNode;
  ++nodeCount_;
}

template< class T >
void gavrilova::FwdList< T >::pop_front()
{
  if (empty())
  {
    throw std::out_of_range("List is empty");
  }
  NodeFwdList<T> *temp = fake_->next;
  fake_->next = fake_->next->next;
  delete temp;
  --nodeCount_;
}

template< class T >
void gavrilova::FwdList< T >::clear()
{
  while (!empty())
  {
    pop_front();
  }
}

template< class T >
void gavrilova::FwdList< T >::remove(const T &value)
{
  NodeFwdList<T> *current = fake_;
  while (current->next != fake_)
  {
    if (current->next->data == value)
    {
      NodeFwdList<T> *temp = current->next;
      current->next = current->next->next;
      delete temp;
      --nodeCount_;
    }
    else
    {
      current = current->next;
    }
  }
}

template< class T >
void gavrilova::FwdList< T >::splice(const FwdList<T> &other)
{
  if (other.empty())
  {
    return;
  }

  NodeFwdList<T> *lastNode = fake_;
  while (lastNode->next != fake_)
  {
    lastNode = lastNode->next;
  }

  lastNode->next = other.fake_->next;
  nodeCount_ += other.nodeCount_;
}

template< class T >
void gavrilova::FwdList< T >::reverse()
{
  if (fake_->next == fake_)
  {
    return;
  }
  NodeFwdList< T >* next = fake_->next;
  NodeFwdList< T >* last = fake_;
  while (next->next != fake_)
  {
    NodeFwdList< T >* remember = next->next;
    next->next = last;
    last = next;
    next = remember;
  }
  next->next = last;
  fake_->next = next;
}

template< class T >
bool gavrilova::FwdList< T >::operator==(const FwdList<T> &other) const
{
  if (nodeCount_ != other.nodeCount_)
  {
    return false;
  }

  IteratorFwd< T > it1 = begin();
  IteratorFwd< T > it2 = other.begin();
  while (it1 != end())
  {
    if (*it1 != *it2)
      return false;
    ++it1;
    ++it2;
  }
  return true;
}

template< class T >
bool gavrilova::FwdList< T >::operator!=(const FwdList<T> &other) const
{
  return !(*this == other);
}

template< class T >
bool gavrilova::FwdList< T >::operator<(const FwdList<T> &other) const
{
  IteratorFwd< T > it1 = begin();
  IteratorFwd< T > it2 = other.begin();
  while (it1 != end() && it2 != other.end())
  {
    if (*it1 < *it2)
    {
      return true;
    }
    if (*it1 > *it2)
    {
      return false;
    }
    ++it1;
    ++it2;
  }
  return nodeCount_ < other.nodeCount_;
}

template< class T >
bool gavrilova::FwdList< T >::operator>(const FwdList<T> &other) const
{
  return other < *this;
}

template< class T>
void gavrilova::FwdList< T >::swap(FwdList< T >& other ) noexcept
{
  std::swap(fake_, other.fake_);
  std::swap(nodeCount_, other.nodeCount_);
}

template< class T>
void gavrilova::FwdList< T>::assign(size_t count, const T& value)
{
  clear();
  for (size_t i = 0; i < count; ++i)
  {
    push_front(value);
  }
}

template< class T>
void gavrilova::FwdList< T>::assign(IteratorFwd< T > first, IteratorFwd< T > last)
{
  clear();
  while(first != last)
  {
    push_front(*first);
    ++first;
  }
  reverse();
}

#endif
