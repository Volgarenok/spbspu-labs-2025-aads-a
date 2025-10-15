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

      bool isLeaf() const noexcept;
      bool isEmpty() const noexcept;
      bool isDouble() const noexcept;
      bool isTriple() const noexcept;
    };
  }
}

enum class aleksandrov::detail::NodeType
{
  Double,
  Triple,
  Empty
};

template< class K, class V >
aleksandrov::detail::Node< K, V >::Node():
  data{ ValueType{}, ValueType{} },
  type(NodeType::Empty)
{}

template< class K, class V >
aleksandrov::detail::Node< K, V >::Node(const ValueType& value):
  data{ value, ValueType{} },
  type(NodeType::Double)
{}

template< class K, class V >
bool aleksandrov::detail::Node< K, V >::isLeaf() const noexcept
{
  return !left && !middle && !right;
}

template< class K, class V >
bool aleksandrov::detail::Node< K, V >::isEmpty() const noexcept
{
  return type == NodeType::Empty;
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

