#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace aleksandrov
{
  namespace detail
  {
    enum class NodeType;

    template< class K, class V >
    struct Node
    {
      using ValueType = std::pair< K, V >;

      ValueType data[2];
      NodeType type;

      Node* left = nullptr;
      Node* middle = nullptr;
      Node* right = nullptr;
      Node* parent = nullptr;

      Node();
      explicit Node(const ValueType&);
      explicit Node(const ValueType&, const ValueType&);

      bool isLeaf() const noexcept;
      bool isDouble() const noexcept;
      bool isTriple() const noexcept;

      Node* fallLeft() noexcept;
      Node* fallRight() noexcept;
    };
  }
}

enum class aleksandrov::detail::NodeType
{
  Null,
  Double,
  Triple
};

template< class K, class V >
aleksandrov::detail::Node< K, V >::Node():
  data{ValueType(), ValueType()},
  type(NodeType::Null)
{}

template< class K, class V >
aleksandrov::detail::Node< K, V >::Node(const ValueType& p):
  data{p, ValueType()},
  type(NodeType::Double)
{}

template< class K, class V >
aleksandrov::detail::Node< K, V >::Node(const ValueType& p1, const ValueType& p2):
  data{p1, p2},
  type(NodeType::Triple)
{}

template< class K, class V >
bool aleksandrov::detail::Node< K, V >::isLeaf() const noexcept
{
  return !left && !middle && !right;
}

template< class K, class V >
bool aleksandrov::detail::Node< K, V >::isDouble() const noexcept
{
  return type == NodeType::Double;
}

template< class K, class V >
bool aleksandrov::detail::Node< K, V >::isTriple() const noexcept
{
  return type == NodeType::Triple;
}

#endif

