#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_FORWARD_LIST_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_FORWARD_LIST_HPP

#include <cassert>
#include <initializer_list>
#include <memory>
#include <utility>
#include "forward-list-comparison.hpp"
#include "forward-list-iterator.hpp"
#include "forward-list-swap.hpp"
#include "type-utils.hpp"

namespace kizhin {
  template < typename T >
  class ForwardList final
  {
  public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = std::size_t;
    using iterator = detail::ForwardListIterator< value_type, false >;
    using const_iterator = detail::ForwardListIterator< value_type, true >;

    ForwardList();
    ForwardList(const ForwardList&);
    ForwardList(ForwardList&&) noexcept;
    explicit ForwardList(size_type, const_reference = value_type());
    template < typename InputIt, enable_if_input_iterator< InputIt > = 0 >
    ForwardList(InputIt, InputIt);
    ForwardList(std::initializer_list< value_type >);
    ~ForwardList();

    ForwardList& operator=(const ForwardList&);
    ForwardList& operator=(ForwardList&&) noexcept;
    ForwardList& operator=(std::initializer_list< value_type >);

    iterator begin() noexcept;
    iterator end() noexcept;
    iterator beforeBegin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator beforeBegin() const noexcept;

    reference front() noexcept;
    reference back() noexcept;
    const_reference front() const noexcept;
    const_reference back() const noexcept;
    bool empty() const noexcept;
    size_type size() const noexcept;

    void pushBack(const_reference);
    void pushBack(value_type&&);
    void pushFront(const_reference);
    void pushFront(value_type&&);
    template < typename... Args >
    void emplaceBack(Args&&...);
    template < typename... Args >
    void emplaceFront(Args&&...);
    template < typename... Args >
    iterator emplaceAfter(const_iterator, Args&&...);

    void popBack() noexcept;
    void popFront() noexcept;
    iterator eraseAfter(const_iterator);
    iterator eraseAfter(const_iterator, const_iterator);

    void assign(size_type, const_reference);
    template < typename InputIt, enable_if_input_iterator< InputIt > = 0 >
    void assign(InputIt, InputIt);
    void assign(std::initializer_list< value_type >);

    iterator insertAfter(const_iterator, value_type);
    iterator insertAfter(const_iterator, size_type, const_reference);
    template < typename InputIt, enable_if_input_iterator< InputIt > = 0 >
    iterator insertAfter(const_iterator, InputIt, InputIt);
    iterator insertAfter(const_iterator, std::initializer_list< value_type >);

    void clear() noexcept;
    void swap(ForwardList&) noexcept;

    void reverse() noexcept;
    void spliceAfter(const_iterator, ForwardList&);
    void spliceAfter(const_iterator, ForwardList&, const_iterator);
    void spliceAfter(const_iterator, ForwardList&, const_iterator, const_iterator);

    void remove(const_reference);
    template < typename UnaryPredicate >
    void removeIf(UnaryPredicate);

    void unique();
    template < typename BinaryPredicate >
    void unique(BinaryPredicate);

    void merge(ForwardList&);
    template < typename Comparator >
    void merge(ForwardList&, Comparator);

    void sort();
    template < typename Comparator >
    void sort(Comparator);

  private:
    using Node = detail::Node< value_type >;

    Node* beforeBegin_;
    Node* end_;
    size_type size_;
  };

  template < typename T >
  ForwardList< T >::ForwardList():
    beforeBegin_(static_cast< Node* >(operator new(sizeof(Node)))),
    end_(beforeBegin_),
    size_(0)
  {
    beforeBegin_->next = nullptr;
  }

  template < typename T >
  ForwardList< T >::ForwardList(const ForwardList& rhs):
    ForwardList()
  {
    insertAfter(beforeBegin(), rhs.begin(), rhs.end());
  }

  template < typename T >
  ForwardList< T >::ForwardList(ForwardList&& rhs) noexcept:
    beforeBegin_(std::exchange(rhs.beforeBegin_, nullptr)),
    end_(std::exchange(rhs.end_, nullptr)),
    size_(std::exchange(rhs.size_, 0))
  {}

  template < typename T >
  ForwardList< T >::ForwardList(size_type size, const_reference value):
    ForwardList()
  {
    insertAfter(beforeBegin(), size, value);
  }

  template < typename T >
  template < typename InputIt, enable_if_input_iterator< InputIt > >
  ForwardList< T >::ForwardList(InputIt first, InputIt last):
    ForwardList()
  {
    insertAfter(beforeBegin(), first, last);
  }

