#include <boost/test/unit_test.hpp>
#include <functional.hpp>
#include <mem_checker.hpp>
#include "variant.hpp"

namespace rychkov
{
  namespace test_visit
  {
    struct TestStruct
    {
      int field = 0;
      template< class T >
      T add(T n, T m) noexcept(noexcept(n + m))
      {
        return n + m;
      }
    };
    static_assert(is_invocable_v< decltype(&TestStruct::add< int >), TestStruct, int, int >, "");
    static_assert(is_invocable_v< decltype(&TestStruct::field), TestStruct >, "");

    template< class T, class... Types >
    struct sum
    {
      using type = decltype(std::declval< T >() + std::declval< typename sum< Types... >::type >());
      type operator()(T num, Types... nums)
      {
        return num + sum< Types... >()(nums...);
      }
    };
    template< class T >
    struct sum< T >
    {
      using type = T;
      type operator()(T num)
      {
        return num;
      }
    };
    struct Adder
    {
      template< class... Types >
      typename sum< Types... >::type operator()(Types... nums)
      {
        return sum< Types... >()(nums...);
      }
    };
    static_assert(is_invocable_v< Adder, int >, "");
    static_assert(std::is_same< invoke_result_t< Adder, int >, int >::value, "");
    static_assert(!is_nothrow_invocable_v< Adder, int >, "");
  }
}

#define USER_DEF_CTOR {}
#define USER_DEF_ASSIGN { return *this; }
#define USER_CHECK(trait, name) std::is_##trait< name >() && !std::is_trivially_##trait< name >()

#define TRIV_DEF_CTOR = default;
#define TRIV_DEF_ASSIGN = default;
#define TRIV_CHECK(trait, name) std::is_trivially_##trait< name >()

#define DEL_DEF_CTOR = delete;
#define DEL_DEF_ASSIGN = delete;
#define DEL_CHECK(trait, name) !std::is_##trait< name >()

#define GENERATE_TRAITS(name, copy_ctor, move_ctor, destructor, copy_assign, move_assign) \
  struct name \
  { \
    name() = default; \
    name(const name&) copy_ctor##_DEF_CTOR \
    name(name&&) move_ctor##_DEF_CTOR \
    ~name() destructor##_DEF_CTOR \
    name& operator=(const name&) copy_assign##_DEF_ASSIGN \
    name& operator=(name&&) move_assign##_DEF_ASSIGN \
  };
