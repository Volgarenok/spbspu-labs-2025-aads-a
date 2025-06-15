#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include "NodeFwdList.hpp"

#include <iostream>
namespace gavrilova {

  template < class T >
  struct IteratorFwd;
  template < class T >
  struct ConstIteratorFwd;

  template < class T >
  class FwdList {
  public:
    using Iterator = gavrilova::IteratorFwd< T >;
    using CIterator = gavrilova::ConstIteratorFwd< T >;

    FwdList();
    FwdList(const FwdList& other);
    FwdList(FwdList&& other) noexcept;
    FwdList(std::initializer_list< T > init);
    ~FwdList();

    FwdList& operator=(const FwdList& other);
    FwdList& operator=(FwdList&& other) noexcept;
    FwdList& operator=(std::initializer_list< T > init);

    bool operator==(const FwdList& other) const noexcept;
    bool operator!=(const FwdList& other) const noexcept;
    bool operator<(const FwdList& other) const noexcept;
    bool operator>(const FwdList& other) const noexcept;
    bool operator<=(const FwdList& other) const noexcept;
    bool operator>=(const FwdList& other) const noexcept;

    Iterator begin() const noexcept;
    Iterator end() const noexcept;

    CIterator cbegin() const noexcept;
    CIterator cend() const noexcept;

    T& front();
    const T& front() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void push_front(const T& value);
    void push_front(T&& value);
    void pop_front() noexcept;
    void clear() noexcept;

    void remove(const T& value) noexcept;
    template < class UnaryPredicate >
    void remove_if(UnaryPredicate p) noexcept;

    void splice(CIterator pos, FwdList< T >&) noexcept;
    void splice(CIterator pos, FwdList< T >&&) noexcept;
    void splice(CIterator pos, FwdList< T >&, CIterator it) noexcept;
    void splice(CIterator pos, FwdList< T >&&, CIterator it) noexcept;
    void splice(CIterator pos, FwdList< T >&, CIterator first, CIterator last) noexcept;
    void splice(CIterator pos, FwdList< T >&&, CIterator first, CIterator last) noexcept;

    void reverse();
    void swap(FwdList< T >& other) noexcept;

    void assign(size_t count, const T& value);
    template < class InputIt >
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list< T > init);

    Iterator insert(CIterator pos, const T& value);
    Iterator insert(CIterator pos, T&& value);
    Iterator insert(CIterator pos, size_t count, const T& value);
    template < class InputIt >
    Iterator insert(CIterator pos, InputIt first, InputIt last);
    Iterator insert(CIterator pos, std::initializer_list< T > init);

    Iterator erase(CIterator pos);
    Iterator erase(CIterator first, CIterator last);

