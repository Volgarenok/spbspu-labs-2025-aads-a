#ifndef UNION_BASE_HPP
#define UNION_BASE_HPP

#include "../bases.hpp"
#include <utility>
#include <memory>

template< class T, class... Types >
union rychkov::details::UnionBase< true, T, Types... >
{
  T current;
  UnionBase< true, Types... > rest;
  UnionBase():
    rest()
  {}
  template< class... Args >
  constexpr UnionBase(in_place_index_t< 0 >, Args&&... args):
    current{std::forward< Args >(args)...}
  {}
  template< size_t N, class... Args, class = std::enable_if_t< N != 0 > >
  constexpr UnionBase(in_place_index_t< N >, Args&&... args):
    rest(in_place_index_t< N - 1 >(), std::forward< Args >(args)...)
  {}
};
template< class T, class... Types >
union rychkov::details::UnionBase< false, T, Types... >
{
  T current;
  UnionBase< false, Types... > rest;
  UnionBase():
    rest()
  {}
  template< class... Args >
  constexpr UnionBase(in_place_index_t< 0 >, Args&&... args):
    current{std::forward< Args >(args)...}
  {}
  template< size_t N, class... Args, class = std::enable_if_t< N != 0 > >
  constexpr UnionBase(in_place_index_t< N >, Args&&... args):
    rest(in_place_index_t< N - 1 >(), std::forward< Args >(args)...)
  {}
  ~UnionBase()
  {}
};
template< bool IsTrivDestr >
union rychkov::details::UnionBase< IsTrivDestr >
{
  UnionBase() = default;
  template< size_t N, class... Args >
  UnionBase(in_place_index_t< N >, Args&&... args) = delete;
};

template< class... Types >
struct rychkov::details::UnionStorage< false, Types... >
{
  using size_type = rychkov::select_size_type_t< sizeof(UnionBase< false, Types... >) >;
  UnionBase< false, Types... > storage;
  size_type active = static_cast< size_type >(variant_npos);

  UnionStorage() = default;
  template< size_t N, class... Args >
  constexpr UnionStorage(in_place_index_t< N > i, Args&&... args):
    storage(i, std::forward< Args >(args)...),
    active(N)
  {}
  ~UnionStorage()
  {
    destroy();
  }
  void destroy()
  {
    rychkov::visit(Destroyer(), *this);
    active = static_cast< size_type >(variant_npos);
  }

  constexpr size_t index() const noexcept
  {
    return valueless_by_exception() ? variant_npos : active;
  }
  constexpr bool valueless_by_exception() const noexcept
  {
    return active == static_cast< size_type >(variant_npos);
  }
  template< size_t N, class... Args >
  constexpr nth_type_t< N, Types... >& emplace(Args&&... args)
  {
    if (!valueless_by_exception())
    {
      destroy();
    }
    new (std::addressof(get< N >(*this))) nth_type_t< N, Types... >{std::forward< Args >(args)...};
    active = N;
    return get< N >(*this);
  }
};
template< class... Types >
struct rychkov::details::UnionStorage< true, Types... >
{
  using size_type = select_size_type_t< sizeof(UnionBase< true, Types... >),
        unsigned char, unsigned short, size_t >;
  UnionBase< true, Types... > storage;
  size_type active = static_cast< size_type >(variant_npos);

  UnionStorage() = default;
  template< size_t N, class... Args >
  constexpr UnionStorage(in_place_index_t< N > i, Args&&... args):
    storage(i, std::forward< Args >(args)...),
    active(N)
  {}
  void destroy()
  {
    active = static_cast< size_type >(variant_npos);
  }

  constexpr size_t index() const noexcept
  {
    return valueless_by_exception() ? variant_npos : active;
  }
  constexpr bool valueless_by_exception() const noexcept
  {
    return active == static_cast< size_type >(variant_npos);
  }
  template< size_t N, class... Args >
  constexpr nth_type_t< N, Types... >& emplace(Args&&... args)
  {
    if (!valueless_by_exception())
    {
      destroy();
    }
    new (std::addressof(get< N >(*this))) nth_type_t< N, Types... >{std::forward< Args >(args)...};
    active = N;
    return get< N >(*this);
  }
};

template< class... Types >
struct rychkov::variant_size< rychkov::details::UnionStorageAlias< Types... > >:
  std::integral_constant< size_t, sizeof...(Types) >
{};
template< size_t N, class... Types >
struct rychkov::variant_alternative< N, rychkov::details::UnionStorageAlias< Types... > >
{
  using type = nth_type_t< N, Types... >;
};

template< class... Types >
struct rychkov::details::VariantBase< false, Types... >: MoveAssignBaseAlias< Types... >
{
  VariantBase() = delete;
  template< size_t N, class... Args >
  constexpr VariantBase(in_place_index_t< N > i, Args&&... args):
    MoveAssignBaseAlias< Types... >(i, std::forward< Args >(args)...)
  {}
};
template< class... Types >
struct rychkov::details::VariantBase< true, Types... >: MoveAssignBaseAlias< Types... >
{
  static constexpr bool is_nothrow_def_ctor = variant_traits< Types... >::is_nothrow_def_ctor;

  constexpr VariantBase() noexcept(is_nothrow_def_ctor):
    MoveAssignBaseAlias< Types... >(in_place_index_t< 0 >())
  {}
  template< size_t N, class... Args >
  constexpr VariantBase(in_place_index_t< N > i, Args&&... args):
    MoveAssignBaseAlias< Types... >(i, std::forward< Args >(args)...)
  {}
};

#endif
