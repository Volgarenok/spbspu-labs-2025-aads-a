#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_MAP_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_MAP_HPP

#include <algorithm>
#include <memory>
#include <tuple>
#include <utility>
#include "internal/map-node.hpp"
#include "queue.hpp"
#include "stack.hpp"
#include "type-utils.hpp"

namespace kizhin {
  template < typename Key, typename T, typename Comparator = std::less< Key > >
  class Map final
  {
  public:
    using value_type = std::pair< const Key, T >;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using key_type = Key;
    using mapped_type = T;
    using size_type = std::size_t;
    using key_compare = Comparator;

  private:
    template < bool isConst >
    class Iterator;

    template < bool isConst >
    class HeavyIterator;
    template < bool isConst >
    class LmrIterator;
    template < bool isConst >
    class RmlIterator;
    template < bool isConst >
    class BfsIterator;

  public:
    using iterator = Iterator< false >;
    using const_iterator = Iterator< true >;

    using lmr_iterator = LmrIterator< false >;
    using const_lmr_iterator = LmrIterator< true >;
    using rml_iterator = RmlIterator< false >;
    using const_rml_iterator = RmlIterator< true >;
    using bfs_iterator = BfsIterator< false >;
    using const_bfs_iterator = BfsIterator< true >;

    class value_compare;

  private:
    static constexpr bool is_nothrow_default_constructible =
        is_nothrow_default_constructible_v< key_compare >;
    static constexpr bool is_nothrow_move_constructible =
        is_nothrow_move_constructible_v< key_compare >;
    static constexpr bool is_nothrow_copy_constructible =
        is_nothrow_copy_constructible_v< key_compare >;
    static constexpr bool is_nothrow_move_assignable =
        is_nothrow_move_assignable_v< key_compare >;
    static constexpr bool is_nothrow_swappable = is_nothrow_swappable_v< key_compare >;

  public:
    Map() noexcept(is_nothrow_default_constructible) = default;
    Map(const Map&);
    Map(Map&&) noexcept(is_nothrow_move_constructible);
    explicit Map(const key_compare&) noexcept(is_nothrow_copy_constructible);
    template < typename InputIt >
    Map(InputIt, InputIt, const key_compare& = key_compare{});
    Map(std::initializer_list< value_type >, const key_compare& = key_compare{});
    ~Map();

    Map& operator=(const Map&);
    Map& operator=(Map&&) noexcept(is_nothrow_move_assignable);
    Map& operator=(std::initializer_list< value_type >);

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    lmr_iterator lmrBegin();
    lmr_iterator lmrEnd();
    const_lmr_iterator lmrBegin() const;
    const_lmr_iterator lmrEnd() const;

    rml_iterator rmlBegin();
    rml_iterator rmlEnd();
    const_rml_iterator rmlBegin() const;
    const_rml_iterator rmlEnd() const;

    bfs_iterator bfsBegin();
    bfs_iterator bfsEnd();
    const_bfs_iterator bfsBegin() const;
    const_bfs_iterator bfsEnd() const;

    size_type size() const noexcept;
    bool empty() const noexcept;

    mapped_type& operator[](const key_type&);
    mapped_type& operator[](key_type&&);

    mapped_type& at(const key_type&);
    const mapped_type& at(const key_type&) const;

    std::pair< iterator, bool > insert(const_reference);
    std::pair< iterator, bool > insert(value_type&&);
    iterator insert(const_iterator, const_reference);
    iterator insert(const_iterator, value_type&&);
    template < typename InputIt >
    void insert(InputIt, InputIt);
    void insert(std::initializer_list< value_type >);

    iterator erase(const_iterator);
    size_type erase(const key_type&);
    iterator erase(const_iterator, const_iterator);

    void clear() noexcept;
    void swap(Map&) noexcept(is_nothrow_swappable);

    template < typename... Args >
    std::pair< iterator, bool > emplace(Args&&...);
    template < typename... Args >
    iterator emplaceHint(const_iterator, Args&&...);

    key_compare keyComp() const;
    value_compare valueComp() const;

    iterator find(const key_type&);
    const_iterator find(const key_type&) const;

    size_type count(const key_type&) const;

    iterator lowerBound(const key_type&);
    const_iterator lowerBound(const key_type&) const;

    iterator upperBound(const key_type&);
    const_iterator upperBound(const key_type&) const;

    std::pair< iterator, iterator > equalRange(const key_type&);
    std::pair< const_iterator, const_iterator > equalRange(const key_type&) const;

    template < typename F >
    F traverseLmr(F) const;
    template < typename F >
    F traverseRml(F) const;
    template < typename F >
    F traverseBreadth(F) const;

  private:
    using Node = detail::Node< value_type >;
    class EndNodeGuard;

    Node* root_ = nullptr;
    size_type size_ = 0;
    Comparator comparator_;

    void deallocate() noexcept;
    Node* getEndNode() const noexcept;

    template < typename... Args >
    Node* emplaceToNode(Node*, Args&&...);
    template < typename... Args >
    iterator emplaceToEmpty(Args&&...);
    Node* eraseFromNode(Node*, const_pointer);
    void swapVals(Node* lhs, pointer lhsPtr, Node* rhs, pointer rhsPtr);

    Node* split(Node*);
    std::tuple< Node*, Node* > splitInTwo(const Node*);
    void splitChildren(const Node* src, Node* left, Node* right) const noexcept;

    pointer findKey(const Node*, const key_type&) const;
    Node* findTarget(Node*, const key_type&) const;
    Node* validateHint(Node*, const key_type&) const;

    Node* fixUnderflow(Node*);
    Node* fixRootUnderflow(Node*);

    bool isMergeable(const Node*) const;
    Node* merge(Node*);

    Node* redistribute(Node*);
    Node* giveToSibling(Node*);
    Node* borrowFromSibling(Node*);
    Node* borrowFromLeft(Node* target, Node* sibling);
    Node* borrowFromRight(Node* target, Node* sibling);
  };

  template < typename K, typename T, typename C >
  Map< K, T, C >::~Map()
  {
    if (!empty()) {
      deallocate();
    }
  }

