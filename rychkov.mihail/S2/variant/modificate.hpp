#ifndef MODIFICATE_HPP
#define MODIFICATE_HPP

#include "declaration.hpp"

template< class... Types >
template< class T, class... Args >
T& rychkov::Variant< Types... >::emplace(Args&&... args)
{
  emplace< find_uniq_type_in_pack< remove_cvref_t< T >, Types... >() >(std::forward< Args >(args)...);
  return *(this->template get< T >());
}
template< class... Types >
template< size_t N, class... Args >
rychkov::variant_alternative_t< N, Types... >& rychkov::Variant< Types... >::emplace(Args&&... args)
{
  using real_type = variant_alternative_t< N, Types... >;
  if (!this->valueless())
  {
    this->destroy();
  }
  new (this->storage) real_type(std::forward< Args >(args)...);
  this->active = N;
  return *(this->template get< real_type >());
}

#endif