  template < typename T >
  ForwardList< T >::ForwardList(std::initializer_list< value_type > init):
    ForwardList(init.begin(), init.end())
  {}

  template < typename T >
  ForwardList< T >::~ForwardList()
  {
    clear();
    operator delete(beforeBegin_);
  }

  template < typename T >
  ForwardList< T >& ForwardList< T >::operator=(const ForwardList& rhs)
  {
    ForwardList tmp(rhs);
    swap(tmp);
    return *this;
  }

  template < typename T >
  ForwardList< T >& ForwardList< T >::operator=(ForwardList&& rhs) noexcept
  {
    clear();
    operator delete(beforeBegin_);
    beforeBegin_ = std::exchange(rhs.beforeBegin_, nullptr);
    end_ = std::exchange(rhs.end_, nullptr);
    size_ = std::exchange(rhs.size_, 0);
    return *this;
  }

  template < typename T >
  ForwardList< T >& ForwardList< T >::operator=(std::initializer_list< value_type > init)
  {
    ForwardList tmp(init);
    swap(tmp);
    return *this;
  }

  template < typename T >
  typename ForwardList< T >::iterator ForwardList< T >::begin() noexcept
  {
    return iterator(beforeBegin_->next);
  }

  template < typename T >
  typename ForwardList< T >::iterator ForwardList< T >::end() noexcept
  {
    return iterator(nullptr);
  }

  template < typename T >
  typename ForwardList< T >::iterator ForwardList< T >::beforeBegin() noexcept
  {
    return iterator(beforeBegin_);
  }

  template < typename T >
  typename ForwardList< T >::const_iterator ForwardList< T >::begin() const noexcept
  {
    return const_iterator(beforeBegin_->next);
  }

  template < typename T >
  typename ForwardList< T >::const_iterator ForwardList< T >::end() const noexcept
  {
    return const_iterator(nullptr);
  }

  template < typename T >
  typename ForwardList< T >::const_iterator ForwardList< T >::beforeBegin() const noexcept
  {
    return const_iterator(beforeBegin_);
  }

  template < typename T >
  typename ForwardList< T >::reference ForwardList< T >::front() noexcept
  {
    assert(!empty() && "ForwardList: front() called on empty list");
    return beforeBegin_->next->data;
  }

  template < typename T >
  typename ForwardList< T >::reference ForwardList< T >::back() noexcept
  {
    assert(!empty() && "ForwardList: back() called on empty list");
    return end_->data;
  }

  template < typename T >
  typename ForwardList< T >::const_reference ForwardList< T >::front() const noexcept
  {
    assert(!empty() && "ForwardList: front() called on empty list");
    return beforeBegin_->next->data;
  }

  template < typename T >
  typename ForwardList< T >::const_reference ForwardList< T >::back() const noexcept
  {
    assert(!empty() && "ForwardList: back() called on empty list");
    return end_->data;
  }

  template < typename T >
  bool ForwardList< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < typename T >
  typename ForwardList< T >::size_type ForwardList< T >::size() const noexcept
  {
    return size_;
  }

  template < typename T >
  void ForwardList< T >::pushBack(const_reference value)
  {
    emplaceBack(value);
  }

  template < typename T >
  void ForwardList< T >::pushBack(value_type&& value)
  {
    emplaceBack(std::move(value));
  }

  template < typename T >
  void ForwardList< T >::pushFront(const_reference value)
  {
    emplaceFront(value);
  }

  template < typename T >
  void ForwardList< T >::pushFront(value_type&& value)
  {
    emplaceFront(std::move(value));
  }

  template < typename T >
  template < typename... Args >
  void ForwardList< T >::emplaceBack(Args&&... args)
  {
    Node* newNode = new Node{ value_type(std::forward< Args >(args)...), nullptr };
    end_->next = newNode;
    end_ = end_->next;
    ++size_;
  }

  template < typename T >
  template < typename... Args >
  void ForwardList< T >::emplaceFront(Args&&... args)
  {
    Node* newNode = new Node{ value_type(std::forward< Args >(args)...), nullptr };
    newNode->next = beforeBegin_->next;
    beforeBegin_->next = newNode;
    if (newNode->next == nullptr) {
      end_ = newNode;
    }
    ++size_;
  }

  template < typename T >
  template < typename... Args >
  typename ForwardList< T >::iterator ForwardList< T >::emplaceAfter(
      const_iterator position, Args&&... args)
  {
    assert(position != end() && "ForwardList: emplaceAfter called with end iterator");
    if (position == beforeBegin()) {
      emplaceFront(std::forward< Args >(args)...);
      return iterator(beforeBegin_->next);
    }
    if (position.node_ == end_) {
      emplaceBack(std::forward< Args >(args)...);
      return iterator(end_);
    }
    Node* prev = position.node_;
    prev->next = new Node{ value_type(std::forward< Args >(args)...), prev->next };
    ++size_;
    return iterator(prev->next);
  }

