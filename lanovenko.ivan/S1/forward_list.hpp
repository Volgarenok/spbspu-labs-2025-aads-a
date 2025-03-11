#ifndef FORWARD_LIST
#define FORWARD_LIST

#include <cassert>
#include <cstddef>
#include <utility>
#include <stdexcept>

namespace lanovenko
{
  template<typename T> class ForwardList
  {
  public:
    ForwardList();
    ~ForwardList();
    ForwardList(const ForwardList&);
    ForwardList(ForwardList&&) noexcept;
    ForwardList<T>& operator=(ForwardList);
    ForwardList<T>& operator=(ForwardList&&) noexcept;

    class ForwardListNode
    {
    public:
      friend class ForwardList<T>;
      friend class ForwardListIterator;
      ForwardListNode(T data);
    private:
      T data_;
      ForwardListNode* next_;
    };

    class ForwardListIterator final : public std::iterator<std::forward_iterator_tag, T>
    {
    public:
      friend class ForwardList<T>;
      using this_t = ForwardListIterator;
      ForwardListIterator();
      ForwardListIterator(ForwardListNode* node);
      ForwardListIterator(const this_t&) = default;
      this_t& operator=(const this_t&) = default;
      this_t& operator++();
      this_t operator++(int);
      T& operator*();
      T* operator->();
      bool operator!=(const this_t&) const;
      bool operator==(const this_t&) const;
    private:
      ForwardListNode* node_;
      bool firstIteration_;
    };

    ForwardListIterator begin() const noexcept;
    ForwardListIterator end() const noexcept;
    T& front();
    T& back();
    bool empty() const noexcept;
    size_t size() const noexcept;
    void push_front(const T& value);
    void push_front(T&& value);
    void pop_front();
    void swap(ForwardList& fwsdlst);
    void clear() noexcept;
    void push_back(const T& value);
    void push_back(T&& value);
  private:
    ForwardListNode* head_;
    ForwardListNode* tail_;
    size_t size_;
    void push_front_value(ForwardListNode* node);
    void push_back_value(ForwardListNode* node);
  };
}

template < typename T >
lanovenko::ForwardList< T >::ForwardListNode::ForwardListNode(T data):
  data_(data),
  next_(nullptr)
{}

template < typename T >
lanovenko::ForwardList< T >::ForwardList():
  head_(nullptr),
  tail_(nullptr),
  size_(0)
{}

template < typename T >
lanovenko::ForwardList<T>::ForwardList(const ForwardList& rhs):
  head_(nullptr),
  tail_(nullptr),
  size_(0)
{
  ForwardList<T> temporary{};
  for (ForwardListIterator it = rhs.begin(); it != rhs.end(); it++)
  {
    temporary.push_front(*it);
  }
  for (ForwardListIterator it = temporary.begin(); it != temporary.end(); it++)
  {
    push_front(*it);
  }
}

template <typename T>
lanovenko::ForwardList<T>::ForwardList(ForwardList&& rhs) noexcept:
  head_(rhs.head_),
  tail_(rhs.tail_),
  size_(rhs.size_)
{
  rhs.head_ = rhs.tail_ = nullptr;
  rhs.size_ = 0;
}

template <typename T>
lanovenko::ForwardList<T>& lanovenko::ForwardList<T>::operator=(ForwardList rhs)
{
  swap(rhs);
  return *this;
}

template <typename T>
lanovenko::ForwardList<T>& lanovenko::ForwardList<T>::operator=(ForwardList&& rhs) noexcept
{
  if (this != &rhs)
  {
    clear();
    head_ = rhs.head_;
    tail_ = rhs.tail_;
    size_ = rhs.size_;
    rhs.tail_ = rhs.head_ = nullptr;
    rhs.size_ = 0;
  }
  return *this;
}

template <typename T>
lanovenko::ForwardList<T>::~ForwardList()
{
  clear();
}

template <typename T>
lanovenko::ForwardList<T>::ForwardListIterator::ForwardListIterator():
  node_(nullptr),
  firstIteration_(true)
{}

template <typename T>
lanovenko::ForwardList<T>::ForwardListIterator::ForwardListIterator(ForwardListNode* node):
  node_(node),
  firstIteration_(true)
{}

