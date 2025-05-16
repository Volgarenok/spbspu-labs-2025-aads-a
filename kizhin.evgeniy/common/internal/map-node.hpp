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
      ~Node() { destroy(begin, end); }
      Node& operator=(const Node&) = delete;
    };

    template < typename NodePtr >
    std::size_t size(NodePtr) noexcept;
    template < typename NodePtr >
    bool isEmpty(NodePtr) noexcept;

    template < typename NodePtr >
    bool isLeaf(NodePtr) noexcept;
    template < typename NodePtr >
    bool isLeft(NodePtr) noexcept;
    template < typename NodePtr >
    bool isMiddle(NodePtr) noexcept;
    template < typename NodePtr >
    bool isRight(NodePtr) noexcept;

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
  }
}

template < typename NodePtr >
std::size_t kizhin::detail::size(NodePtr node) noexcept
{
  assert(node && "Attempt to check is nullptr node empty");
  return node->end - node->begin;
}

template < typename NodePtr >
bool kizhin::detail::isEmpty(NodePtr node) noexcept
{
  assert(node && "Attempt to check is nullptr node empty");
  return node->end == node->begin;
}

template < typename NodePtr >
bool kizhin::detail::isLeaf(NodePtr node) noexcept
{
  assert(node && "Attempt to check is nullptr node a leaf");
  return node->children[0] == nullptr;
}

template < typename NodePtr >
bool kizhin::detail::isLeft(NodePtr node) noexcept
{
  assert(node && "Attempt to check nullptr");
  assert(node->parent && "Attempt to check node without parent");
  return node == node->parent->children[0];
}

template < typename NodePtr >
bool kizhin::detail::isMiddle(NodePtr node) noexcept
{
  assert(node && "Attempt to check nullptr");
  assert(node->parent && "Attempt to check node without parent");
  const bool isParentThree = size(node->parent) == 2;
  return isParentThree && node == node->parent->children[1];
}

template < typename NodePtr >
bool kizhin::detail::isRight(NodePtr node) noexcept
{
  assert(node && "Attempt to check nullptr");
  assert(node->parent && "Attempt to check node without parent");
  NodePtr parent = node->parent;
  const size_t parentSz = size(parent);
  const bool checkTwo = parentSz == 1 && node == parent->children[1];
  const bool checkThree = parentSz == 2 && node == parent->children[2];
  return checkTwo || checkThree;
}

template < typename NodePtr >
NodePtr kizhin::detail::treeMin(NodePtr root) noexcept
{
  assert(root && "Root cannot be nullptr");
  while (root->children[0]) {
    root = root->children[0];
  }
  return root;
}

template < typename NodePtr >
NodePtr kizhin::detail::treeMax(NodePtr root) noexcept
{
  assert(root && "Root cannot be nullptr");
  while (!isLeaf(root)) {
    root = root->children[size(root)];
  }
  return root;
}

template < typename NodePtr >
NodePtr kizhin::detail::updateParent(NodePtr parent) noexcept
{
  assert(parent && "Attempt to update nullptr parent");
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
  assert(lhs && rhs && "Attempt to relink nullptr nodes");
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
    NodePtr rightSibling = node->children[(valuePtr - node->begin) + 1];
    node = treeMin(rightSibling);
    return std::make_tuple(node, node->begin);
  }
  if (valuePtr + 1 != node->end) {
    return std::make_tuple(node, ++valuePtr);
  }
  while (node->parent) {
    NodePtr parent = node->parent;
    if (isLeft(node) || (size(parent) == 2 && isMiddle(node))) {
      valuePtr = isLeft(node) ? parent->begin : parent->begin + 1;
      node = parent;
      return std::make_tuple(node, valuePtr);
    }
    node = parent;
  }
  return std::make_tuple(nullptr, nullptr);
}

template < typename NodePtr, typename ValPtr >
std::tuple< NodePtr, ValPtr > kizhin::detail::prevIter(NodePtr node, ValPtr valuePtr)
{
  assert(node && valuePtr && "Decrementing empty iterator");
  if (!isLeaf(node)) {
    NodePtr leftSibling = node->children[valuePtr - node->begin];
    node = treeMax(leftSibling);
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
  assert(node && "Attempt to emplace in nullptr node");
  assert(size(node) < maxValues && "Node already filled");
  using value_type = std::remove_reference_t< decltype(*(node->begin)) >;
  new (node->end) value_type(std::forward< Args >(args)...);
  ++node->end;
}

template < typename NodePtr, typename Cmp, typename... Args >
void kizhin::detail::emplace(NodePtr src, NodePtr dest, Cmp cmp, Args&&... args)
{
  assert(src && "Attempt to emplace in nullptr src");
  assert(size(src) < maxValues && "Src already filled");
  assert(isEmpty(dest) && "Dest must be empty node");
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

#endif