  template < typename T >
  void ForwardList< T >::popBack() noexcept
  {
    assert(!empty() && "ForwardList: popBack() called on empty list");
    Node* current = beforeBegin_;
    while (current->next != end_) {
      current = current->next;
    }
    delete current->next;
    current->next = nullptr;
    end_ = current;
    --size_;
  }

  template < typename T >
  void ForwardList< T >::popFront() noexcept
  {
    assert(!empty() && "ForwardList: popFront() called on empty list");
    Node* tmp = beforeBegin_->next->next;
    delete beforeBegin_->next;
    beforeBegin_->next = tmp;
    if (tmp == nullptr) {
      end_ = beforeBegin_;
    }
    --size_;
  }

  template < typename T >
  typename ForwardList< T >::iterator ForwardList< T >::eraseAfter(
      const_iterator position)
  {
    assert(!empty() && "ForwardList: cannot erase from empty list");
    assert(position != end() && "ForwardList: cannot erase after end iterator");
    assert(position.node_ != end_ && "ForwardList: cannot erase after last element");
    return eraseAfter(position, std::next(position, 2));
  }

  template < typename T >
  typename ForwardList< T >::iterator ForwardList< T >::eraseAfter(const_iterator first,
      const_iterator last)
  {
    assert(!empty() && "ForwardList: cannot erase from empty list");
    assert(first.node_ != last.node_ && "ForwardList: empty erase range (first, last)");
    Node* firstPtr = first.node_->next;
    Node* lastPtr = last.node_;
    while (firstPtr != lastPtr) {
      Node* tmp = firstPtr->next;
      delete firstPtr;
      firstPtr = tmp;
      --size_;
    }
    first.node_->next = lastPtr;
    if (lastPtr == nullptr) {
      end_ = first.node_;
    }
    return iterator(first.node_);
  }

  template < typename T >
  void ForwardList< T >::assign(size_type size, const_reference value)
  {
    ForwardList tmp(size, value);
    swap(tmp);
  }

  template < typename T >
  template < typename InputIt, enable_if_input_iterator< InputIt > >
  void ForwardList< T >::assign(InputIt first, InputIt last)
  {
    ForwardList tmp(first, last);
    swap(tmp);
  }

  template < typename T >
  void ForwardList< T >::assign(std::initializer_list< value_type > init)
  {
    ForwardList tmp(init);
    swap(tmp);
  }

  template < typename T >
  typename ForwardList< T >::iterator ForwardList< T >::insertAfter(
      const_iterator position, value_type value)
  {
    assert(position != end() && "ForwardList: insertAfter called with end iterator");
    return emplaceAfter(position, std::move(value));
  }

  template < typename T >
  typename ForwardList< T >::iterator ForwardList< T >::insertAfter(
      const_iterator position, size_type size, const_reference value)
  {
    assert(position != end() && "ForwardList: insertAfter called with end iterator");
    if (size == 0) {
      return iterator(position.node_);
    }
    iterator result = emplaceAfter(position, value);
    position = result;
    for (size_type i = 1; i != size; ++i) {
      position = emplaceAfter(position, value);
    }
    return result;
  }

  template < typename T >
  template < typename InputIt, enable_if_input_iterator< InputIt > >
  typename ForwardList< T >::iterator ForwardList< T >::insertAfter(
      const_iterator position, InputIt first, InputIt last)
  {
    assert(position != end() && "ForwardList: insertAfter called with end iterator");
    if (first == last) {
      return iterator(position.node_);
    }
    iterator result = emplaceAfter(position, *first);
    iterator pos = result;
    ++first;
    for (; first != last; ++first) {
      pos = emplaceAfter(pos, *first);
    }
    return result;
  }

  template < typename T >
  typename ForwardList< T >::iterator ForwardList< T >::insertAfter(
      const_iterator position, std::initializer_list< value_type > init)
  {
    assert(position != end() && "ForwardList: insertAfter called with end iterator");
    return insertAfter(position, init.begin(), init.end());
  }

  template < typename T >
  void ForwardList< T >::clear() noexcept
  {
    while (!empty()) {
      popFront();
    }
  }

