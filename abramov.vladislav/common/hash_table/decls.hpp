#ifndef DECLS_HPP
#define DECLS_HPP

namespace abramov
{
  template < class Key, class Value, class Hash, class Equal >
  struct HashTable;

  template < class Key, class Value, class Hash, class Equal >
  struct HashIterator;

  template < class Key, class Value, class Hash, class Equal >
  struct ConstHashIterator;
}
#endif
