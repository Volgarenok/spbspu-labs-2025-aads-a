#ifndef ACCESS_HPP
#define ACCESS_HPP

#include "../declaration.hpp"

#include <exception>
#include <utility>

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
constexpr std::enable_if_t< rychkov::exactly_once< T, Types... >, T* >
    rychkov::get_if(Variant< Types... >* variant) noexcept
{
  return get_if< find_unique_v< T, Types... > >(variant);
}
template< class T, class... Types >
constexpr std::enable_if_t< rychkov::exactly_once< T, Types... >, const T* >
    rychkov::get_if(const Variant< Types... >* variant) noexcept
{
  return get_if< find_unique_v< T, Types... > >(variant);
}
template< size_t N, class... Types >
rychkov::nth_type_t< N, Types... >* rychkov::get_if(Variant< Types... >* variant) noexcept
{
  if (variant->index() == N)
  {
    return &get< N >(*static_cast< details::UnionStorageAlias< Types... >* >(variant));
  }
  return nullptr;
}
template< size_t N, class... Types >
const rychkov::nth_type_t< N, Types... >* rychkov::get_if(const Variant< Types... >* variant) noexcept
{
  if (variant->index() == N)
  {
    return &get< N >(*static_cast< const details::UnionStorageAlias< Types... >* >(variant));
  }
  return nullptr;
}

template< class T, class... Types >
constexpr std::enable_if_t< rychkov::exactly_once< T, Types... >, T& >
    rychkov::get(Variant< Types... >& variant)
{
  return get< find_unique_v< T, Types... > >(variant);
}
template< class T, class... Types >
constexpr std::enable_if_t< rychkov::exactly_once< T, Types... >, const T& >
    rychkov::get(const Variant< Types... >& variant)
{
  return get< find_unique_v< T, Types... >() >(variant);
}
template< class T, class... Types >
constexpr std::enable_if_t< rychkov::exactly_once< T, Types... >, T&& >
    rychkov::get(Variant< Types... >&& variant)
{
  return std::move(get< T >(variant));
}
template< class T, class... Types >
constexpr std::enable_if_t< rychkov::exactly_once< T, Types... >, const T&& >
    rychkov::get(const Variant< Types... >&& variant)
{
  return std::move(get< T >(variant));
}
template< size_t N, class... Types >
rychkov::nth_type_t< N, Types... >& rychkov::get(Variant< Types... >& variant)
{
  if (variant.index() != N)
  {
    details::throw_bad_variant_access(variant.valueless_by_exception());
  }
  return *get_if< N >(&variant);
}
template< size_t N, class... Types >
const rychkov::nth_type_t< N, Types... >& rychkov::get(const Variant< Types... >& variant)
{
  if (variant.index() != N)
  {
    details::throw_bad_variant_access(variant.valueless_by_exception());
  }
  return *get_if< N >(&variant);
}
template< size_t N, class... Types >
rychkov::nth_type_t< N, Types... >&& rychkov::get(Variant< Types... >&& variant)
{
  return std::move(get< N >(variant));
}
template< size_t N, class... Types >
const rychkov::nth_type_t< N, Types... >&& rychkov::get(const Variant< Types... >&& variant)
{
  return std::move(get< N >(variant));
}

template< class T, class... Types >
bool rychkov::holds_alternative(const Variant< Types... >& variant) noexcept
{
  return !variant.valueless_by_exception()
      && (variant.index() == find_unique_v< T, Types... >);
}

#endif
