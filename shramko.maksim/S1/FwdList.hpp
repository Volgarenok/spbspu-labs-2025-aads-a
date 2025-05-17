#ifndef FORWARD_LIST_HPP
#define FORWARD_LIST_HPP

#include <cassert>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include "FwdListNode.hpp"
#include "constIterator.hpp"
#include "iterator.hpp"

namespace shramko
{
  template< typename T >
  class ForwardList
  {
  public:
    friend class ConstIterator;
    friend class Iterator;
    using ConstIterator = ForwardListConstIterator< T >;
    using Iterator = ForwardListIterator< T >;

    ForwardList();
    ~ForwardList();
    ForwardList(const ForwardList& other);
    ForwardList(ForwardList&& other) noexcept;
    ForwardList& operator=(ForwardList other);
    ForwardList& operator=(ForwardList&& other);

    Iterator begin() const noexcept;
    Iterator end() const noexcept;
    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;

    T& getFront() const;
    T& getBack() const;
    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;

    void addToFront(const T& value);
    void addToFront(T&& value);
    void removeFront();
    void swapLists(ForwardList& other);
    void clearAll() noexcept;
    void addToBack(const T& value);
    void addToBack(T&& value);

  private:
    ForwardListNode< T >* headNode_;
    ForwardListNode< T >* tailNode_;
    size_t currentSize_;

    void insertFrontNode(ListNode< T >* newNode);
    void insertBackNode(ListNode< T >* newNode);
  };
}

template< typename T >
shramko::ForwardList< T >::ForwardList():
  headNode_(nullptr),
  tailNode_(nullptr),
  currentSize_(0)
{}

template< typename T >
shramko::ForwardList< T >::ForwardList(const ForwardList& other):
  headNode_(nullptr),
  tailNode_(nullptr),
  currentSize_(0)
{
  ForwardList temp;
  for (auto it = other.begin(); it != other.end(); ++it)
  {
    temp.addToFront(*it);
  }
  for (auto it = temp.begin(); it != temp.end(); ++it)
  {
    addToFront(*it);
  }
}

template< typename T >
shramko::ForwardList< T >::ForwardList(ForwardList&& other) noexcept:
  headNode_(other.headNode_),
  tailNode_(other.tailNode_),
  currentSize_(other.currentSize_)
{
  other.headNode_ = other.tailNode_ = nullptr;
  other.currentSize_ = 0;
}

template< typename T >
shramko::ForwardList< T >& shramko::ForwardList< T >::operator=(ForwardList other)
{
  swapLists(other);
  return *this;
}

template< typename T >
shramko::ForwardList< T >& shramko::ForwardList< T >::operator=(ForwardList&& other)
{
  if (this != &other)
  {
    clearAll();
    headNode_ = other.headNode_;
    tailNode_ = other.tailNode_;
    currentSize_ = other.currentSize_;
    other.tailNode_ = other.headNode_ = nullptr;
    other.currentSize_ = 0;
  }
  return *this;
}

template< typename T >
shramko::ForwardList< T >::~ForwardList()
{
  clearAll();
}

template< typename T >
typename shramko::ForwardList< T >::Iterator shramko::ForwardList< T >::begin() const noexcept
{
  return Iterator(headNode_);
}

template< typename T >
typename shramko::ForwardList< T >::Iterator shramko::ForwardList< T >::end() const noexcept
{
  if (currentSize_ == 0) return Iterator(nullptr);
  Iterator it(tailNode_->next_);
  it.isFirstPass_ = false;
  return it;
}

template< typename T >
typename shramko::ForwardList< T >::ConstIterator shramko::ForwardList< T >::cbegin() const noexcept
{
  return ConstIterator(headNode_);
}

template< typename T >
typename shramko::ForwardList< T >::ConstIterator shramko::ForwardList< T >::cend() const noexcept
{
  if (currentSize_ == 0) return ConstIterator(nullptr);
  ConstIterator it(tailNode_->next_);
  it.isFirstPass_ = false;
  return it;
}

template< typename T >
T& shramko::ForwardList< T >::getFront() const
{
  if (isEmpty()) throw std::logic_error("List is empty!");
  return headNode_->data_;
}

template< typename T >
T& shramko::ForwardList< T >::getBack() const
{
  if (isEmpty()) throw std::logic_error("List is empty!");
  return tailNode_->data_;
}

template< typename T >
bool shramko::ForwardList< T >::isEmpty() const noexcept
{
  return currentSize_ == 0;
}

template< typename T >
size_t shramko::ForwardList< T >::getSize() const noexcept
{
  return currentSize_;
}

template< typename T >
void shramko::ForwardList< T >::insertFrontNode(ListNode< T >* newNode)
{
  newNode->nextPtr = headNode_;
  headNode_ = newNode;
  if (isEmpty())
  {
    tailNode_ = newNode;
  }
  tailNode_->nextPtr = headNode_;
  currentSize_++;
}

template< typename T >
void shramko::ForwardList< T >::addToFront(const T& value)
{
  insertFrontNode(new ForwardListNode<T>(value));
}

template< typename T >
void shramko::ForwardList< T >::addToFront(T&& value)
{
  insertFrontNode(new ForwardListNode<T>(std::move(value)));
}

template< typename T >
void shramko::ForwardList< T >::removeFront()
{
  if (!isEmpty())
  {
    ForwardListNode< T >* oldHead = headNode_;
    headNode_ = headNode_->next_;
    tailNode_->next_ = headNode_;
    delete oldHead;
    currentSize_--;
    if (isEmpty())
    {
      headNode_ = tailNode_ = nullptr;
    }
  }
}

template< typename T >
void shramko::ForwardList< T >::insertBackNode(ListNode< T >* newNode)
{
  if (!isEmpty())
  {
    tailNode_->nextPtr = newNode;
    tailNode_ = newNode;
    tailNode_->nextPtr = headNode_;
  }
  else
  {
    headNode_ = tailNode_ = newNode;
    tailNode_->nextPtr = headNode_;
  }
  currentSize_++;
}

template< typename T >
void shramko::ForwardList< T >::addToBack(const T& value)
{
  insertBackNode(new ForwardListNode<T>(value));
}

template< typename T >
void shramko::ForwardList< T >::addToBack(T&& value)
{
  insertBackNode(new ForwardListNode<T>(std::move(value)));
}

template< typename T >
void shramko::ForwardList< T >::swapLists(ForwardList& other)
{
  std::swap(headNode_, other.headNode_);
  std::swap(tailNode_, other.tailNode_);
  std::swap(currentSize_, other.currentSize_);
}

template< typename T >
void shramko::ForwardList< T >::clearAll() noexcept
{
  while (!isEmpty())
  {
    removeFront();
  }
}

#endif