  template < typename K, typename T, typename C >
  bool operator==(const Map< K, T, C >&, const Map< K, T, C >&);

  template < typename K, typename T, typename C >
  bool operator!=(const Map< K, T, C >&, const Map< K, T, C >&);

  template < typename K, typename T, typename C >
  bool operator<(const Map< K, T, C >&, const Map< K, T, C >&);

  template < typename K, typename T, typename C >
  bool operator>(const Map< K, T, C >&, const Map< K, T, C >&);

  template < typename K, typename T, typename C >
  bool operator<=(const Map< K, T, C >&, const Map< K, T, C >&);

  template < typename K, typename T, typename C >
  bool operator>=(const Map< K, T, C >&, const Map< K, T, C >&);

  template < typename K, typename T, typename C >
  void swap(Map< K, T, C >& l, Map< K, T, C >& r) noexcept(noexcept(l.swap(r)));
}

template < typename K, typename T, typename C >
template < bool IsConst >
class kizhin::Map< K, T, C >::Iterator
{
private:
  template < typename T1, typename T2 >
  using conditional_t = std::conditional_t< IsConst, T1, T2 >;

public:
  using value_type = Map::value_type;
  using difference_type = std::ptrdiff_t;
  using pointer = conditional_t< Map::const_pointer, Map::pointer >;
  using reference = conditional_t< Map::const_reference, Map::reference >;
  using iterator_category = std::bidirectional_iterator_tag;

  Iterator() noexcept = default;
  template < bool RhsConst, std::enable_if_t< IsConst && !RhsConst, int > = 0 >
  Iterator(const Iterator< RhsConst >&) noexcept;

  pointer operator->() const noexcept;
  reference operator*() const noexcept;

  Iterator& operator++();
  Iterator operator++(int);
  Iterator& operator--();
  Iterator operator--(int);

  friend bool operator==(const Iterator& lhs, const Iterator& rhs) noexcept
  {
    return rhs.node_ == lhs.node_ && rhs.valuePtr_ == lhs.valuePtr_;
  }

  friend bool operator!=(const Iterator& lhs, const Iterator& rhs) noexcept
  {
    return !(rhs == lhs);
  }

private:
  friend class Map;

  Node* node_ = nullptr;
  pointer valuePtr_ = nullptr;

  Iterator(Node*) noexcept;
  Iterator(Node*, pointer) noexcept;
};

template < typename K, typename T, typename C >
template < bool IsConst >
kizhin::Map< K, T, C >::Iterator< IsConst >::Iterator(Node* node) noexcept:
  node_(node),
  valuePtr_(node ? node->begin : nullptr)
{}

template < typename K, typename T, typename C >
template < bool IsConst >
kizhin::Map< K, T, C >::Iterator< IsConst >::Iterator(Node* node,
    pointer valuePtr) noexcept:
  node_(node),
  valuePtr_(valuePtr)
{}

template < typename K, typename T, typename C >
template < bool IsConst >
template < bool IsRhsConst, std::enable_if_t< IsConst && !IsRhsConst, int > >
kizhin::Map< K, T, C >::Iterator< IsConst >::Iterator(
    const Iterator< IsRhsConst >& rhs) noexcept:
  node_(rhs.node_),
  valuePtr_(rhs.valuePtr_)
{}

template < typename K, typename T, typename C >
template < bool IsConst >
auto kizhin::Map< K, T, C >::Iterator< IsConst >::operator->() const noexcept -> pointer
{
  assert(node_ && valuePtr_ && "Dereferencing empty iterator");
  return std::addressof(**this);
}

template < typename K, typename T, typename C >
template < bool IsConst >
auto kizhin::Map< K, T, C >::Iterator< IsConst >::operator*() const noexcept -> reference
{
  assert(node_ && valuePtr_ && "Dereferencing empty iterator");
  return *valuePtr_;
}

template < typename K, typename T, typename C >
template < bool IsConst >
auto kizhin::Map< K, T, C >::Iterator< IsConst >::operator++() -> Iterator&
{
  assert(node_ && valuePtr_ && "Incrementing empty iterator");
  std::tie(node_, valuePtr_) = detail::nextIter(node_, valuePtr_);
  return *this;
}

template < typename K, typename T, typename C >
template < bool IsConst >
auto kizhin::Map< K, T, C >::Iterator< IsConst >::operator++(int) -> Iterator
{
  Iterator result(*this);
  ++(*this);
  return result;
}

template < typename K, typename T, typename C >
template < bool IsConst >
auto kizhin::Map< K, T, C >::Iterator< IsConst >::operator--() -> Iterator&
{
  assert(node_ && valuePtr_ && "Decrementing empty iterator");
  std::tie(node_, valuePtr_) = detail::prevIter(node_, valuePtr_);
  return *this;
}

template < typename K, typename T, typename C >
template < bool IsConst >
auto kizhin::Map< K, T, C >::Iterator< IsConst >::operator--(int) -> Iterator
{
  Iterator result(*this);
  --(*this);
  return result;
}

template < typename K, typename T, typename C >
template < bool IsConst >
class kizhin::Map< K, T, C >::HeavyIterator
{
private:
  template < typename T1, typename T2 >
  using conditional_t = std::conditional_t< IsConst, T1, T2 >;

public:
  using value_type = Map::value_type;
  using difference_type = std::ptrdiff_t;
  using pointer = conditional_t< Map::const_pointer, Map::pointer >;
  using reference = conditional_t< Map::const_reference, Map::reference >;
  using iterator_category = std::forward_iterator_tag;

  pointer operator->() const noexcept { return std::addressof(**this); }
  reference operator*() const noexcept
  {
    assert(valuePtr_ && "Dereferencing empty HeavyIterator");
    return *valuePtr_;
  }
  operator Map::Iterator< IsConst >() { return { node_, valuePtr_ }; }

  friend bool operator==(const HeavyIterator& lhs, const HeavyIterator& rhs) noexcept
  {
    return lhs.node_ == rhs.node_ && lhs.valuePtr_ == rhs.valuePtr_;
  }

