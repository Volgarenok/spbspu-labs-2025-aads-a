#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_MAP_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_MAP_HPP

#include <algorithm>
#include <memory>
#include <tuple>
#include <utility>
#include "internal/map-node.hpp"
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

  public:
    using iterator = Iterator< false >;
    using const_iterator = Iterator< true >;

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

    size_type size() const noexcept;
    bool empty() const noexcept;

    mapped_type& operator[](const key_type&);
    mapped_type& operator[](key_type&&);

    mapped_type& at(const key_type&);
    const mapped_type& at(const key_type&) const;

    std::pair< iterator, bool > insert(const_reference);
    std::pair< iterator, bool > insert(value_type&&);
    std::pair< iterator, bool > insert(const_iterator, const_reference);
    std::pair< iterator, bool > insert(const_iterator, value_type&&);
    template < typename InputIt >
    void insert(InputIt, InputIt);
    void insert(std::initializer_list< value_type >);

    void clear() noexcept;
    void swap(Map&) noexcept(is_nothrow_swappable);

    template < typename... Args >
    std::pair< iterator, bool > emplace(Args&&...);
    template < typename... Args >
    std::pair< iterator, bool > emplaceHint(const_iterator, Args&&...);

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

    Node* split(Node*);
    std::tuple< Node*, Node* > splitInTwo(const Node*) const;
    void splitChildren(const Node* src, Node* left, Node* right) const noexcept;

    pointer findKey(const Node*, const key_type&) const;
    Node* findTarget(Node*, const key_type&) const;
    Node* validateHint(Node*, const key_type&) const;
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
  return valuePtr_;
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
std::pair< typename kizhin::Map< K, T, C >::iterator, bool > kizhin::Map< K, T,
    C >::insert(const_iterator hint, const_reference value)
{
  return emplaceHint(hint, value);
}

template < typename K, typename T, typename C >
std::pair< typename kizhin::Map< K, T, C >::iterator, bool > kizhin::Map< K, T,
    C >::insert(const_iterator hint, value_type&& value)
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
  return emplaceHint(rootPos, std::forward< Args >(args)...);
}

template < typename K, typename T, typename C >
template < typename... Args >
std::pair< typename kizhin::Map< K, T, C >::iterator, bool > kizhin::Map< K, T,
    C >::emplaceHint(const_iterator hint, Args&&... args)
{
  if (empty()) {
    root_ = new Node;
    detail::emplaceBack(root_, std::forward< Args >(args)...);
    Node* endNode = new Node;
    endNode->parent = root_;
    root_->children.fill(endNode);
    ++size_;
    return std::make_pair(begin(), true);
  }
  const EndNodeGuard guard(this);
  value_type value{ std::forward< Args >(args)... };
  Node* target = findTarget(hint.node_, value.first);
  pointer valuePtr = findKey(target, value.first);
  if (valuePtr != target->end) {
    return std::make_pair(iterator(target, valuePtr), false);
  }
  const key_type key = value.first;
  target = emplaceToNode(target, std::move(value));
  ++size_;
  while (detail::size(target) > 2) {
    target = split(target);
  }
  return std::make_pair(find(key), true);
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
    Node* max = detail::treeMax(owner_->root_);
    endNode_->parent = max;
    max->children.fill(endNode_);
  }

private:
  Map* owner_ = nullptr;
  Node* endNode_ = nullptr;
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
  auto& parChildren = parent->children;
  *std::remove(parChildren.begin(), parChildren.end(), node) = nullptr;
  delete node;

  auto it = std::remove(parChildren.begin(), parChildren.end(), nullptr);
  *(it++) = detail::updateParent(left);
  *(it++) = detail::updateParent(right);
  const auto pred = [this](const Node* lhs, const Node* rhs) -> bool
  {
    return valueComp()(*(lhs->begin), *(rhs->begin));
  }; /* TODO: Remove lambdas */
  std::sort(parChildren.begin(), it, pred);
  return detail::updateParent(parent);
}

template < typename K, typename T, typename C >
std::tuple< typename kizhin::Map< K, T, C >::Node*,
    typename kizhin::Map< K, T, C >::Node* >
kizhin::Map< K, T, C >::splitInTwo(const Node* node) const
{
  assert(node && "splitInTwo: nullptr node given");
  assert(detail::size(node) == 3 && "splitInTwo: node must be filled");
  std::unique_ptr< Node > left = std::make_unique< Node >();
  std::unique_ptr< Node > right = std::make_unique< Node >();

  detail::emplaceBack(left.get(), *node->begin);
  detail::emplaceBack(right.get(), *(node->end - 1));
  splitChildren(node, left.get(), right.get());

  const auto result = std::make_tuple(left.get(), right.get());
  left.release();
  right.release();
  return result;
}

template < typename K, typename T, typename C >
void kizhin::Map< K, T, C >::splitChildren(const Node* src, Node* left,
    Node* right) const noexcept
{
  assert(src && left && right && "nullptr given to splitChildren");
  left->children[0] = src->children[0];
  left->children[1] = src->children[1];
  right->children[0] = src->children[2];
  right->children[1] = src->children[3];
}

template < typename K, typename T, typename C >
typename kizhin::Map< K, T, C >::pointer kizhin::Map< K, T, C >::findKey(const Node* node,
    const key_type& key) const
{
  assert(node && "Attempt to check is key in nullptr node");
  const auto pred = [this, &key](const_reference val) -> bool
  {
    return !keyComp()(val.first, key) && !keyComp()(key, val.first);
  }; /* TODO: Remove lambdas */
  return std::find_if(node->begin, node->end, pred);
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
  const Node* current = hint;
  while (current->parent && isValid) {
    const key_type& parFirst = current->parent->begin->first;
    const key_type& parLast = (current->parent->end - 1)->first;
    const bool isMid = detail::isMiddle(current);
    if (isMid && (keyComp()(key, parFirst) || keyComp()(parLast, key))) {
      isValid = false;
    } else if (detail::isLeft(current) && keyComp()(parFirst, key)) {
      isValid = false;
    } else if (detail::isRight(current) && keyComp()(key, parLast)) {
      isValid = false;
    }
    current = current->parent;
  }
  return isValid ? hint : root_;
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

