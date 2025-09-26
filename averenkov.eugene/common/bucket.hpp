#ifndef BUCKET_HPP
#define BUCKET_HPP

#include <utility>

namespace averenkov::detail
{
  template< class Key, class Value >
  struct Bucket
  {
    Key key;
    Value value;
    bool occupied = false;
    bool deleted = false;

    Bucket();
  };
}

template< class Key, class Value >
averenkov::detail::Bucket< Key, Value >::Bucket():
  key(Key{}),
  value(Value{}),
  occupied(false),
  deleted(false)
{}


#endif