  friend bool operator!=(const HeavyIterator& lhs, const HeavyIterator& rhs) noexcept
  {
    return !(lhs == rhs);
  }

protected:
  Node* node_ = nullptr;
  pointer valuePtr_ = nullptr;
};

template < typename K, typename T, typename C >
template < bool IsConst >
class kizhin::Map< K, T, C >::LmrIterator: public HeavyIterator< IsConst >
{
public:
  using pointer = typename HeavyIterator< IsConst >::pointer;
  using reference = typename HeavyIterator< IsConst >::reference;

  LmrIterator() noexcept = default;
  template < bool RhsConst, std::enable_if_t< IsConst && !RhsConst, int > = 0 >
  LmrIterator(const LmrIterator< RhsConst >& rhs):
    HeavyIterator< IsConst >(rhs),
    values_(rhs.values_)
  {}
  template < bool RhsConst, std::enable_if_t< !IsConst || RhsConst, int > = 0 >
  LmrIterator(Iterator< RhsConst > rhs):
    LmrIterator(rhs.node_)
  {}

  LmrIterator& operator++();
  LmrIterator operator++(int)
  {
    LmrIterator result(*this);
    ++(*this);
    return result;
  }

private:
  friend class Map;

  Stack< std::pair< Node*, pointer > > values_{};
  using HeavyIterator< IsConst >::valuePtr_;
  using HeavyIterator< IsConst >::node_;

  LmrIterator(Node*);
};

template < typename K, typename T, typename C >
template < bool IsConst >
kizhin::Map< K, T, C >::LmrIterator< IsConst >::LmrIterator(Node* root):
  HeavyIterator< IsConst >()
{
  while (root && root->parent) {
    root = root->parent;
  }
  while (root && !detail::isEmpty(root)) {
    values_.emplace(root, root->begin);
    root = root->children[0];
  }
  ++(*this);
}

template < typename K, typename T, typename C >
template < bool IsConst >
auto kizhin::Map< K, T, C >::LmrIterator< IsConst >::operator++() -> LmrIterator&
{
  if (values_.empty()) {
    valuePtr_ = nullptr;
    node_ = nullptr;
    return *this;
  }
  node_ = values_.top().first;
  valuePtr_ = values_.top().second;
  values_.pop();
  Node* next = nullptr;
  if (valuePtr_ == node_->begin && detail::isThree(node_)) {
    values_.emplace(node_, valuePtr_ + 1);
    next = node_->children[1];
  } else {
    next = node_->children[detail::size(node_)];
  }
  while (next && !detail::isEmpty(next)) {
    values_.emplace(next, next->begin);
    next = next->children[0];
  }
  return *this;
}

template < typename K, typename T, typename C >
template < bool IsConst >
class kizhin::Map< K, T, C >::RmlIterator: public HeavyIterator< IsConst >
{
public:
  using pointer = typename HeavyIterator< IsConst >::pointer;
  using reference = typename HeavyIterator< IsConst >::reference;

  RmlIterator() noexcept = default;
  template < bool RhsConst, std::enable_if_t< IsConst && !RhsConst, int > = 0 >
  RmlIterator(const RmlIterator< RhsConst >& rhs):
    HeavyIterator< IsConst >(rhs),
    values_(rhs.values_)
  {}
  template < bool RhsConst, std::enable_if_t< !IsConst || RhsConst, int > = 0 >
  RmlIterator(Iterator< RhsConst > rhs):
    RmlIterator(rhs.node_)
  {}

  RmlIterator& operator++();
  RmlIterator operator++(int)
  {
    RmlIterator result(*this);
    ++(*this);
    return result;
  }

private:
  friend class Map;

  Stack< std::pair< Node*, pointer > > values_{};
  using HeavyIterator< IsConst >::valuePtr_;
  using HeavyIterator< IsConst >::node_;

  RmlIterator(Node*);
};

template < typename K, typename T, typename C >
template < bool IsConst >
kizhin::Map< K, T, C >::RmlIterator< IsConst >::RmlIterator(Node* root):
  HeavyIterator< IsConst >()
{
  while (root && root->parent) {
    root = root->parent;
  }
  while (root && !detail::isEmpty(root)) {
    values_.emplace(root, root->end - 1);
    root = root->children[detail::size(root)];
  }
  ++(*this);
}

template < typename K, typename T, typename C >
template < bool IsConst >
auto kizhin::Map< K, T, C >::RmlIterator< IsConst >::operator++() -> RmlIterator&
{
  if (values_.empty()) {
    valuePtr_ = nullptr;
    node_ = nullptr;
    return *this;
  }
  node_ = values_.top().first;
  valuePtr_ = values_.top().second;
  values_.pop();
  Node* next = nullptr;
  if (valuePtr_ != node_->begin) {
    values_.emplace(node_, valuePtr_ + 1);
    next = node_->children[valuePtr_ == node_->begin + 1];
  } else {
    next = node_->children[0];
  }
  while (next && !detail::isEmpty(next)) {
    values_.emplace(next, next->begin);
    next = next->children[detail::size(next)];
  }
  return *this;
}

template < typename K, typename T, typename C >
template < bool IsConst >
class kizhin::Map< K, T, C >::BfsIterator: public HeavyIterator< IsConst >
{
public:
  using pointer = typename HeavyIterator< IsConst >::pointer;
  using reference = typename HeavyIterator< IsConst >::reference;

  BfsIterator() noexcept = default;
  template < bool RhsConst, std::enable_if_t< IsConst && !RhsConst, int > = 0 >
  BfsIterator(const BfsIterator< RhsConst >& rhs):
    HeavyIterator< IsConst >(rhs),
    nodes_(rhs.nodes_)
  {}
  template < bool RhsConst, std::enable_if_t< !IsConst || RhsConst, int > = 0 >
  BfsIterator(Iterator< RhsConst > rhs):
    BfsIterator(rhs.node_)
  {}

  BfsIterator& operator++();
  BfsIterator operator++(int)
  {
    BfsIterator result(*this);
    ++(*this);
    return result;
  }

private:
  friend class Map;

  Queue< Node* > nodes_{};
  using HeavyIterator< IsConst >::valuePtr_;
  using HeavyIterator< IsConst >::node_;

