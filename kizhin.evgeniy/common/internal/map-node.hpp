#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_INTERNAL_MAP_NODE_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_INTERNAL_MAP_NODE_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include "algorithm-utils.hpp"

namespace kizhin {
  namespace detail {
    constexpr std::size_t maxValues = 3;
    constexpr std::size_t maxChildren = 4;

    template < typename NodePtr >
    void clear(NodePtr) noexcept;

    template < typename T >
    struct Node final
    {
      alignas(T) char buffer[maxValues * sizeof(T)];
      T* begin = reinterpret_cast< T* >(buffer);
      T* end = reinterpret_cast< T* >(buffer);
      std::array< Node*, maxChildren > children{};
      Node* parent = nullptr;

      Node() = default;
      Node(const Node&) = delete;
      ~Node() { clear(this); }
      Node& operator=(const Node&) = delete;
    };

    template < typename NodePtr >
    std::size_t size(NodePtr) noexcept;
    template < typename NodePtr >
    bool isEmpty(NodePtr) noexcept;
    template < typename NodePtr >
    bool isThree(NodePtr) noexcept;

    template < typename NodePtr >
    bool isLeaf(NodePtr) noexcept;
    template < typename NodePtr >
    bool isLeft(NodePtr) noexcept;
    template < typename NodePtr >
    bool isMiddle(NodePtr) noexcept;
    template < typename NodePtr >
    bool isRight(NodePtr) noexcept;

    template < typename NodePtr >
    NodePtr getRightSibling(NodePtr) noexcept;
    template < typename NodePtr >
    NodePtr getLeftSibling(NodePtr) noexcept;

    template < typename NodePtr >
    NodePtr treeMin(NodePtr) noexcept;
    template < typename NodePtr >
    NodePtr treeMax(NodePtr) noexcept;

    template < typename NodePtr >
    NodePtr updateParent(NodePtr) noexcept;
    template < typename NodePtr >
    void relink(NodePtr, NodePtr) noexcept;

    template < typename NodePtr, typename ValPtr >
    std::tuple< NodePtr, ValPtr > nextIter(NodePtr, ValPtr);
    template < typename NodePtr, typename ValPtr >
    std::tuple< NodePtr, ValPtr > prevIter(NodePtr, ValPtr);

    template < typename NodePtr, typename... Args >
    void emplaceBack(NodePtr, Args&&...);
    template < typename NodePtr, typename Cmp, typename... Args >
    void emplace(NodePtr src, NodePtr dest, Cmp, Args&&...);

    template < typename NodePtr >
    void popBack(NodePtr) noexcept;
    template < typename NodePtr, typename ValPtr >
    void pop(NodePtr src, NodePtr dest, ValPtr);
  }
}

template < typename NodePtr >
void kizhin::detail::clear(NodePtr node) noexcept
{
  assert(node && "Clear: nullptr node given");
  kizhin::destroy(node->begin, node->end);
  node->end = node->begin;
}

template < typename NodePtr >
std::size_t kizhin::detail::size(NodePtr node) noexcept
{
  assert(node && "Size: nullptr node given");
  return node->end - node->begin;
}

template < typename NodePtr >
bool kizhin::detail::isEmpty(NodePtr node) noexcept
{
  assert(node && "IsEmpty: nullptr node given");
  return node->end == node->begin;
}

template < typename NodePtr >
bool kizhin::detail::isThree(NodePtr node) noexcept
{
  assert(node && "IsThree: nullptr node given");
  return size(node) == 2;
}

template < typename NodePtr >
bool kizhin::detail::isLeaf(NodePtr node) noexcept
{
  assert(node && "IsLeaf: nullptr node given");
  return !node->children[0] || isEmpty(node->children[0]);
}

template < typename NodePtr >
bool kizhin::detail::isLeft(NodePtr node) noexcept
{
  assert(node && "IsLeft: nullptr node given");
  assert(node->parent && "IsLeft: root node given");
  return node == node->parent->children[0];
}

template < typename NodePtr >
bool kizhin::detail::isMiddle(NodePtr node) noexcept
{
  assert(node && "IsMiddle: nullptr node given");
  assert(node->parent && "IsMiddle: root node given");
  NodePtr parent = node->parent;
  return isThree(parent) && node == parent->children[1];
}

template < typename NodePtr >
bool kizhin::detail::isRight(NodePtr node) noexcept
{
  assert(node && "IsRight: nullptr node given");
  assert(node->parent && "IsRight: root node given");
  NodePtr parent = node->parent;
  return node == parent->children[size(parent)];
}

template < typename NodePtr >
NodePtr kizhin::detail::getRightSibling(NodePtr node) noexcept
{
  assert(node && "GetSibling: nullptr node given");
  assert(node->parent && "GetSibling: root node given");
  assert(!isRight(node) && "GetRightSibling: right node given");
  const NodePtr parent = node->parent;
  if (isLeft(node)) {
    return parent->children[1];
  }
  return parent->children[2];
}

template < typename NodePtr >
NodePtr kizhin::detail::getLeftSibling(NodePtr node) noexcept
{
  assert(node && "GetSibling: nullptr node given");
  assert(node->parent && "GetSibling: root node given");
  assert(!isLeft(node) && "GetLeftSibling: left node given");
  const NodePtr parent = node->parent;
  if (isRight(node)) {
    return parent->children[size(parent) - 1];
  }
  return parent->children[0];
}

template < typename NodePtr >
NodePtr kizhin::detail::treeMin(NodePtr root) noexcept
{
  assert(root && "TreeMin: nullptr node given");
  while (!isLeaf(root)) {
    root = root->children[0];
  }
  return root;
}

