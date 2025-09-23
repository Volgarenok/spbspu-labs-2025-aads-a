#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <unordered-map.hpp>

using UnorderedMapT = kizhin::UnorderedMap< int, int >;

void testUnorderedMapInvariants(const UnorderedMapT& map)
{
  const std::size_t count = std::distance(map.begin(), map.end());
  BOOST_TEST(map.size() == count);
  BOOST_TEST(map.empty() == (count == 0));
}

BOOST_AUTO_TEST_SUITE(constructors);

BOOST_AUTO_TEST_CASE(default_constructor)
{
  const UnorderedMapT map;
  testUnorderedMapInvariants(map);
  BOOST_TEST(map.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor_from_empty)
{
  const UnorderedMapT original;
  const UnorderedMapT copy(original);
  testUnorderedMapInvariants(copy);
  BOOST_TEST(copy.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  const UnorderedMapT original = { { 1, 1 }, { 2, 2 } };
  const UnorderedMapT copy(original);
  testUnorderedMapInvariants(copy);
  BOOST_TEST(copy.size() == 2);
  BOOST_TEST(copy.find(1)->second == 1);
  BOOST_TEST(copy.find(2)->second == 2);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  UnorderedMapT original = { { 1, 1 }, { 2, 2 } };
  const UnorderedMapT copy(original);
  const UnorderedMapT moved(std::move(original));
  testUnorderedMapInvariants(moved);
  BOOST_TEST(moved == copy);
}

BOOST_AUTO_TEST_CASE(range_constructor)
{
  const std::initializer_list< UnorderedMapT::value_type > vals = {
    { 1, 1 },
    { 2, 2 },
  };
  const UnorderedMapT map(vals.begin(), vals.end());
  testUnorderedMapInvariants(map);
  BOOST_TEST(map.size() == 2);
  BOOST_TEST(map.find(1)->second == 1);
  BOOST_TEST(map.find(2)->second == 2);
}

BOOST_AUTO_TEST_CASE(initializer_list_constructor)
{
  const std::initializer_list< UnorderedMapT::value_type > vals = {
    { 1, 1 },
    { 2, 2 },
  };
  const UnorderedMapT map(vals);
  testUnorderedMapInvariants(map);
  BOOST_TEST(map.size() == 2);
  BOOST_TEST(map.find(1)->second == 1);
  BOOST_TEST(map.find(2)->second == 2);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(assignment_operators);

BOOST_AUTO_TEST_CASE(copy_assignment_operator)
{
  const UnorderedMapT original = { { 1, 1 } };
  UnorderedMapT copy;
  copy = original;
  testUnorderedMapInvariants(copy);
  BOOST_TEST(copy == original);
}

BOOST_AUTO_TEST_CASE(move_assignment_operator)
{
  UnorderedMapT original = { { 1, 1 } };
  const UnorderedMapT copy(original);
  UnorderedMapT moved;
  moved = std::move(original);
  testUnorderedMapInvariants(moved);
  BOOST_TEST(moved == copy);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(element_access);

BOOST_AUTO_TEST_CASE(square_brackets_existing_key)
{
  UnorderedMapT map = { { 1, 1 } };
  BOOST_TEST(map[1] == 1);
  testUnorderedMapInvariants(map);
}

BOOST_AUTO_TEST_CASE(square_brackets_new_key)
{
  UnorderedMapT map;
  map[1] = 1;
  testUnorderedMapInvariants(map);
  BOOST_TEST(map.size() == 1);
  BOOST_TEST(map.find(1)->second == 1);
}

BOOST_AUTO_TEST_CASE(at_existing_key)
{
  UnorderedMapT map = { { 1, 1 } };
  BOOST_TEST(map.at(1) == 1);
  testUnorderedMapInvariants(map);
}

BOOST_AUTO_TEST_CASE(at_const_existing_key)
{
  const UnorderedMapT map = { { 1, 1 } };
  BOOST_TEST(map.at(1) == 1);
  testUnorderedMapInvariants(map);
}

BOOST_AUTO_TEST_CASE(at_missing_key)
{
  const UnorderedMapT map;
  BOOST_CHECK_THROW(map.at(1), std::out_of_range);
  testUnorderedMapInvariants(map);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(modifiers);

BOOST_AUTO_TEST_CASE(insert_new_key)
{
  UnorderedMapT map;
  const auto result = map.insert({ 1, 1 });
  testUnorderedMapInvariants(map);
  BOOST_TEST(result.second);
  BOOST_TEST((*result.first == UnorderedMapT::value_type(1, 1)));
  BOOST_TEST(map.size() == 1);
}

BOOST_AUTO_TEST_CASE(insert_existing_key)
{
  UnorderedMapT map = { { 1, 1 } };
  auto result = map.insert({ 1, 2 });
  testUnorderedMapInvariants(map);
  BOOST_TEST(!result.second);
  BOOST_TEST((*result.first == UnorderedMapT::value_type(1, 1)));
  BOOST_TEST(map.size() == 1);
}

BOOST_AUTO_TEST_CASE(insert_range)
{
  const std::initializer_list< UnorderedMapT::value_type > vals = {
    { 1, 1 },
    { 2, 2 },
  };
  UnorderedMapT map;
  map.insert(vals.begin(), vals.end());
  testUnorderedMapInvariants(map);
  BOOST_TEST(map.size() == 2);
  BOOST_TEST(map.at(1) == 1);
  BOOST_TEST(map.at(2) == 2);
}

BOOST_AUTO_TEST_CASE(erase_existing_key)
{
  UnorderedMapT map = { { 1, 1 }, { 2, 2 } };
  auto it = map.find(1);
  map.erase(it);
  testUnorderedMapInvariants(map);
  BOOST_TEST(map.size() == 1);
  BOOST_TEST((map.find(1) == map.end()));
}

BOOST_AUTO_TEST_CASE(clear)
{
  UnorderedMapT map = { { 1, 1 } };
  map.clear();
  testUnorderedMapInvariants(map);
  BOOST_TEST(map.empty());
}

BOOST_AUTO_TEST_CASE(swap)
{
  UnorderedMapT map1 = { { 1, 1 } };
  UnorderedMapT map2 = { { 2, 2 } };
  const UnorderedMapT first(map1);
  const UnorderedMapT second(map2);
  map1.swap(map2);
  testUnorderedMapInvariants(map1);
  testUnorderedMapInvariants(map2);
  BOOST_TEST(map2 == first);
  BOOST_TEST(map1 == second);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(lookup);

BOOST_AUTO_TEST_CASE(find_existing_key)
{
  const UnorderedMapT map = { { 1, 1 } };
  auto it = map.find(1);
  BOOST_TEST((it != map.end()));
  BOOST_TEST((*it == UnorderedMapT::value_type(1, 1)));
  testUnorderedMapInvariants(map);
}

BOOST_AUTO_TEST_CASE(find_missing_key)
{
  const UnorderedMapT map;
  auto it = map.find(1);
  BOOST_TEST((it == map.end()));
  testUnorderedMapInvariants(map);
}

BOOST_AUTO_TEST_CASE(count_existing_key)
{
  const UnorderedMapT map = { { 1, 1 } };
  BOOST_TEST(map.count(1) == 1);
  testUnorderedMapInvariants(map);
}

BOOST_AUTO_TEST_CASE(count_missing_key)
{
  const UnorderedMapT map;
  BOOST_TEST(map.count(1) == 0);
  testUnorderedMapInvariants(map);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(observers);

BOOST_AUTO_TEST_CASE(hash_function)
{
  const UnorderedMapT map;
  const auto hashFunc = map.hashFunc();
  BOOST_TEST(hashFunc(123) == std::hash< int >{}(123));
}

BOOST_AUTO_TEST_CASE(key_equal_function)
{
  UnorderedMapT map;
  const auto keyEq = map.keyEq();
  BOOST_TEST(keyEq(1, 1));
  BOOST_TEST(!keyEq(1, 2));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(free_functions);

BOOST_AUTO_TEST_CASE(equal_opeator)
{
  using namespace kizhin;
  const UnorderedMapT map1 = { { 1, 1 } };
  const UnorderedMapT map2 = { { 1, 1 } };
  const UnorderedMapT map3 = { { 2, 2 } };
  BOOST_TEST(map1 == map1);
  BOOST_TEST((map1 == map2 && map2 == map1));
  BOOST_TEST((map1 != map3 && map3 != map1));
  BOOST_TEST((map2 != map3 && map3 != map2));
}

BOOST_AUTO_TEST_CASE(swap_func)
{
  UnorderedMapT map1 = { { 1, 1 } };
  UnorderedMapT map2 = { { 2, 2 } };
  const UnorderedMapT first(map1);
  const UnorderedMapT second(map2);
  swap(map1, map2);
  testUnorderedMapInvariants(map1);
  testUnorderedMapInvariants(map2);
  BOOST_TEST(map2 == first);
  BOOST_TEST(map1 == second);
}

BOOST_AUTO_TEST_SUITE_END();