  template < typename T >
  void ForwardList< T >::swap(ForwardList& rhs) noexcept
  {
    using std::swap;
    swap(beforeBegin_, rhs.beforeBegin_);
    swap(end_, rhs.end_);
    swap(size_, rhs.size_);
  }

  template < typename T >
  void ForwardList< T >::reverse() noexcept
  {
    Node* prev = nullptr;
    Node* curr = beforeBegin_->next;
    end_ = beforeBegin_->next;
    while (curr) {
      Node* next = curr->next;
      curr->next = prev;
      prev = curr;
      curr = next;
    }
    beforeBegin_->next = prev;
  }

  template < typename T >
  void ForwardList< T >::spliceAfter(const_iterator position, ForwardList& source)
  {
    if (!source.empty()) {
      spliceAfter(position, source, source.beforeBegin(), source.end());
    }
  }

  template < typename T >
  void ForwardList< T >::spliceAfter(const_iterator position, ForwardList& source,
      const_iterator sourcePosition)
  {
    spliceAfter(position, source, sourcePosition, std::next(sourcePosition, 2));
  }

  template < typename T >
  void ForwardList< T >::spliceAfter(const_iterator position, ForwardList& source,
      const_iterator first, const_iterator last)
  {
    if (first == last || std::addressof(source) == this) {
      return;
    }
    Node* next = position.node_->next;
    position.node_->next = first.node_->next;
    size_type distance = std::distance(first, last) - 1;
    Node* lastInserted = std::next(first, distance).node_;
    lastInserted->next = next;
    if (lastInserted->next == nullptr) {
      end_ = lastInserted;
    }
    first.node_->next = last.node_;
    if (last == source.end()) {
      source.end_ = first.node_;
    }
    size_ += distance;
    source.size_ -= distance;
  }

  template < typename T >
  void ForwardList< T >::remove(const_reference value)
  {
    const auto pred = [&value](const_reference rhs) -> bool
    {
      return value == rhs;
    };
    removeIf(pred);
  }

  template < typename T >
  template < typename UnaryPredicate >
  void ForwardList< T >::removeIf(UnaryPredicate p)
  {
    const_iterator curr = begin();
    const_iterator prev = beforeBegin();
    while (curr != end()) {
      if (p(*curr)) {
        curr = eraseAfter(prev);
      } else {
        prev = curr;
        ++curr;
      }
    }
  }

  template < typename T >
  void ForwardList< T >::unique()
  {
    return unique(std::equal_to< value_type >{});
  }

  template < typename T >
  template < typename BinaryPredicate >
  void ForwardList< T >::unique(BinaryPredicate p)
  {
    if (empty()) {
      return;
    }
    Node* curr = beforeBegin_->next;
    while (curr->next != nullptr) {
      if (p(curr->data, curr->next->data)) {
        Node* tmp = curr->next;
        curr->next = tmp->next;
        delete tmp;
        --size_;
      } else {
        curr = curr->next;
      }
    }
    end_ = curr;
  }

  template < typename T >
  void ForwardList< T >::merge(ForwardList& source)
  {
    merge(source, std::less< value_type >{});
  }

  template < typename T >
  template < typename Comparator >
  void ForwardList< T >::merge(ForwardList& source, Comparator comp)
  {
    if (this == std::addressof(source) || source.empty()) {
      return;
    }
    if (empty()) {
      swap(source);
      return;
    }
    iterator thisCurr = begin();
    iterator sourceCurr = source.begin();
    ForwardList result;
    while (thisCurr != end() && sourceCurr != source.end()) {
      if (comp(*thisCurr, *sourceCurr)) {
        result.emplaceBack(*thisCurr);
        ++thisCurr;
      } else {
        result.emplaceBack(*sourceCurr);
        ++sourceCurr;
      }
    }
    result.insertAfter(const_iterator(result.end_), thisCurr, end());
    result.insertAfter(const_iterator(result.end_), sourceCurr, source.end());
    source.clear();
    swap(result);
  }

  template < typename T >
  void ForwardList< T >::sort()
  {
    return sort(std::less< value_type >{});
  }

  template < typename T >
  template < typename Comparator >
  void ForwardList< T >::sort(Comparator comp)
  {
    if (size_ <= 1) {
      return;
    }
    const iterator mid = std::next(begin(), size_ / 2);
    ForwardList< T > left;
    ForwardList< T > right;
    left.spliceAfter(left.beforeBegin(), *this, beforeBegin(), mid);
    right.spliceAfter(right.beforeBegin(), *this, beforeBegin(), end());

    left.sort(comp);
    right.sort(comp);
    merge(left, comp);
    merge(right, comp);
  }
}

#endif

