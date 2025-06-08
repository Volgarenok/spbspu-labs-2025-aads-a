#ifndef CONSTRUCT_DESTRUCT_HPP
#define CONSTRUCT_DESTRUCT_HPP

#include "../declaration.hpp"

#include <utility>

template< class... Types >
template< class T, size_t RealTypeId, class, class, class >
rychkov::Variant< Types... >::Variant(T&& value):
  details::VariantBaseAlias< Types... >(in_place_index_t< RealTypeId >(), std::forward< T >(value))
{}
template< class... Types >
template< class T, class... Args >
rychkov::Variant< Types... >::Variant(in_place_type_t< T >, Args&&... args):
  details::VariantBaseAlias< Types... >(in_place_index_t< find_unique_v< T, Types... > >(),
        std::forward< Args >(args)...)
{}
template< class... Types >
template< size_t N, class... Args >
rychkov::Variant< Types... >::Variant(in_place_index_t< N >, Args&&... args):
  details::VariantBaseAlias< Types... >(in_place_index_t< N >(), std::forward< Args >(args)...)
{}

template< class... Types >
template< class T, size_t RealTypeId, class, class RealType, class >
rychkov::Variant< Types... >& rychkov::Variant< Types... >::operator=(T&& rhs)
    noexcept(std::is_nothrow_constructible< RealType, T >::value
    && std::is_nothrow_assignable< RealType, T >::value)
{
  if (!valueless_by_exception())
  {
    this->destroy();
  }
  if (this->active != RealTypeId)
  {
    emplace< RealTypeId >(std::forward< T >(rhs));
  }
  else
  {
    get< RealTypeId >(static_cast< details::UnionStorageAlias< Types... >& >(*this)) = std::forward< T >(rhs);
  }
  return *this;
}

template< class... Types >
template< class T, class... Args >
T& rychkov::Variant< Types... >::emplace(Args&&... args)
{
  return emplace< find_unique_v< remove_cvref_t< T >, Types... > >(std::forward< Args >(args)...);
}

#endif