template <typename T>
typename lanovenko::ForwardList<T>::ForwardListIterator& lanovenko::ForwardList<T>::ForwardListIterator::operator++()
{
  assert(node_ != nullptr);
  node_ = node_->next_;
  firstIteration_ = false;
  return *this;
}

template <typename T>
typename lanovenko::ForwardList<T>::ForwardListIterator lanovenko::ForwardList<T>::ForwardListIterator::operator++(int)
{
  assert(node_ != nullptr);
  this_t result(*this);
  ++(*this);
  return result;
}

template <typename T>
bool lanovenko::ForwardList<T>::ForwardListIterator::operator==(const this_t& rhs) const
{
  return (node_ == rhs.node_ && this->firstIteration_ == rhs.firstIteration_);
}

template <typename T>
bool lanovenko::ForwardList<T>::ForwardListIterator::operator!=(const this_t& rhs) const
{
  return !(*this == rhs);
}

template <typename T>
T& lanovenko::ForwardList<T>::ForwardListIterator::operator*()
{
  assert(node_ != nullptr);
  return node_->data_;
}

template <typename T>
T* lanovenko::ForwardList<T>::ForwardListIterator::operator->()
{
  assert(node_ != nullptr);
  return std::addressof(node_->data_);
}

template <typename T>
typename lanovenko::ForwardList<T>::ForwardListIterator lanovenko::ForwardList<T>::begin() const noexcept
{
  return ForwardListIterator(head_);
}

template <typename T>
typename lanovenko::ForwardList<T>::ForwardListIterator lanovenko::ForwardList<T>::end() const noexcept
{
  if (size_ == 0)
  {
    return ForwardListIterator(nullptr);
  }
  ForwardListIterator it(tail_->next_);
  it.firstIteration_ = false;
  return it;
}

template <typename T>
T& lanovenko::ForwardList<T>::front()
{
  if (empty())
  {
    throw std::logic_error("Empty List");
  }
  return head_->data_;
}

template <typename T>
T& lanovenko::ForwardList<T>::back()
{
  if (empty())
  {
    throw std::logic_error("Empty List");
  }
  return tail_->data_;
}

template <typename T>
bool lanovenko::ForwardList<T>::empty() const noexcept
{
  return size_ == 0;
}

template <typename T>
size_t lanovenko::ForwardList<T>::size() const noexcept
{
  return size_;
}

template <typename T>
void lanovenko::ForwardList<T>::push_front_value(ForwardListNode* newNode)
{
  newNode->next_ = head_;
  head_ = newNode;
  if (empty())
  {
    tail_ = newNode;
  }
  tail_->next_ = head_;
  size_++;
}

template <typename T>
void lanovenko::ForwardList<T>::push_front(const T& value)
{
  push_front_value(new ForwardListNode(value));
}

template <typename T>
void lanovenko::ForwardList<T>::push_front(T&& value)
{
  push_front_value(new ForwardListNode(std::move(value)));
}

template <typename T>
void lanovenko::ForwardList<T>::pop_front()
{
  if (!empty())
  {
    ForwardListNode* toDelete = head_;
    head_ = head_->next_;
    tail_->next_ = head_;
    delete toDelete;
    size_--;
  }
  if (empty())
  {
    head_ = nullptr;
    tail_ = nullptr;
  }
}

template<typename T>
void lanovenko::ForwardList<T>::push_back_value(ForwardListNode* newNode)
{
  if (!empty())
  {
    tail_->next_ = newNode;
    tail_ = newNode;
    tail_->next_ = head_;
  }
  if (empty())
  {
    head_ = newNode;
    tail_ = newNode;
    tail_->next_ = newNode;
  }
  size_++;
}

template<typename T>
void lanovenko::ForwardList<T>::push_back(const T& value)
{
  push_back_value(new ForwardListNode(value));
}

template<typename T>
void lanovenko::ForwardList<T>::push_back(T&& value)
{
  push_back_value(new ForwardListNode(std::move(value)));
}

template <typename T>
void lanovenko::ForwardList<T>::swap(ForwardList& fwdlst)
{
  std::swap(head_, fwdlst.head_);
  std::swap(tail_, fwdlst.tail_);
  std::swap(size_, fwdlst.size_);
}

template <typename T>
void lanovenko::ForwardList<T>::clear() noexcept
{
  while (!empty())
  {
    pop_front();
  }
}

#endif
