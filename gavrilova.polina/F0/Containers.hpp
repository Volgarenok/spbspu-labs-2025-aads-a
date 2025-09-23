#ifndef CONTAINERS_HPP
#define CONTAINERS_HPP

#include <functional>
#include <list/ConstIteratorFwd.hpp>
#include <list/FwdList.hpp>
#include <list/IteratorFwd.hpp>
#include <tree/ConstIterator.hpp>
#include <tree/Iterator.hpp>
#include <tree/TwoThreeTree.hpp>

namespace gavrilova {

  template < class Key, class Value, class Cmp = std::less< Key > >
  using map = TwoThreeTree< Key, Value, Cmp >;

  template < class Key, class Cmp = std::less< Key > >
  using set = TwoThreeTree< Key, Key, Cmp >;

}

#endif
