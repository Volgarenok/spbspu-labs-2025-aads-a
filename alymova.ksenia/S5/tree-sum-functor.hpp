#ifndef TREE_SUM_FUNCTOR_HPP
#define TREE_SUM_FUNCTOR_HPP
#include <iostream>
#include <utility>
#include <queue.hpp>

namespace alymova
{
  struct TreeSum
  {
    int keys;
    Queue< std::string > values;

    TreeSum();
    void operator()(const std::pair< int, std::string >& key_value);
  };
  std::ostream& operator<<(std::ostream& out, const TreeSum& object);
  bool isOverflowAddition(long long int lhs, long long int rhs);
}
#endif