  BfsIterator(Node*);
};

template < typename K, typename T, typename C >
template < bool IsConst >
kizhin::Map< K, T, C >::BfsIterator< IsConst >::BfsIterator(Node* root):
  HeavyIterator< IsConst >()
{
  while (root && root->parent) {
    root = root->parent;
  }
  if (root) {
    nodes_.push(root);
    ++(*this);
  }
}

template < typename K, typename T, typename C >
template < bool IsConst >
auto kizhin::Map< K, T, C >::BfsIterator< IsConst >::operator++() -> BfsIterator&
{
  if (valuePtr_ && valuePtr_ + 1 != node_->end) {
    ++valuePtr_;
    return *this;
  }
  if (nodes_.empty()) {
    valuePtr_ = nullptr;
    node_ = nullptr;
    return *this;
  }
  node_ = nodes_.front();
  valuePtr_ = node_->begin;
  nodes_.pop();
  for (Node* child: node_->children) {
    if (child && !detail::isEmpty(child)) {
      nodes_.push(child);
    }
  }
  return *this;
}

template < typename K, typename T, typename C >
class kizhin::Map< K, T, C >::value_compare
{
public:
  bool operator()(const_reference lhs, const_reference rhs) const
  {
    return comp_(lhs.first, rhs.first);
  }

private:
  friend class Map;
  key_compare comp_;

  value_compare(key_compare comp):
    comp_(comp)
  {}
};

template < typename K, typename T, typename C >
kizhin::Map< K, T, C >::Map(const Map& rhs):
  Map(rhs.begin(), rhs.end(), rhs.comparator_)
{}

template < typename K, typename T, typename C >
kizhin::Map< K, T, C >::Map(Map&& rhs) noexcept(is_nothrow_move_constructible):
  root_(std::exchange(rhs.root_, nullptr)),
  size_(std::exchange(rhs.size_, 0)),
  comparator_(std::move(rhs.comparator_))
{}

template < typename K, typename T, typename C >
kizhin::Map< K, T, C >::Map(const key_compare& comparator) noexcept(
    is_nothrow_copy_constructible):
  comparator_(comparator)
{}

template < typename K, typename T, typename C >
template < typename InputIt >
kizhin::Map< K, T, C >::Map(const InputIt first, const InputIt last,
    const key_compare& comparator):
  Map(comparator)
{
  insert(first, last);
}

template < typename K, typename T, typename C >
kizhin::Map< K, T, C >::Map(std::initializer_list< value_type > init,
    const key_compare& comparator):
  Map(init.begin(), init.end(), comparator)
{}

template < typename K, typename T, typename C >
kizhin::Map< K, T, C >& kizhin::Map< K, T, C >::operator=(const Map& rhs)
{
  Map(rhs).swap(*this);
  return *this;
}

template < typename K, typename T, typename C >
kizhin::Map< K, T, C >& kizhin::Map< K, T, C >::operator=(Map&& rhs) noexcept(
    is_nothrow_move_assignable)
{
  Map(std::move(rhs)).swap(*this);
  return *this;
}

