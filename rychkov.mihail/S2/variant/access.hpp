#ifndef ACCESS_HPP
#define ACCESS_HPP

#include "declaration.hpp"

#include <exception>

class rychkov::bad_variant_access: public std::exception
{
public:
  bad_variant_access() noexcept;
  virtual const char* what() const noexcept override;
private:
  const char* data_;
  bad_variant_access(const char* message) noexcept;
  friend void rychkov::details::throw_bad_variant_access(const char* message);
};

template< class T, class... Types >
T* rychkov::get_if(Variant< Types... >* variant) noexcept
{
  return get_if< find_uniq_type_in_pack< T, Types... >() >(variant);
}
template< class T, class... Types >
const T* rychkov::get_if(const Variant< Types... >* variant) noexcept
{
  return get_if< find_uniq_type_in_pack< T, Types... >() >(variant);
}
template< size_t N, class... Types >
rychkov::variant_alternative_t< N, Types... >* rychkov::get_if(Variant< Types... >* variant) noexcept
{
  if (variant->index() == N)
  {
    return variant->template get< variant_alternative_t< N, Types... > >();
  }
  return nullptr;
}
template< size_t N, class... Types >
const rychkov::variant_alternative_t< N, Types... >* rychkov::get_if(const Variant< Types... >* variant) noexcept
{
  if (variant->index() == N)
  {
    return variant->template get< variant_alternative_t< N, Types... > >();
  }
  return nullptr;
}

template< class T, class... Types >
T& rychkov::get(Variant< Types... >& variant)
{
  return get< find_uniq_type_in_pack< T, Types... >() >(variant);
}
template< class T, class... Types >
const T& rychkov::get(const Variant< Types... >& variant)
{
  return get< find_uniq_type_in_pack< T, Types... >() >(variant);
}
template< class T, class... Types >
T&& rychkov::get(Variant< Types... >&& variant)
{
  return std::move(get< T >(variant));
}
template< class T, class... Types >
const T&& rychkov::get(const Variant< Types... >&& variant)
{
  return std::move(get< T >(variant));
}
template< size_t N, class... Types >
rychkov::variant_alternative_t< N, Types... >& rychkov::get(Variant< Types... >& variant)
{
  rychkov::variant_alternative_t< N, Types... >* resultPtr = get_if< N, Types... >(&variant);
  if (resultPtr == nullptr)
  {
    details::throw_bad_variant_access(variant.valueless_by_exception());
  }
  return *resultPtr;
}
template< size_t N, class... Types >
const rychkov::variant_alternative_t< N, Types... >& rychkov::get(const Variant< Types... >& variant)
{
  rychkov::variant_alternative_t< N, Types... >* resultPtr = get_if< N >(&variant);
  if (resultPtr != nullptr)
  {
    details::throw_bad_variant_access(variant.valueless_by_exception());
  }
  return *resultPtr;
}
template< size_t N, class... Types >
rychkov::variant_alternative_t< N, Types... >&& rychkov::get(Variant< Types... >&& variant)
{
  return std::move(get< N >(variant));
}
template< size_t N, class... Types >
const rychkov::variant_alternative_t< N, Types... >&& rychkov::get(const Variant< Types... >&& variant)
{
  return std::move(get< N >(variant));
}

template< class... Types >
size_t rychkov::Variant< Types... >::index() const noexcept
{
  return this->valueless() ? variant_npos : this->active;
}
template< class... Types >
bool rychkov::Variant< Types... >::valueless_by_exception() const noexcept
{
  return this->valueless();
}

template< class T, class... Types >
bool rychkov::holds_alternative(const Variant< Types... >& variant) noexcept
{
  return variant.index() == find_uniq_type_in_pack< T, Types... >();
}

#endif
