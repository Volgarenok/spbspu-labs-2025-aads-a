#ifndef HASH_WRAPPER_HPP
#define HASH_WRAPPER_HPP
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>

namespace savintsev
{
  template< class T, class H >
  struct Hash
  {
  public:
    size_t operator()(T const & v) const
    {
      H h;
      boost::hash2::hash_append(h, {}, v);
      return boost::hash2::get_integral_result< size_t >(h);
    }
  };
}

#endif
