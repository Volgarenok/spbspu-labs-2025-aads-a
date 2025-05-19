#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace aleksandrov
{
  namespace detail
  {
    enum class NodeType
    {
      Empty,
      Double,
      Triple
    };

    template< typename K, typename V >
    struct Node
    {
      using ValueType = std::pair< const K, V >;

      ValueType data[2];
      NodeType type;
      Node* left;
      Node* middle;
      Node* right;
      Node* parent;

      Node();
      explicit Node(const ValueType&);
      explicit Node(const ValueType&, const ValueType&);

      bool isLeaf() const noexcept;
      bool isDouble() const noexcept;
      bool isTriple() const noexcept;
    };

    template< typename K, typename V >
    Node< K, V >::Node():
      data{std::make_pair< K, V >(), std::make_pair< K, V >()},
      type(NodeType::Empty),
      left(nullptr),
      middle(nullptr),
      right(nullptr),
      parent(nullptr)
    {}

    template< typename K, typename V >
    Node< K, V >::Node(const ValueType& pair):
      data{pair, std::make_pair< K, V >()},
      type(NodeType::Empty),
      left(nullptr),
      middle(nullptr),
      right(nullptr),
      parent(nullptr)
    {}

    template< typename K, typename V >
    Node< K, V >::Node(const ValueType& pair1, const ValueType& pair2):
      data{pair1, pair2},
      type(NodeType::Empty),
      left(nullptr),
      middle(nullptr),
      right(nullptr),
      parent(nullptr)
    {}

    template< typename K, typename V >
    bool Node< K, V >::isLeaf() const noexcept
    {
      return !left && !middle && !right;
    }

    template< typename K, typename V >
    bool Node< K, V >::isDouble() const noexcept
    {
      return type == NodeType::Double;
    }

    template< typename K, typename V >
    bool Node< K, V >::isTriple() const noexcept
    {
      return type == NodeType::Triple;
    }
  }
}

#endif

