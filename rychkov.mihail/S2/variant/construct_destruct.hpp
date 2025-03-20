#ifndef CONSTRUCT_DESTRUCT_HPP
#define CONSTRUCT_DESTRUCT_HPP

#include "declaration.hpp"

template< class... Types >
template< class T, size_t RealTypeId, class, class RealType, class >
rychkov::Variant< Types... >::Variant(T&& value):
  details::VariantBaseAlias< Types... >(-1)
{
  new (this->storage) RealType(std::forward< T >(value));
  this->active = RealTypeId;
}
template< class... Types >
template< class T, class... Args >
rychkov::Variant< Types... >::Variant(in_place_type_t< T >, Args&&... args):
  details::VariantBaseAlias< Types... >(-1)
{
  new (this->storage) T(std::forward< Args >(args)...);
  this->active = find_uniq_type_in_pack< T, Types... >();
}
template< class... Types >
template< size_t N, class... Args >
rychkov::Variant< Types... >::Variant(in_place_index_t< N >, Args&&... args):
  details::VariantBaseAlias< Types... >(-1)
{
  using real_type = variant_alternative_t< N, Types... >;
  new (this->storage) real_type(std::forward< Args >(args)...);
  this->active = N;
}

template< class... Types >
template< class T, size_t RealTypeId, class, class RealType, class >
rychkov::Variant< Types... >& rychkov::Variant< Types... >::operator=(T&& rhs)
    noexcept(std::is_nothrow_constructible< RealType, T >::value
    && std::is_nothrow_assignable< RealType, T >::value)
{
  if (!this->valueless())
  {
    this->destroy();
  }
  if (this->active != RealTypeId)
  {
    new (this->storage) RealType(std::forward< T >(rhs));
    this->active = RealTypeId;
  }
  else
  {
    *(this->template get< RealType >()) = std::forward< T >(rhs);
  }
  return *this;
}
#endif
