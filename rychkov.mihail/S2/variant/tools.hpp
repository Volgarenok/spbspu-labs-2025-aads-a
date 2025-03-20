#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <type_traits.hpp>

namespace rychkov
{
  using byte = unsigned char;

  template< size_t N, class T, class... Types >
  struct variant_alternative
  {
    using type = typename variant_alternative< N - 1, Types... >::type;
  };
  template< class T, class... Types >
  struct variant_alternative< 0, T, Types... >
  {
    using type = T;
  };
  template< size_t N, class... Types >
  using variant_alternative_t = typename variant_alternative< N, Types... >::type;

  template< class... Types >
  constexpr size_t variant_size_v = sizeof...(Types);

  namespace details
  {
    template< class T, class U, class... Types >
    struct find_unique
    {
      static constexpr bool is_same = std::is_same< T, U >::value;
      static constexpr size_t value = is_same ? 0 : 1 + find_unique< T, Types... >::value;
      static_assert(!is_same || (find_unique< T, Types... >::value == sizeof...(Types)));
    };
    template< class T, class U >
    struct find_unique< T, U >
    {
      static constexpr size_t value = !std::is_same< T, U >::value;
    };
  }
  template< class U, class... Types >
  constexpr size_t find_uniq_type_in_pack()
  {
    constexpr size_t result = details::find_unique< U, Types... >::value;
    static_assert(result != sizeof...(Types));
    return result;
  }
  template< class T, class... Types >
  constexpr bool exactly_once = find_uniq_type_in_pack< T, Types... >() < sizeof...(Types);

  template< class T >
  struct in_place_type_t
  {};
  template< size_t N >
  struct in_place_index_t
  {};
  constexpr size_t variant_npos = static_cast< size_t >(-1);

  namespace details
  {
    template< class T >
    void destructor(byte* lhs) noexcept
    {
      reinterpret_cast< T* >(lhs)->~T();
    }
    template< bool Nothrow, class T >
    void copy_ctor(byte* lhs, const byte* rhs) noexcept(Nothrow)
    {
      new(lhs) T(*reinterpret_cast< const T* >(rhs));
    }
    template< bool Nothrow, class T >
    void move_ctor(byte* lhs, byte* rhs) noexcept(Nothrow)
    {
      new(lhs) T(std::move(*reinterpret_cast< T* >(rhs)));
    }
    template< bool Nothrow, class T >
    void copy_assign(byte* lhs, const byte* rhs) noexcept(Nothrow)
    {
      *reinterpret_cast< T* >(lhs) = *reinterpret_cast< const T* >(rhs);
    }
    template< bool Nothrow, class T >
    void move_assign(byte* lhs, byte* rhs) noexcept(Nothrow)
    {
      *reinterpret_cast< T* >(lhs) = std::move(*reinterpret_cast< T* >(rhs));
    }

    template< class T >
    struct ArrConvertHelper
    {
      T value[1];
    };
    template< class Dest, class Src, class = void >
    struct is_inarray_constructible: std::false_type
    {};
    template< class Dest, class Src >
    struct is_inarray_constructible< Dest, Src,
          void_t< decltype(ArrConvertHelper< Dest >{{std::declval< Src >()}}) > >: std::true_type
    {};
    template< class Src, class Dest >
    constexpr bool is_inarray_constructible_v = is_inarray_constructible< Dest, Src >::value;

    template< size_t N, class Dest, class Src, class = void >
    struct overload
    {
      void resolve() = delete;
    };
    template< size_t N, class Dest, class Src >
    struct overload< N, Dest, Src, std::enable_if_t< is_inarray_constructible< Dest, Src >::value > >
    {
      static std::integral_constant< size_t, N > resolve(Dest);
    };
    template< class T, class U, class... Types >
    struct resolve_overload: overload< 1 + sizeof...(Types), U, T >, resolve_overload< T, Types... >
    {
      using overload< 1 + sizeof...(Types), U, T >::resolve;
      using resolve_overload< T, Types... >::resolve;
    };
    template< class T, class U >
    struct resolve_overload< T, U >: overload< 1, U, T >
    {
      using overload< 1, U, T >::resolve;
    };
    template< class T, class = void, class... Types >
    struct resolve_overloaded_construct: std::integral_constant< size_t, 0 >
    {};
    template< class T, class... Types >
    struct resolve_overloaded_construct< T,
          void_t< decltype(resolve_overload< T, Types... >::resolve(std::declval< T >())) >, Types... >:
      decltype(resolve_overload< T, Types... >::resolve(std::declval< T >()))
    {};
  }
  template< class T, class... Types >
  constexpr size_t resolve_overloaded_construct_v = sizeof...(Types) - details::resolve_overloaded_construct< T, void, Types... >::value;
}

#endif