template < typename K, typename T, typename C >
kizhin::Map< K, T, C >& kizhin::Map< K, T, C >::operator=(
    std::initializer_list< value_type > init)
{
  Map(init, comparator_).swap(*this);
  return *this;
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::iterator kizhin::Map< K, T, C >::begin() noexcept
{
  if (empty()) {
    return end();
  }
  return iterator{ detail::treeMin(root_) };
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::iterator kizhin::Map< K, T, C >::end() noexcept
{
  return iterator{ getEndNode() };
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::const_iterator kizhin::Map< K, T, C >::begin()
    const noexcept
{
  if (empty()) {
    return end();
  }
  return const_iterator{ detail::treeMin(root_) };
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::const_iterator kizhin::Map< K, T, C >::end()
    const noexcept
{
  return const_iterator{ getEndNode() };
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::lmr_iterator kizhin::Map< K, T, C >::lmrBegin()
{
  return lmr_iterator(root_);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::lmr_iterator kizhin::Map< K, T, C >::lmrEnd()
{
  return lmr_iterator(nullptr);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::const_lmr_iterator kizhin::Map< K, T, C >::lmrBegin()
    const
{
  return const_lmr_iterator(root_);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::const_lmr_iterator kizhin::Map< K, T, C >::lmrEnd() const
{
  return const_lmr_iterator(nullptr);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::rml_iterator kizhin::Map< K, T, C >::rmlBegin()
{
  return rml_iterator(root_);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::rml_iterator kizhin::Map< K, T, C >::rmlEnd()
{
  return rml_iterator(nullptr);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::const_rml_iterator kizhin::Map< K, T, C >::rmlBegin()
    const
{
  return const_rml_iterator(root_);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::const_rml_iterator kizhin::Map< K, T, C >::rmlEnd() const
{
  return const_rml_iterator(nullptr);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::bfs_iterator kizhin::Map< K, T, C >::bfsBegin()
{
  return bfs_iterator(root_);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::bfs_iterator kizhin::Map< K, T, C >::bfsEnd()
{
  return bfs_iterator(nullptr);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::const_bfs_iterator kizhin::Map< K, T, C >::bfsBegin()
    const
{
  return const_bfs_iterator(root_);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::const_bfs_iterator kizhin::Map< K, T, C >::bfsEnd() const
{
  return const_bfs_iterator(nullptr);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::size_type kizhin::Map< K, T, C >::size() const noexcept
{
  return size_;
}

template < typename K, typename T, typename C >
bool kizhin::Map< K, T, C >::empty() const noexcept
{
  return root_ == nullptr;
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::mapped_type& kizhin::Map< K, T, C >::operator[](
    const key_type& key)
{
  const auto inserted = emplace(key, mapped_type());
  return inserted.first->second;
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::mapped_type& kizhin::Map< K, T, C >::operator[](
    key_type&& key)
{
  const auto inserted = emplace(std::move(key), mapped_type());
  return inserted.first->second;
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::mapped_type& kizhin::Map< K, T, C >::at(
    const key_type& key)
{
  const Map* constThis = this;
  return const_cast< mapped_type& >(constThis->at(key));
}

template < typename K, typename T, typename C >
const typename kizhin::Map< K, T, C >::mapped_type& kizhin::Map< K, T, C >::at(
    const key_type& key) const
{
  auto it = find(key);
  if (it == end()) {
    throw std::out_of_range("Key not found");
  }
  return it->second;
}

template < typename K, typename T, typename C >
std::pair< typename kizhin::Map< K, T, C >::iterator, bool > kizhin::Map< K, T,
    C >::insert(const_reference value)
{
  return emplace(value);
}

template < typename K, typename T, typename C >
std::pair< typename kizhin::Map< K, T, C >::iterator, bool > kizhin::Map< K, T,
    C >::insert(value_type&& value)
{
  return emplace(std::move(value));
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::iterator kizhin::Map< K, T, C >::insert(
    const_iterator hint, const_reference value)
{
  return emplaceHint(hint, value);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::iterator kizhin::Map< K, T, C >::insert(
    const_iterator hint, value_type&& value)
{
  return emplaceHint(hint, std::move(value));
}

template < typename K, typename T, typename C >
template < typename InputIt >
void kizhin::Map< K, T, C >::insert(InputIt first, const InputIt last)
{
  for (; first != last; ++first) {
    insert(*first);
  }
}

template < typename K, typename T, typename C >
void kizhin::Map< K, T, C >::insert(std::initializer_list< value_type > list)
{
  insert(list.begin(), list.end());
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::iterator kizhin::Map< K, T, C >::erase(
    const_iterator position)
{
  assert(position != end() && "Position for erasing must not be end()");
  const key_type erasingKey = position->first;
  Node* node = position.node_;
  pointer nodePtr = const_cast< pointer >(position.valuePtr_);
  EndNodeGuard guard(this);
  using std::exchange;
  if (!detail::isLeaf(node) && nodePtr == node->begin) {
    Node* left = detail::treeMax(node->children[0]);
    pointer leftPtr = left->end - 1;
    swapVals(left, leftPtr, exchange(node, left), exchange(nodePtr, leftPtr));
  } else if (!detail::isLeaf(node)) {
    Node* right = detail::treeMin(node->children[detail::size(node)]);
    pointer rightPtr = right->begin;
    swapVals(right, rightPtr, exchange(node, right), exchange(nodePtr, rightPtr));
  }
  node = eraseFromNode(node, nodePtr);
  --size_;
  while (node && detail::isEmpty(node)) {
    node = fixUnderflow(node);
  }
  guard.join();
  return upperBound(erasingKey);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::size_type kizhin::Map< K, T, C >::erase(
    const key_type& key)
{
  const_iterator position = find(key);
  if (position == end()) {
    return 0;
  }
  erase(position);
  return 1;
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::iterator kizhin::Map< K, T, C >::erase(
    const_iterator first, const const_iterator last)
{
  if (first == begin() && last == end()) {
    clear();
    return end();
  }
  iterator firstI(first.node_, const_cast< pointer >(first.valuePtr_));
  for (; firstI != last; ++first) {
    firstI = erase(firstI);
  }
  return firstI;
}

template < typename K, typename T, typename C >
void kizhin::Map< K, T, C >::clear() noexcept
{
  if (!empty()) {
    deallocate();
    root_ = nullptr;
    size_ = 0;
  }
}

template < typename K, typename T, typename C >
void kizhin::Map< K, T, C >::swap(Map& rhs) noexcept(is_nothrow_swappable)
{
  using std::swap;
  swap(root_, rhs.root_);
  swap(size_, rhs.size_);
  swap(comparator_, rhs.comparator_);
}

template < typename K, typename T, typename C >
template < typename... Args >
std::pair< typename kizhin::Map< K, T, C >::iterator, bool > kizhin::Map< K, T,
    C >::emplace(Args&&... args)
{
  const_iterator rootPos{ root_ };
  value_type value(std::forward< Args >(args)...);
  const bool result = count(value.first);
  return std::make_pair(emplaceHint(rootPos, std::move(value)), !result);
}

template < typename K, typename T, typename C >
template < typename... Args >
typename kizhin::Map< K, T, C >::iterator kizhin::Map< K, T, C >::emplaceHint(
    const_iterator hint, Args&&... args)
{
  if (empty()) {
    return emplaceToEmpty(std::forward< Args >(args)...);
  }
  EndNodeGuard guard(this);
  value_type value{ std::forward< Args >(args)... };
  Node* target = findTarget(hint.node_, value.first);
  pointer valuePtr = findKey(target, value.first);
  if (valuePtr != target->end) {
    return iterator(target, valuePtr);
  }
  const key_type key = value.first;
  target = emplaceToNode(target, std::move(value));
  ++size_;
  while (detail::size(target) > 2) {
    target = split(target);
  }
  guard.join();
  return find(key);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::key_compare kizhin::Map< K, T, C >::keyComp() const
{
  return comparator_;
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::value_compare kizhin::Map< K, T, C >::valueComp() const
{
  return value_compare(comparator_);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::iterator kizhin::Map< K, T, C >::find(
    const key_type& key)
{
  const Map* constThis = this;
  const_iterator constRes = constThis->find(key);
  return iterator(constRes.node_, const_cast< pointer >(constRes.valuePtr_));
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::const_iterator kizhin::Map< K, T, C >::find(
    const key_type& key) const
{
  if (empty()) {
    return end();
  }
  Node* target = findTarget(root_, key);
  pointer valuePtr = findKey(target, key);
  return valuePtr == target->end ? end() : const_iterator(target, valuePtr);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::size_type kizhin::Map< K, T, C >::count(
    const key_type& key) const
{
  return find(key) == end() ? 0 : 1;
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::iterator kizhin::Map< K, T, C >::lowerBound(
    const key_type& key)
{
  const Map* constThis = this;
  const auto res = constThis->lowerBound(key);
  return iterator{ res.node_, const_cast< pointer >(res.valuePtr_) };
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::const_iterator kizhin::Map< K, T, C >::lowerBound(
    const key_type& key) const
{
  auto res = begin();
  while (res != end() && keyComp()(res->first, key)) {
    ++res;
  };
  return res;
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::iterator kizhin::Map< K, T, C >::upperBound(
    const key_type& key)
{
  const Map* constThis = this;
  const auto res = constThis->upperBound(key);
  return iterator{ res.node_, const_cast< pointer >(res.valuePtr_) };
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::const_iterator kizhin::Map< K, T, C >::upperBound(
    const key_type& key) const
{
  auto res = begin();
  while (res != end() && !keyComp()(key, res->first)) {
    ++res;
  };
  return res;
}

template < typename K, typename T, typename C >
auto kizhin::Map< K, T, C >::equalRange(const key_type& key)
    -> std::pair< iterator, iterator >
{
  return std::make_pair(lowerBound(key), upperBound(key));
}

template < typename K, typename T, typename C >
auto kizhin::Map< K, T, C >::equalRange(const key_type& key) const
    -> std::pair< const_iterator, const_iterator >
{
  return std::make_pair(lowerBound(key), upperBound(key));
}

template < typename K, typename T, typename C >
template < typename F >
F kizhin::Map< K, T, C >::traverseLmr(F func) const
{
  return std::for_each(lmrBegin(), lmrEnd(), func);
}

template < typename K, typename T, typename C >
template < typename F >
F kizhin::Map< K, T, C >::traverseRml(F func) const
{
  return std::for_each(rmlBegin(), rmlEnd(), func);
}

template < typename K, typename T, typename C >
template < typename F >
F kizhin::Map< K, T, C >::traverseBreadth(F func) const
{
  return std::for_each(bfsBegin(), bfsEnd(), func);
}

template < typename K, typename T, typename C >
class kizhin::Map< K, T, C >::EndNodeGuard
{
public:
  EndNodeGuard(const EndNodeGuard&) = delete;
  EndNodeGuard(Map* owner) noexcept:
    owner_(owner),
    endNode_(owner->getEndNode())
  {
    endNode_->parent->children.fill(nullptr);
  }
  EndNodeGuard& operator=(const EndNodeGuard&) = delete;

  ~EndNodeGuard()
  {
    if (!isJoined_) {
      join();
    }
  }

  void join() noexcept
  {
    assert(!isJoined_ && "EndNode has already joined");
    isJoined_ = true;
    if (owner_->empty()) {
      delete endNode_;
      return;
    }
    Node* max = detail::treeMax(owner_->root_);
    endNode_->parent = max;
    max->children.fill(endNode_);
  }

private:
  Map* owner_ = nullptr;
  Node* endNode_ = nullptr;
  bool isJoined_ = false;
};

template < typename K, typename T, typename C >
void kizhin::Map< K, T, C >::deallocate() noexcept
{
  assert(!empty() && "Attempt to deallocate empty tree");
  Node* endNode = getEndNode();
  endNode->parent->children.fill(nullptr);
  delete endNode;
  Node* left = root_;
  while (root_->children[0]) {
    left = detail::treeMin(left);
    const auto& rtChildren = root_->children;
    std::copy(rtChildren.begin() + 1, rtChildren.end(), left->children.begin());
    delete std::exchange(root_, root_->children[0]);
  }
  delete root_;
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::Node* kizhin::Map< K, T, C >::getEndNode() const noexcept
{
  if (empty()) {
    return nullptr;
  }
  Node* max = detail::treeMax(root_);
  return detail::isEmpty(max) ? max : max->children[0];
}

template < typename K, typename T, typename C >
template < typename... Args >
typename kizhin::Map< K, T, C >::Node* kizhin::Map< K, T, C >::emplaceToNode(Node* node,
    Args&&... args)
{
  assert(node && "emplaceToNode: nullptr given");
  if (detail::isEmpty(node)) {
    detail::emplaceBack(node, std::forward< Args >(args)...);
    return node;
  }
  std::unique_ptr< Node > result = std::make_unique< Node >();
  detail::emplace(node, result.get(), valueComp(), std::forward< Args >(args)...);
  detail::relink(node, result.get());
  if (node == root_) {
    root_ = result.get();
  }
  delete node;
  return result.release();
}

template < typename K, typename T, typename C >
template < typename... Args >
typename kizhin::Map< K, T, C >::iterator kizhin::Map< K, T, C >::emplaceToEmpty(
    Args&&... args)
{
  assert(empty() && "emplaceToEmpty called on non empty Map");
  root_ = new Node;
  detail::emplaceBack(root_, std::forward< Args >(args)...);
  Node* endNode = new Node;
  endNode->parent = root_;
  root_->children.fill(endNode);
  ++size_;
  return begin();
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::Node* kizhin::Map< K, T, C >::eraseFromNode(Node* node,
    const_pointer valPtr)
{
  assert(node && valPtr && "eraseFromNode: nullptr given");
  assert(valPtr >= node->begin && valPtr < node->end && "eraseFromNode: invalid valPtr");
  if (valPtr == node->end - 1) {
    detail::popBack(node);
    return node;
  }
  std::unique_ptr< Node > result = std::make_unique< Node >();
  detail::pop(node, result.get(), valPtr);
  detail::relink(node, result.get());
  if (node == root_) {
    root_ = result.get();
  }
  delete node;
  return result.release();
}

template < typename K, typename T, typename C >
void kizhin::Map< K, T, C >::swapVals(Node* lhs, pointer lhsPtr, Node* rhs,
    pointer rhsPtr)
{
  assert(lhs && lhsPtr && rhs && rhsPtr && "SwapVals: nullptr node given");
  value_type temp1(*lhsPtr);
  value_type temp2(*rhsPtr);
  lhsPtr->~value_type();
  rhsPtr->~value_type();
  new (lhsPtr) value_type(std::move(temp2));
  new (rhsPtr) value_type(std::move(temp1));
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::Node* kizhin::Map< K, T, C >::split(Node* node)
{
  assert(node && "Attempt to split nullptr node");
  assert(detail::size(node) > 2 && "Cannot split node with size less than 3");
  assert((!node->parent || detail::size(node->parent) < 3) && "Split: filled parent");

  Node* left = nullptr;
  Node* right = nullptr;
  std::tie(left, right) = splitInTwo(node);
  Node* parent = node->parent;
  if (!parent) {
    parent = new Node;
    parent->children[0] = node;
    root_ = parent;
  }
  parent = emplaceToNode(parent, *(node->begin + 1));
  auto& children = parent->children;
  *std::remove(children.begin(), children.end(), node) = nullptr;
  delete node;
  auto it = std::find(children.begin(), children.end(), nullptr);
  *(it++) = detail::updateParent(left);
  *(it++) = detail::updateParent(right);
  struct NodeComparator
  {
    bool operator()(const Node* lhs, const Node* rhs) const
    {
      return comp(*(lhs->begin), *(rhs->begin));
    }
    value_compare comp;
  };
  std::sort(children.begin(), it, NodeComparator{ valueComp() });
  return detail::updateParent(parent);
}

template < typename K, typename T, typename C >
std::tuple< typename kizhin::Map< K, T, C >::Node*,
    typename kizhin::Map< K, T, C >::Node* >
kizhin::Map< K, T, C >::splitInTwo(const Node* node)
{
  assert(node && "splitInTwo: nullptr node given");
  assert(detail::size(node) == 3 && "splitInTwo: node must be filled");
  std::unique_ptr< Node > left = std::make_unique< Node >();
  std::unique_ptr< Node > right = std::make_unique< Node >();
  emplaceToNode(left.get(), *node->begin);
  emplaceToNode(right.get(), *(node->end - 1));
  splitChildren(node, left.get(), right.get());
  return std::make_tuple(left.release(), right.release());
}

template < typename K, typename T, typename C >
void kizhin::Map< K, T, C >::splitChildren(const Node* src, Node* left,
    Node* right) const noexcept
{
  assert(src && left && right && "SplitChildren: nullptr node given");
  auto mid = src->children.begin() + (src->children.size() / 2);
  std::copy(src->children.begin(), mid, left->children.begin());
  std::copy(mid, src->children.end(), right->children.begin());
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::pointer kizhin::Map< K, T, C >::findKey(const Node* node,
    const key_type& key) const
{
  assert(node && "FindKey: nullptr node given");
  struct KeyEqual
  {
    bool operator()(const_reference val) const
    {
      return !comp(val.first, key) && !comp(key, val.first);
    }
    C comp;
    const key_type& key;
  };
  return std::find_if(node->begin, node->end, KeyEqual{ keyComp(), key });
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::Node* kizhin::Map< K, T, C >::findTarget(Node* hint,
    const key_type& key) const
{
  assert(!empty() && "Attempt to find target node in empty tree");
  Node* current = validateHint(hint, key);
  while (!detail::isLeaf(current) && findKey(current, key) == current->end) {
    const size_t currSize = detail::size(current);
    if (comparator_(key, current->begin->first)) {
      current = current->children[0];
    } else if (currSize == 1 || comparator_(key, (current->begin + 1)->first)) {
      current = current->children[1];
    } else {
      current = current->children[2];
    }
  }
  return current;
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::Node* kizhin::Map< K, T, C >::validateHint(Node* hint,
    const key_type& key) const
{
  if (!hint || detail::isEmpty(hint) || hint == root_) {
    return root_;
  }
  bool isValid = true;
  bool isMid = false;
  const Node* current = hint;
  while (current->parent && isValid && !isMid) {
    const key_type& parFirst = current->parent->begin->first;
    const key_type& parLast = (current->parent->end - 1)->first;
    isMid = detail::isMiddle(current);
    isValid = isMid && !(keyComp()(key, parFirst) || keyComp()(parLast, key));
    isValid = isValid || (detail::isLeft(current) && !keyComp()(parFirst, key));
    isValid = isValid || (detail::isRight(current) && !keyComp()(key, parLast));
    current = current->parent;
  }
  return isValid ? hint : root_;
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::Node* kizhin::Map< K, T, C >::fixUnderflow(Node* node)
{
  assert(node && "fixUnderflow called with nullptr");
  assert(detail::isEmpty(node) && "fixUnderflow called on non-empty node");
  if (node == root_) {
    return fixRootUnderflow(node);
  }
  if (isMergeable(node)) {
    return merge(node)->parent;
  }
  return redistribute(node)->parent;
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::Node* kizhin::Map< K, T, C >::fixRootUnderflow(
    Node* root)
{
  assert(root && "fixRootUnderflow: nullptr node given");
  assert(root == root_ && "fixRootUnderflow: non-root node given");
  if (detail::isLeaf(root)) {
    delete std::exchange(root_, nullptr);
    return root_;
  }
  Node* newRoot = root_->children[0];
  newRoot->parent = nullptr;
  delete std::exchange(root_, newRoot);
  return root_;
}

template < typename K, typename T, typename C >
bool kizhin::Map< K, T, C >::isMergeable(const Node* node) const
{
  assert(node && "IsMergeable: nullptr node given");
  assert(node->parent && "IsMergeable: root node given");
  if (detail::isThree(node->parent)) {
    return false;
  }
  const auto getRight = detail::getRightSibling< const Node* >;
  const auto getLeft = detail::getLeftSibling< const Node* >;
  return !detail::isThree(detail::isLeft(node) ? getRight(node) : getLeft(node));
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::Node* kizhin::Map< K, T, C >::merge(Node* node)
{
  assert(node && "Merge: nullptr node given");
  assert(node->parent && detail::isEmpty(node) && "Merge: empty or root node given");
  assert(!detail::isThree(node->parent) && "Merge: invalid node given");
  Node* parent = node->parent;
  using detail::getLeftSibling;
  using detail::getRightSibling;
  Node* sibling = detail::isLeft(node) ? getRightSibling(node) : getLeftSibling(node);
  sibling = emplaceToNode(sibling, std::move_if_noexcept(*parent->begin));
  if (detail::isRight(node)) {
    sibling->children[2] = node->children[0];
  } else {
    sibling->children[2] = std::exchange(sibling->children[1], sibling->children[0]);
    sibling->children[0] = node->children[0];
  }
  *std::remove(parent->children.begin(), parent->children.end(), node) = nullptr;
  delete node;
  detail::clear(parent);
  return detail::updateParent(sibling);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::Node* kizhin::Map< K, T, C >::redistribute(Node* node)
{
  assert(node && "Redistribute: nullptr node given");
  assert(!isMergeable(node) && "Redistribute: merge must be called");
  assert(detail::isEmpty(node) && "Redistribute: non-empty node given");
  assert(node->parent && "Redistribute: root node given");
  const auto& children = node->parent->children;
  const auto end = std::find(children.begin(), children.end(), nullptr);
  if (std::any_of(children.begin(), end, detail::isThree< const Node* >)) {
    return borrowFromSibling(node);
  }
  return giveToSibling(node);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::Node* kizhin::Map< K, T, C >::giveToSibling(Node* node)
{
  assert(node && "GiveToSibling: nullptr node given");
  assert(node->parent && "GiveToSibling: root node given");
  assert(detail::isThree(node->parent) && "GiveToSibling: invalid node given");
  const auto getRight = detail::getRightSibling< Node* >;
  const auto getLeft = detail::getLeftSibling< Node* >;
  Node* parent = node->parent;
  Node* sibling = detail::isLeft(node) ? getRight(node) : getLeft(node);
  pointer parentPtr = detail::isRight(node) ? parent->end - 1 : parent->begin;
  sibling = emplaceToNode(sibling, *parentPtr);
  parent = eraseFromNode(parent, parentPtr);
  if (detail::isLeft(node)) {
    sibling->children[2] = sibling->children[1];
    sibling->children[1] = sibling->children[0];
    sibling->children[0] = node->children[0];
  } else {
    sibling->children[2] = node->children[0];
  }
  *std::remove(parent->children.begin(), parent->children.end(), node) = nullptr;
  delete node;
  return detail::updateParent(sibling);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::Node* kizhin::Map< K, T, C >::borrowFromSibling(
    Node* node)
{
  assert(node && "BorrowFromSibling: nullptr node given");
  assert(node->parent && "BorrowFromSibling: root node given");
  Node* sibling = nullptr;
  const bool isMid = detail::isMiddle(node);
  const bool fromLeftMid = isMid && detail::isThree(detail::getRightSibling(node));
  const bool fromLeft = detail::isLeft(node) || fromLeftMid;
  if (fromLeft) {
    sibling = detail::getRightSibling(node);
    sibling = detail::isThree(sibling) ? sibling : borrowFromSibling(sibling);
    node = borrowFromRight(node, sibling);
    sibling = detail::getRightSibling(node);
    node->children[detail::size(node)] = sibling->children[0];
    sibling->children[0] = sibling->children[1];
    sibling->children[1] = sibling->children[2];
  } else {
    sibling = detail::getLeftSibling(node);
    sibling = detail::isThree(sibling) ? sibling : borrowFromSibling(sibling);
    node = borrowFromLeft(node, sibling);
    sibling = detail::getLeftSibling(node);
    node->children[2] = node->children[1];
    node->children[1] = node->children[0];
    node->children[0] = sibling->children[2];
  }
  sibling->children[2] = nullptr;
  return detail::updateParent(node);
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::Node* kizhin::Map< K, T, C >::borrowFromLeft(Node* node,
    Node* sibling)
{
  assert(node && sibling && "BorrowFromLeft: nullptr given");
  assert(node->parent == sibling->parent && "BorrowFromLeft: siblings expected");
  assert(node != sibling && isThree(sibling) && "BorrowFromLeft: invalid sibling");
  assert(!detail::isLeft(node) && "BorrowFromLeft: left node given");
  Node* parent = node->parent;
  pointer const parentPtr = detail::isRight(node) ? parent->end - 1 : parent->begin;
  node = emplaceToNode(node, *parentPtr);
  parent = eraseFromNode(parent, parentPtr);
  parent = emplaceToNode(parent, *(sibling->end - 1));
  sibling = eraseFromNode(sibling, sibling->end - 1);
  return node;
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::Node* kizhin::Map< K, T, C >::borrowFromRight(Node* node,
    Node* sibling)
{
  assert(node && sibling && "BorrowFromRight: nullptr given");
  assert(node->parent == sibling->parent && "BorrowFromRight: siblings expected");
  assert(node != sibling && isThree(sibling) && "BorrowFromRight: invalid sibling");
  assert(!detail::isRight(node) && "BorrowFromRight: left node given");
  Node* parent = node->parent;
  pointer const parentPtr = detail::isLeft(node) ? parent->begin : parent->end - 1;
  node = emplaceToNode(node, *parentPtr);
  parent = eraseFromNode(parent, parentPtr);
  parent = emplaceToNode(parent, *sibling->begin);
  sibling = eraseFromNode(sibling, sibling->begin);
  return node;
}

template < typename K, typename T, typename C >
bool kizhin::operator==(const Map< K, T, C >& lhs, const Map< K, T, C >& rhs)
{
  const bool sameSize = lhs.size() == rhs.size();
  return sameSize && compare(lhs.begin(), lhs.end(), rhs.begin());
}

template < typename K, typename T, typename C >
bool kizhin::operator!=(const Map< K, T, C >& lhs, const Map< K, T, C >& rhs)
{
  return !(lhs == rhs);
}

template < typename K, typename T, typename C >
bool kizhin::operator<(const Map< K, T, C >& lhs, const Map< K, T, C >& rhs)
{
  return lexicographicalCompare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template < typename K, typename T, typename C >
bool kizhin::operator>(const Map< K, T, C >& lhs, const Map< K, T, C >& rhs)
{
  return rhs < lhs;
}

template < typename K, typename T, typename C >
bool kizhin::operator<=(const Map< K, T, C >& lhs, const Map< K, T, C >& rhs)
{
  return !(rhs < lhs);
}

template < typename K, typename T, typename C >
bool kizhin::operator>=(const Map< K, T, C >& lhs, const Map< K, T, C >& rhs)
{
  return !(lhs < rhs);
}

template < typename K, typename T, typename C >
void kizhin::swap(Map< K, T, C >& lhs, Map< K, T, C >& rhs) noexcept(
    noexcept(lhs.swap(rhs)))
{
  lhs.swap(rhs);
}

#endif