  private:
    NodeFwdList< T >* fake_;
    size_t nodeCount_;
  };

  template < class T >
  FwdList< T >::FwdList():
    fake_(reinterpret_cast< NodeFwdList< T >* >(new char[sizeof(NodeFwdList< T >)])),
    nodeCount_(0)
  {
    fake_->next = fake_;
  }

  template < class T >
  FwdList< T >::FwdList(const FwdList& other):
    FwdList()
  {
    assign(other.begin(), other.end());
  }

  template < class T >
  FwdList< T >::FwdList(FwdList&& other) noexcept:
    fake_(other.fake_),
    nodeCount_(other.nodeCount_)
  {
    other.fake_ = nullptr;
    other.nodeCount_ = 0;
  }

  template < class T >
  FwdList< T >::FwdList(std::initializer_list< T > init):
    FwdList()
  {
    assign(init);
  }

  template < class T >
  FwdList< T >& FwdList< T >::operator=(const FwdList& other)
  {
    if (this != std::addressof(other)) {
      FwdList< T > cpy(other);
      swap(cpy);
    }
    return *this;
  }

  template < class T >
  FwdList< T >& FwdList< T >::operator=(FwdList&& other) noexcept
  {
    if (this == &other) {
      return *this;
    }
    clear();
    delete[] reinterpret_cast< char* >(fake_);
    fake_ = other.fake_;
    nodeCount_ = other.nodeCount_;

    other.fake_ = nullptr;
    other.nodeCount_ = 0;

    return *this;
  }

  template < class T >
  FwdList< T >& FwdList< T >::operator=(std::initializer_list< T > init)
  {
    FwdList< T > temp(init);
    swap(temp);
    return *this;
  }

  template < class T >
  FwdList< T >::~FwdList()
  {
    clear();
    delete[] reinterpret_cast< char* >(fake_);
  }

  template < class T >
  typename FwdList< T >::Iterator FwdList< T >::begin() const noexcept
  {
    return Iterator(fake_->next);
  }

  template < class T >
  typename FwdList< T >::Iterator FwdList< T >::end() const noexcept
  {
    return Iterator(fake_);
  }

  template < class T >
  typename FwdList< T >::CIterator FwdList< T >::cbegin() const noexcept
  {
    return CIterator(fake_->next);
  }

  template < class T >
  typename FwdList< T >::CIterator FwdList< T >::cend() const noexcept
  {
    return CIterator(fake_);
  }

  template < class T >
  T& FwdList< T >::front()
  {
    if (empty()) {
      throw std::out_of_range("List is empty");
    }
    return fake_->next->data;
  }

  template < class T >
  const T& FwdList< T >::front() const
  {
    if (empty()) {
      throw std::out_of_range("List is empty");
    }
    return fake_->next->data;
  }

  template < class T >
  bool FwdList< T >::empty() const noexcept
  {
    return nodeCount_ == 0;
  }

  template < class T >
  size_t FwdList< T >::size() const noexcept
  {
    return nodeCount_;
  }

  template < class T >
  void FwdList< T >::push_front(const T& value)
  {
    NodeFwdList< T >* newNode = new NodeFwdList< T >{value, fake_->next};
    fake_->next = newNode;
    ++nodeCount_;
  }

  template < class T >
  void FwdList< T >::push_front(T&& value)
  {
    NodeFwdList< T >* newNode = new NodeFwdList< T >{std::move(value), fake_->next};
    fake_->next = newNode;
    ++nodeCount_;
  }

  template < class T >
  void FwdList< T >::pop_front() noexcept
  {
    if (empty()) {
      return;
    }
    NodeFwdList< T >* temp = fake_->next;

    fake_->next = fake_->next->next;
    delete temp;
    --nodeCount_;
  }

  template < class T >
  void FwdList< T >::clear() noexcept
  {
    while (!empty()) {
      pop_front();
    }
  }

  template < class T >
  void FwdList< T >::remove(const T& value) noexcept
  {
    struct IsEqual {
      const T& value;
      IsEqual(const T& v):
        value(v)
      {}
      bool operator()(const T& data) const
      {
        return data == value;
      }
    };

    remove_if(IsEqual(value));
  }

  template < class T >
  template < class UnaryPredicate >
  void FwdList< T >::remove_if(UnaryPredicate p) noexcept
  {
    NodeFwdList< T >* current = fake_;
    while (current->next != fake_) {
      if (p(current->next->data)) {
        NodeFwdList< T >* temp = current->next;
        current->next = current->next->next;
        delete temp;
        --nodeCount_;
      } else {
        current = current->next;
      }
    }
  }

  template < class T >
  void FwdList< T >::splice(CIterator pos, FwdList< T >& other) noexcept
  {
    if (other.empty()) {
      return;
    }
    NodeFwdList< T >* first_other = other.fake_->next;
    NodeFwdList< T >* last_other = first_other;

    while (last_other->next != other.fake_) {
      last_other = last_other->next;
    }
    other.fake_->next = other.fake_;

    NodeFwdList< T >* node = pos.node_;
    NodeFwdList< T >* node_next = node->next;
    node->next = first_other;
    last_other->next = node_next;

    nodeCount_ += other.nodeCount_;
    other.nodeCount_ = 0;
  }

  template < class T >
  void FwdList< T >::splice(CIterator pos, FwdList< T >&& other) noexcept
  {
    splice(pos, other);
  }

  template < class T >
  void FwdList< T >::splice(CIterator pos, FwdList< T >& other, CIterator it) noexcept
  {
    auto it_end = it;
    splice(pos, other, it, ++it_end);
  }

  template < class T >
  void FwdList< T >::splice(CIterator pos, FwdList< T >&& other, CIterator it) noexcept
  {
    splice(pos, other, it);
  }

  template < class T >
  void FwdList< T >::splice(CIterator pos, FwdList< T >& other, CIterator first, CIterator last) noexcept
  {
    if (this == std::addressof(other) || (first == last && first != CIterator(other.fake_))) {
      return;
    }

    size_t count = 0;
    NodeFwdList< T >* before_first = first.node_;
    NodeFwdList< T >* last_node = before_first->next;

    while (last_node != last.node_ && last_node != other.fake_) {
      last_node = last_node->next;
      ++count;
    }

    if (last_node == other.fake_ && last.node_ != other.fake_) {
      return;
    }
    NodeFwdList< T >* range_first = before_first->next;
    NodeFwdList< T >* range_last = last_node;
    before_first->next = last_node->next;
    other.nodeCount_ -= count;

    NodeFwdList< T >* pos_node = pos.node_;
    NodeFwdList< T >* pos_next = pos_node->next;

    pos_node->next = range_first;
    range_last->next = pos_next;
    nodeCount_ += count;

    if (other.empty()) {
      other.fake_->next = other.fake_;
    }
  }

  template < class T >
  void FwdList< T >::splice(CIterator pos, FwdList< T >&& other, CIterator first, CIterator last) noexcept
  {
    splice(pos, other, first, last);
  }

  template < class T >
  void FwdList< T >::reverse()
  {
    if (empty()) {
      return;
    }
    NodeFwdList< T >* next = fake_->next;
    NodeFwdList< T >* last = fake_;
    while (next->next != fake_) {
      NodeFwdList< T >* remember = next->next;
      next->next = last;
      last = next;
      next = remember;
    }
    next->next = last;
    fake_->next = next;
  }

  template < class T >
  bool FwdList< T >::operator==(const FwdList< T >& other) const noexcept
  {
    if (nodeCount_ != other.nodeCount_) {
      return false;
    }

    Iterator it1 = begin();
    Iterator it2 = other.begin();
    while (it1 != end()) {
      if (*it1 != *it2)
        return false;
      ++it1;
      ++it2;
    }
    return true;
  }

  template < class T >
  bool FwdList< T >::operator!=(const FwdList< T >& other) const noexcept
  {
    return !(*this == other);
  }

  template < class T >
  bool FwdList< T >::operator<(const FwdList< T >& other) const noexcept
  {
    Iterator it1 = begin();
    Iterator it2 = other.begin();
    while (it1 != end() && it2 != other.end()) {
      if (*it1 < *it2) {
        return true;
      }
      if (*it1 > *it2) {
        return false;
      }
      ++it1;
      ++it2;
    }
    return nodeCount_ < other.nodeCount_;
  }

  template < class T >
  bool FwdList< T >::operator>(const FwdList< T >& other) const noexcept
  {
    return other < *this;
  }

  template < class T >
  bool FwdList< T >::operator>=(const FwdList< T >& other) const noexcept
  {
    return !(other < *this);
  }

  template < class T >
  bool FwdList< T >::operator<=(const FwdList< T >& other) const noexcept
  {
    return !(other > *this);
  }

  template < class T >
  void FwdList< T >::swap(FwdList< T >& other) noexcept
  {
    std::swap(fake_, other.fake_);
    std::swap(nodeCount_, other.nodeCount_);
  }

  template < class T >
  void FwdList< T >::assign(size_t count, const T& value)
  {
    FwdList< T > temporary;
    for (size_t i = 0; i < count; ++i) {
      temporary.push_front(value);
    }
    swap(temporary);
  }

  template < class T >
  template < class InputIt >
  void FwdList< T >::assign(InputIt first, InputIt last)
  {
    FwdList< T > temporary;
    auto it_temp = temporary.begin();
    for (auto it = first; it != last; ++it) {
      temporary.insert(it_temp, *it);
      ++it_temp;
    }
    swap(temporary);
  }

  template < class T >
  void FwdList< T >::assign(std::initializer_list< T > init)
  {
    assign(init.begin(), init.end());
  }

  template < class T >
  typename FwdList< T >::Iterator FwdList< T >::insert(CIterator pos, const T& value)
  {
    NodeFwdList< T >* node = pos.node_;
    if (node == fake_) {
      push_front(value);
      return begin();
    }
    NodeFwdList< T >* node_next = node->next;
    node->next = new NodeFwdList< T >{value, node_next};
    ++nodeCount_;
    return Iterator(node->next);
  }

  template < class T >
  typename FwdList< T >::Iterator FwdList< T >::insert(CIterator pos, T&& value)
  {
    NodeFwdList< T >* node = pos.node_;
    if (node == fake_) {
      push_front(std::move(value));
      return begin();
    }
    NodeFwdList< T >* node_next = node->next;
    node->next = new NodeFwdList< T >{std::move(value), node_next};
    ++nodeCount_;
    return Iterator(node->next);
  }

  template < class T >
  typename FwdList< T >::Iterator FwdList< T >::insert(CIterator pos, size_t count, const T& value)
  {
    if (count == 0) {
      return Iterator(pos.node_);
    }

    Iterator result = insert(pos, value);
    if (count != 1) {
      FwdList< T > values(--count, value);
      splice(pos, values);
    }
    return result;
  }

  template < class T >
  template < class InputIt >
  typename FwdList< T >::Iterator FwdList< T >::insert(CIterator pos, InputIt first, InputIt last)
  {
    if (first == last) {
      return Iterator(pos.node_);
    }
    Iterator result = insert(pos, *first);
    FwdList< T > tempList(++first, last);
    splice(pos, tempList);
    return result;
  }

  template < class T >
  typename FwdList< T >::Iterator FwdList< T >::insert(CIterator pos, std::initializer_list< T > init)
  {
    if (init.size() == 0) {
      return Iterator(pos.node_);
    }

    return insert(pos, init.begin(), init.end());
  }

  template < class T >
  typename FwdList< T >::Iterator FwdList< T >::erase(CIterator pos)
  {
    if (empty()) {
      return end();
    }
    NodeFwdList< T >* prevNode = pos.node_;
    NodeFwdList< T >* nodeToDelete = prevNode->next;
    if (nodeToDelete == fake_) {
      return (Iterator{end()});
    }
    prevNode->next = nodeToDelete->next;
    delete nodeToDelete;
    --nodeCount_;
    return Iterator{prevNode->next};
  }

  template < class T >
  typename FwdList< T >::Iterator FwdList< T >::erase(CIterator first, CIterator last)
  {
    Iterator erased;
    for (auto it = first; it != last; it++) {
      erased = erase(it);
    }
    return erased;
  }

}

#endif