#define CHECK_TRAITS(name, copy_ctor, move_ctor, destructor, copy_assign, move_assign) \
  static_assert(copy_ctor##_CHECK(copy_constructible, rychkov::Variant< name >), ""); \
  static_assert(move_ctor##_CHECK(move_constructible, rychkov::Variant< name >), ""); \
  static_assert(destructor##_CHECK(destructible, rychkov::Variant< name >), ""); \
  static_assert(copy_assign##_CHECK(copy_assignable, rychkov::Variant< name >), ""); \
  static_assert(move_assign##_CHECK(move_assignable, rychkov::Variant< name >), "")

namespace rychkov
{
  namespace test_variant_traits
  {
    GENERATE_TRAITS(A1, TRIV, TRIV, TRIV, TRIV, TRIV);
    CHECK_TRAITS(A1, TRIV, TRIV, TRIV, TRIV, TRIV);

    GENERATE_TRAITS(A2, USER, TRIV, TRIV, TRIV, TRIV);
    CHECK_TRAITS(A2, USER, TRIV, TRIV, USER, TRIV);
    GENERATE_TRAITS(A3, TRIV, USER, TRIV, TRIV, TRIV);
    CHECK_TRAITS(A3, TRIV, USER, TRIV, TRIV, USER);
    GENERATE_TRAITS(A4, DEL, TRIV, TRIV, TRIV, TRIV);
    CHECK_TRAITS(A4, DEL, TRIV, TRIV, DEL, TRIV);
    GENERATE_TRAITS(A5, TRIV, DEL, TRIV, TRIV, TRIV);
    CHECK_TRAITS(A5, TRIV, TRIV, TRIV, TRIV, TRIV);

    GENERATE_TRAITS(A6, TRIV, TRIV, TRIV, USER, TRIV);
    CHECK_TRAITS(A6, TRIV, TRIV, TRIV, USER, TRIV);
    GENERATE_TRAITS(A7, TRIV, TRIV, TRIV, TRIV, USER);
    CHECK_TRAITS(A7, TRIV, TRIV, TRIV, TRIV, USER);
    GENERATE_TRAITS(A8, TRIV, TRIV, TRIV, DEL, TRIV);
    CHECK_TRAITS(A8, TRIV, TRIV, TRIV, DEL, TRIV);
    GENERATE_TRAITS(A9, TRIV, TRIV, TRIV, TRIV, DEL);
    CHECK_TRAITS(A9, TRIV, TRIV, TRIV, TRIV, TRIV);

    GENERATE_TRAITS(A10, TRIV, TRIV, USER, TRIV, TRIV);
    CHECK_TRAITS(A10, USER, USER, USER, USER, USER);
  }
}

BOOST_AUTO_TEST_SUITE(S2_variant_test)

BOOST_AUTO_TEST_CASE(visit_test)
{
  rychkov::Variant< int, char > variant1 = 59;
  rychkov::Variant< int, char > variant2 = 934;
  rychkov::test_visit::Adder adder{};
  BOOST_TEST(adder(1, 2, 3, 4) == 10);
  BOOST_TEST(rychkov::visit< int >(adder, variant1, variant2, variant1) == 59 * 2 + 934);
}

BOOST_AUTO_TEST_CASE(destructors_leak_test)
{
  using rychkov::test_variant_traits::A1;
  using A1_tracked = rychkov::MemChecker< A1 >;
  using rychkov::test_variant_traits::A2;
  using A2_tracked = rychkov::MemChecker< A2 >;

  rychkov::MemTrack< A1, A2 > observer{};
  rychkov::Variant< A1_tracked, A2_tracked, char > variant;
  variant.emplace< 1 >();
  variant.emplace< 2 >('E');
  variant.emplace< A2_tracked >();
  variant.emplace< char >();
  variant = 'w';
  variant = A2();
}
BOOST_AUTO_TEST_CASE(holds_test)
{
  rychkov::Variant< char, int > variant;
  BOOST_TEST(rychkov::holds_alternative< char >(variant));
  BOOST_TEST(variant.index() == 0);
  BOOST_TEST(rychkov::get_if< 1 >(&variant) == nullptr);
  BOOST_TEST(rychkov::get_if< 0 >(&variant) != nullptr);
  BOOST_TEST(rychkov::get_if< int >(&variant) == nullptr);
  BOOST_TEST(rychkov::get_if< char >(&variant) != nullptr);
  variant = 43;
  BOOST_TEST(rychkov::holds_alternative< int >(variant));
  BOOST_TEST(variant.index() == 1);
  BOOST_TEST(rychkov::get_if< 1 >(&variant) != nullptr);
  BOOST_TEST(rychkov::get_if< 0 >(&variant) == nullptr);
  BOOST_TEST(rychkov::get_if< int >(&variant) != nullptr);
  BOOST_TEST(rychkov::get_if< char >(&variant) == nullptr);
  BOOST_TEST(rychkov::get< 1 >(variant) == 43);
}
BOOST_AUTO_TEST_CASE(get_test)
{
  rychkov::Variant< char, int > variant{91};
  BOOST_TEST(rychkov::get< int >(variant) == 91);
  BOOST_TEST(rychkov::get< 1 >(variant) == 91);
  BOOST_TEST(rychkov::get< int >(std::move(variant)) == 91);
  BOOST_TEST(rychkov::get< 1 >(std::move(variant)) == 91);
  BOOST_CHECK_THROW(rychkov::get< char >(variant), rychkov::bad_variant_access);
  BOOST_CHECK_THROW(rychkov::get< 0 >(variant), rychkov::bad_variant_access);
  BOOST_CHECK_THROW(rychkov::get< char >(std::move(variant)), rychkov::bad_variant_access);
  BOOST_CHECK_THROW(rychkov::get< 0 >(std::move(variant)), rychkov::bad_variant_access);

  const rychkov::Variant< char, int > variant2{34};
  BOOST_TEST(rychkov::get< int >(variant) == 91);
  BOOST_TEST(rychkov::get< 1 >(variant) == 91);
  BOOST_TEST(rychkov::get< int >(std::move(variant)) == 91);
  BOOST_TEST(rychkov::get< 1 >(std::move(variant)) == 91);
  BOOST_CHECK_THROW(rychkov::get< char >(variant), rychkov::bad_variant_access);
  BOOST_CHECK_THROW(rychkov::get< 0 >(variant), rychkov::bad_variant_access);
  BOOST_CHECK_THROW(rychkov::get< char >(std::move(variant)), rychkov::bad_variant_access);
  BOOST_CHECK_THROW(rychkov::get< 0 >(std::move(variant)), rychkov::bad_variant_access);
}
BOOST_AUTO_TEST_CASE(in_place_test)
{
  rychkov::Variant< int, char, int > variant1{rychkov::in_place_index_t< 0 >(), 55};
  rychkov::Variant< int, char > variant2{rychkov::in_place_type_t< char >(), 'l'};
  BOOST_TEST(rychkov::get< 0 >(variant1) == 55);
  BOOST_TEST(rychkov::get< char >(variant2) == 'l');
}
BOOST_AUTO_TEST_CASE(copy_move_test)
{
  struct A
  {
    int value = 0;
    A(int rhs):
      value(rhs)
    {}
    A(A&& rhs):
      value(std::exchange(rhs.value, 0))
    {}
    A& operator=(A&& rhs)
    {
      value = std::exchange(rhs.value, 0);
      return *this;
    }
  };
  rychkov::Variant< A, int, char > variant1{rychkov::in_place_type_t< A >(), 55};
  rychkov::Variant< A, int, char > variant2 = std::move(variant1);
  BOOST_TEST(rychkov::get< A >(variant1).value == 0);
  BOOST_TEST(rychkov::get< A >(variant2).value == 55);
  variant1 = std::move(variant2);
  BOOST_TEST(rychkov::get< A >(variant1).value == 55);
  BOOST_TEST(rychkov::get< A >(variant2).value == 0);

  rychkov::Variant< int, char > variant3 = 90;
  rychkov::Variant< int, char > variant4 = variant3;
  BOOST_TEST(rychkov::get< int >(variant3) == 90);
  BOOST_TEST(rychkov::get< int >(variant4) == 90);
  variant3 = '!';
  BOOST_TEST(rychkov::get< char >(variant3) == '!');
  variant4 = variant3;
  BOOST_TEST(rychkov::get< char >(variant4) == '!');
}

BOOST_AUTO_TEST_SUITE_END()