template < typename NodePtr >
NodePtr kizhin::detail::treeMax(NodePtr root) noexcept
{
  assert(root && "TreeMax: nullptr node given");
  while (!isLeaf(root)) {
    root = root->children[size(root)];
  }
  return root;
}

template < typename NodePtr >
NodePtr kizhin::detail::updateParent(NodePtr parent) noexcept
{
  assert(parent && "UpdateParent: nullptr node given");
  for (auto child: parent->children) {
    if (child) {
      child->parent = parent;
    }
  }
  return parent;
}

template < typename NodePtr >
void kizhin::detail::relink(NodePtr lhs, NodePtr rhs) noexcept
{
  assert(lhs && rhs && "Relink: nullptr node given");
  if (lhs->parent) {
    auto& children = lhs->parent->children;
    auto pos = std::find(children.begin(), children.end(), lhs);
    assert(pos != children.end() && *pos == lhs);
    *pos = rhs;
  }
  if (rhs->parent) {
    auto& children = rhs->parent->children;
    auto pos = std::find(children.begin(), children.end(), lhs);
    assert(pos != children.end() && *pos == rhs);
    *pos = lhs;
  }
  std::swap(lhs->parent, rhs->parent);
  std::swap(lhs->children, rhs->children);
  updateParent(lhs);
  updateParent(rhs);
}

template < typename NodePtr, typename ValPtr >
std::tuple< NodePtr, ValPtr > kizhin::detail::nextIter(NodePtr node, ValPtr valuePtr)
{
  assert(node && valuePtr && "Incrementing empty iterator");
  if (!isLeaf(node)) {
    node = treeMin(node->children[(valuePtr - node->begin) + 1]);
    return std::make_tuple(node, node->begin);
  }
  if (valuePtr + 1 != node->end) {
    return std::make_tuple(node, ++valuePtr);
  }
  NodePtr firstChild = node->children[1];
  if (firstChild && isEmpty(firstChild)) {
    return std::make_tuple(firstChild, firstChild->begin);
  }
  while (node->parent) {
    NodePtr parent = node->parent;
    if (isLeft(node) || (isThree(parent) && isMiddle(node))) {
      valuePtr = isLeft(node) ? parent->begin : parent->begin + 1;
      return std::make_tuple(parent, valuePtr);
    }
    node = parent;
  }
  return std::make_tuple(nullptr, nullptr);
}

template < typename NodePtr, typename ValPtr >
std::tuple< NodePtr, ValPtr > kizhin::detail::prevIter(NodePtr node, ValPtr valuePtr)
{
  assert(node && valuePtr && "Decrementing empty iterator");
  if (isEmpty(node)) {
    return std::make_tuple(node->parent, node->parent->end - 1);
  }
  if (!isLeaf(node)) {
    node = treeMax(node->children[valuePtr - node->begin]);
    return std::make_tuple(node, node->end - 1);
  }
  if (valuePtr != node->begin) {
    return std::make_tuple(node, --valuePtr);
  }
  while (node->parent) {
    NodePtr parent = node->parent;
    if (!isLeft(node)) {
      const bool useBegin = !(size(parent) == 1 || isMiddle(node));
      return std::make_tuple(parent, parent->begin + useBegin);
    }
    node = parent;
  }
  return std::make_tuple(nullptr, nullptr);
}

template < typename NodePtr, typename... Args >
void kizhin::detail::emplaceBack(NodePtr node, Args&&... args)
{
  assert(node && "EmplaceBack: nullptr node given");
  assert(size(node) < maxValues && "EmplaceBack: filled node given");
  using value_type = std::remove_reference_t< decltype(*(node->begin)) >;
  new (node->end) value_type(std::forward< Args >(args)...);
  ++node->end;
}

template < typename NodePtr, typename Cmp, typename... Args >
void kizhin::detail::emplace(NodePtr src, NodePtr dest, Cmp cmp, Args&&... args)
{
  assert(src && dest && "Emplace: nullptr node given");
  assert(size(src) < maxValues && "Emplace: filled node given");
  assert(isEmpty(dest) && "Emplace: non-empty node given");
  using value_type = std::remove_reference_t< decltype(*(src->begin)) >;
  value_type val(std::forward< Args >(args)...);
  auto i = src->begin;
  while (i != src->end && cmp(*i, val)) {
    emplaceBack(dest, std::move_if_noexcept(*(i++)));
  }
  emplaceBack(dest, std::move(val));
  while (i != src->end) {
    emplaceBack(dest, std::move_if_noexcept(*(i++)));
  }
}

template < typename NodePtr >
void kizhin::detail::popBack(NodePtr node) noexcept
{
  assert(node && "PopBack: nullptr node given");
  assert(!isEmpty(node) && "PopBack: non-empty node given");
  using value_type = std::remove_reference_t< decltype(*(node->begin)) >;
  (node->end - 1)->~value_type();
  --node->end;
}

template < typename NodePtr, typename ValPtr >
void kizhin::detail::pop(NodePtr src, NodePtr dest, ValPtr val)
{
  assert(src && dest && "Pop: nullptr node given");
  assert(isEmpty(dest) && "Pop: non-empty node given");
  assert(val >= src->begin && val < src->end && "Pop: invalid val given");
  auto i = src->begin;
  while (i != val) {
    emplaceBack(dest, std::move_if_noexcept(*(i++)));
  }
  ++i;
  while (i != src->end) {
    emplaceBack(dest, std::move_if_noexcept(*(i++)));
  }
}

#endif

