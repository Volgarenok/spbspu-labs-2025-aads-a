#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

#include <utility>
#include <type_traits.hpp>

namespace rychkov
{
  template< class C, class T >
  class compare_to_value
  {
  public:
    compare_to_value(C condition, const T& rhs):
      value_(rhs),
      condition_(condition)
    {}
    bool operator()(const T& lhs)
    {
      return condition_(lhs, value_);
    }
  private:
    const T& value_;
    C condition_;
  };

  template< class F, class... Args >
  decltype(std::declval< F >()(std::declval< Args >()...)) invoke(F&& func, Args&&... args)
      noexcept(noexcept(std::declval< F >()(std::declval< Args >()...)))
  {
    return std::forward< F >(func)(std::forward< Args >(args)...);
  }
  template< class Member, class Base, class Obj, class... Args >
  decltype(std::declval< Member >()(std::declval< Args >()...))
      invoke(Member Base::* member, Obj&& obj, Args&&... args)
      noexcept(noexcept(std::declval< Member >()(std::declval< Args >()...)))
  {
    return (std::forward< Obj >(obj).*member)(std::forward< Args >(args)...);
  }
  template< class Member, class Base, class Obj >
  std::enable_if_t< !std::is_function< Member >::value, Member > invoke(Member Base::* member, Obj&& obj)
  {
    return std::forward< Obj >(obj).*member;
  }
  template< class R, class F, class... Args >
  std::enable_if_t< !std::is_void< R >::value, R > invoke_r(F&& func, Args&&... args)
      noexcept(noexcept(R(invoke(std::declval< F >(), std::declval< Args >()...))))
  {
    return invoke(std::forward< F >(func), std::forward< Args >(args)...);
  }
  template< class R, class F, class... Args >
  std::enable_if_t< std::is_void< R >::value, void > invoke_r(F&& func, Args&&... args)
      noexcept(noexcept(rychkov::invoke(std::declval< F >(), std::declval< Args >()...)))
  {
    rychkov::invoke(std::forward< F >(func), std::forward< Args >(args)...);
  }

  namespace details
  {
    template< class AlwaysVoid, class F, class... Args >
    struct invoke_result
    {};
    template< class F, class... Args >
    struct invoke_result< void_t< decltype(invoke(std::declval< F >(), std::declval< Args >()...)) >, F, Args... >
    {
      using type = decltype(invoke(std::declval< F >(), std::declval< Args >()...));
    };

    template< class AlwaysVoid, class F, class... Args >
    struct is_invocable: std::false_type
    {};
    template< class F, class... Args >
    struct is_invocable< void_t< typename invoke_result< void, F, Args... >::type >, F, Args... >:
      std::true_type
    {};

    template< class AlwaysVoid, class F, class... Args >
    struct is_nothrow_invocable: std::false_type
    {};
    template< class F, class... Args >
    struct is_nothrow_invocable< void_t< typename invoke_result< void, F, Args... >::type >, F, Args... >:
      std::integral_constant< bool, noexcept(invoke(std::declval< F >(), std::declval< Args >()...)) >
    {};
  }
  template< class F, class... Args >
  struct invoke_result
  {
    using type = typename details::invoke_result< void, F, Args... >::type;
  };
  template< class F, class... Args >
  struct is_invocable: details::is_invocable< void, F, Args... >
  {};
  template< class F, class... Args >
  struct is_nothrow_invocable: details::is_nothrow_invocable< void, F, Args... >
  {};
  template< class F, class... Args >
  using invoke_result_t = typename invoke_result< F, Args... >::type;
  template< class F, class... Args >
  constexpr bool is_invocable_v = is_invocable< F, Args... >::value;
  template< class F, class... Args >
  constexpr bool is_nothrow_invocable_v = is_nothrow_invocable< F, Args... >::value;
};

#endif
