#include <numeric>
#include <random>
#include <boost/test/unit_test.hpp>
#include "test-utils.hpp"

using kizhin::MapT;
using kizhin::testMapComparison;
using kizhin::testMapInvariants;

BOOST_AUTO_TEST_SUITE(constructors);

BOOST_AUTO_TEST_CASE(default_constructor)
{
  const MapT map;
  testMapInvariants(map);
  BOOST_TEST(map.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor_from_empty)
{
  const MapT map;
  const MapT copied(map);
  testMapInvariants(copied);
  BOOST_TEST(copied.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  const MapT map{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  const MapT copied(map);
  testMapInvariants(copied);
  BOOST_TEST(copied == map);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  const MapT map{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  const MapT copied(map);
  const MapT moved(std::move(map));
  testMapInvariants(moved);
  BOOST_TEST(moved == copied);
}

BOOST_AUTO_TEST_CASE(empty_range_constructor)
{
  const std::initializer_list< MapT::value_type > init{};
  const MapT map(init.begin(), init.end());
  testMapInvariants(map);
  BOOST_TEST(map.empty());
}

BOOST_AUTO_TEST_CASE(range_constructor)
{
  const std::initializer_list< MapT::value_type > init{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  const MapT map(init.begin(), init.end());
  testMapInvariants(map);
  BOOST_TEST(map.size() == init.size());
  BOOST_TEST(std::equal(map.begin(), map.end(), init.begin()));
}

BOOST_AUTO_TEST_CASE(initializer_list_constructor)
{
  const std::initializer_list< MapT::value_type > init{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  const MapT map(init);
  testMapInvariants(map);
  BOOST_TEST(map.size() == init.size());
  BOOST_TEST(std::equal(map.begin(), map.end(), init.begin()));
}
BOOST_AUTO_TEST_SUITE_END();
BOOST_AUTO_TEST_SUITE(assignment_operators);

BOOST_AUTO_TEST_CASE(copy_assignment_operator)
{
  const MapT map{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  MapT copied;
  copied = map;
  testMapInvariants(copied);
  BOOST_TEST(map == copied);
}

BOOST_AUTO_TEST_CASE(move_assignment_operator)
{
  const MapT map{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  const MapT copied(map);
  MapT moved;
  moved = std::move(map);
  testMapInvariants(moved);
  BOOST_TEST(moved == copied);
}

BOOST_AUTO_TEST_CASE(initializer_list_assignment_operator)
{
  const std::initializer_list< MapT::value_type > init{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  const MapT initied(init);
  MapT map;
  map = init;
  testMapInvariants(map);
  BOOST_TEST(map == initied);
}

BOOST_AUTO_TEST_SUITE_END();
BOOST_AUTO_TEST_SUITE(element_access);

BOOST_AUTO_TEST_CASE(square_brackets_access)
{
  MapT map{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  const MapT copied(map);
  const MapT::mapped_type& val1 = map[2];
  const MapT::mapped_type& val2 = map[2];
  testMapInvariants(map);
  BOOST_TEST(map == copied);
  BOOST_TEST(std::addressof(val1) == std::addressof(val2));
}

BOOST_AUTO_TEST_CASE(square_brackets_insertion)
{
  const MapT expected{ { 1, "abc" } };
  MapT map;
  map[1] = "abc";
  testMapInvariants(map);
  BOOST_TEST(map == expected);
}

BOOST_AUTO_TEST_CASE(at_access)
{
  const MapT map{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  const MapT::mapped_type& val1 = map.at(2);
  const MapT::mapped_type& val2 = map.at(2);
  testMapInvariants(map);
  BOOST_TEST(std::addressof(val1) == std::addressof(val2));
}

BOOST_AUTO_TEST_CASE(at_out_of_range)
{
  const MapT map{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  BOOST_CHECK_THROW(map.at(4), std::out_of_range);
  testMapInvariants(map);
}

BOOST_AUTO_TEST_SUITE_END();
BOOST_AUTO_TEST_SUITE(modifiers);

BOOST_AUTO_TEST_CASE(insert)
{
  const MapT expected{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  MapT map{
    { 2, "def" },
    { 3, "ghi" },
  };
  const MapT::value_type value = { 1, "abc" };
  const auto res = map.insert(value);
  testMapInvariants(map);
  BOOST_TEST(map == expected);
  BOOST_TEST((res.first == map.begin()));
  BOOST_TEST(res.second);
}

BOOST_AUTO_TEST_CASE(insert_shuffled_keys)
{
  MapT map;
  std::vector< MapT::key_type > keys(1'500);
  std::iota(keys.begin(), keys.end(), 0);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(keys.begin(), keys.end(), gen);
  for (const auto& key: keys) {
    BOOST_TEST(map.insert(std::make_pair(key, "")).second);
    testMapInvariants(map);
  }
  std::sort(keys.begin(), keys.end(), map.keyComp());
  const auto cmp = [](const auto& lhs, const auto& rhs) -> bool
  {
    return lhs == rhs.first;
  };
  BOOST_TEST(std::equal(keys.begin(), keys.end(), map.begin(), map.end(), cmp));
}

BOOST_AUTO_TEST_CASE(insert_shuffled_hints)
{
  MapT map;
  std::vector< MapT::key_type > keys(1'500);
  std::iota(keys.begin(), keys.end(), 0);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(keys.begin(), keys.end(), gen);
  for (const auto& key: keys) {
    std::uniform_int_distribution<> distr(0, map.size());
    auto pos = std::next(map.begin(), distr(gen));
    map.insert(pos, std::make_pair(key, ""));
    testMapInvariants(map);
  }
  std::sort(keys.begin(), keys.end(), map.keyComp());
  const auto cmp = [](const auto& lhs, const auto& rhs) -> bool
  {
    return lhs == rhs.first;
  };
  BOOST_TEST(std::equal(keys.begin(), keys.end(), map.begin(), map.end(), cmp));
}

BOOST_AUTO_TEST_CASE(insert_empty_range)
{
  MapT map;
  const std::initializer_list< MapT::value_type > init;
  map.insert(init.begin(), init.end());
  testMapInvariants(map);
  BOOST_TEST(map.empty());
}

BOOST_AUTO_TEST_CASE(insert_range)
{
  const MapT expected{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  MapT map;
  map.insert(expected.begin(), expected.end());
  testMapInvariants(map);
  BOOST_TEST(map == expected);
}

BOOST_AUTO_TEST_CASE(insert_initializer_list)
{
  const std::initializer_list< MapT::value_type > init{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  const MapT initied(init);
  MapT map;
  map.insert(init);
  testMapInvariants(map);
  BOOST_TEST(map == initied);
}

BOOST_AUTO_TEST_CASE(erase_all_form_empty)
{
  MapT map;
  map.erase(map.begin(), map.end());
  testMapInvariants(map);
  BOOST_TEST(map.empty());
}

BOOST_AUTO_TEST_CASE(erase_all)
{
  MapT map;
  std::vector< MapT::key_type > keys(500);
  std::iota(keys.begin(), keys.end(), -1);
  for (const auto& key: keys) {
    map.insert(std::make_pair(key, ""));
  }
  map.erase(map.begin(), map.end());
  testMapInvariants(map);
  BOOST_TEST(map.empty());
}

BOOST_AUTO_TEST_CASE(erase_key)
{
  const MapT expected{
    { 1, "abc" },
    { 3, "ghi" },
  };
  MapT map{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  map.erase(2);
  testMapInvariants(map);
  BOOST_TEST(map == expected);
}

BOOST_AUTO_TEST_CASE(erase_shuffeled_keys)
{
  MapT map;
  std::vector< MapT::key_type > keys(1'500);
  std::iota(keys.begin(), keys.end(), 0);
  for (const auto& key: keys) {
    map.insert(std::make_pair(key, ""));
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(keys.begin(), keys.end(), gen);
  size_t size = keys.size();
  for (const auto& key: keys) {
    BOOST_TEST(map.erase(key) == 1);
    testMapInvariants(map);
    --size;
    BOOST_TEST(map.size() == size);
  }
  testMapInvariants(map);
  BOOST_TEST(map.empty());
}

BOOST_AUTO_TEST_CASE(erase_shuffled_pos)
{
  MapT map;
  std::vector< MapT::key_type > keys(1'500);
  std::iota(keys.begin(), keys.end(), 0);
  for (const auto& key: keys) {
    map.insert(std::make_pair(key, ""));
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  for (size_t i = keys.size(); i != 0; --i) {
    std::uniform_int_distribution<> distr(0, map.size() - 1);
    auto pos = std::next(map.begin(), distr(gen));
    BOOST_TEST(map.size() == i);
    map.erase(pos);
    testMapInvariants(map);
  }
  BOOST_TEST(map.empty());
  testMapInvariants(map);
}

BOOST_AUTO_TEST_CASE(clear_empty)
{
  MapT map;
  map.clear();
  testMapInvariants(map);
  BOOST_TEST(map.empty());
}

BOOST_AUTO_TEST_CASE(clear)
{
  MapT map{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  map.clear();
  testMapInvariants(map);
  BOOST_TEST(map.empty());
}

BOOST_AUTO_TEST_CASE(swap)
{
  MapT first{ { 1, "abc" } };
  MapT second{ { 2, "def" } };
  const MapT firstCopy(first);
  const MapT secondCopy(second);
  first.swap(second);
  testMapInvariants(first);
  testMapInvariants(second);
  BOOST_TEST(first == secondCopy);
  BOOST_TEST(second == firstCopy);
}

BOOST_AUTO_TEST_CASE(emplace_empty)
{
  const MapT::value_type value = { 1, "abc" };
  MapT map;
  const auto res = map.emplace(value);
  testMapInvariants(map);
  BOOST_TEST(map.size() == 1);
  BOOST_TEST((*map.begin() == value));
  BOOST_TEST((res.first == map.begin()));
  BOOST_TEST(res.second);
}

BOOST_AUTO_TEST_CASE(emplace)
{
  MapT map{
    { 2, "def" },
    { 3, "ghi" },
  };
  const MapT::value_type value = { 1, "abc" };
  map.emplace(value);
  testMapInvariants(map);
  BOOST_TEST(map.size() == 3);
  BOOST_TEST((*map.begin() == value));
}

BOOST_AUTO_TEST_CASE(emplace_hint)
{
  MapT map{
    { 2, "def" },
    { 3, "ghi" },
  };
  const MapT::value_type value = { 1, "abc" };
  map.emplaceHint(map.begin(), value);
  testMapInvariants(map);
  BOOST_TEST(map.size() == 3);
  BOOST_TEST((*map.begin() == value));
}

BOOST_AUTO_TEST_SUITE_END();
BOOST_AUTO_TEST_SUITE(operations);

BOOST_AUTO_TEST_CASE(find_in_empty)
{
  const MapT map;
  const auto res = map.find(1);
  testMapInvariants(map);
  BOOST_TEST(map.empty());
  BOOST_TEST((res == map.end()));
}

BOOST_AUTO_TEST_CASE(find_existing)
{
  const MapT map{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  const auto res = map.find(1);
  testMapInvariants(map);
  BOOST_TEST((res == map.begin()));
}

BOOST_AUTO_TEST_CASE(find_non_existent)
{
  const MapT map{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  const auto res = map.find(4);
  testMapInvariants(map);
  BOOST_TEST((res == map.end()));
}

BOOST_AUTO_TEST_CASE(count)
{
  const MapT map{
    { 2, "def" },
  };
  const size_t res = map.count(2);
  testMapInvariants(map);
  BOOST_TEST(res == 1);
}

BOOST_AUTO_TEST_CASE(lower_bound_empty)
{
  const MapT map;
  const auto res = map.lowerBound(1);
  testMapInvariants(map);
  BOOST_TEST((res == map.end()));
}

BOOST_AUTO_TEST_CASE(lower_bound)
{
  const MapT map{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  const auto res = map.lowerBound(2);
  testMapInvariants(map);
  BOOST_TEST((res == ++map.begin()));
}

BOOST_AUTO_TEST_CASE(upper_bound_empty)
{
  const MapT map;
  const auto res = map.lowerBound(1);
  testMapInvariants(map);
  BOOST_TEST((res == map.end()));
}

BOOST_AUTO_TEST_CASE(upper_bound)
{
  const MapT map{
    { 1, "abc" },
    { 2, "def" },
    { 3, "ghi" },
  };
  const auto res = map.upperBound(1);
  testMapInvariants(map);
  BOOST_TEST((res == ++map.begin()));
}

BOOST_AUTO_TEST_CASE(equal_range)
{
  const MapT map{
    { 1, "abc" },
  };
  const auto res = map.equalRange(1);
  testMapInvariants(map);
  BOOST_TEST((res.first == map.begin()));
  BOOST_TEST((res.second == map.end()));
}

BOOST_AUTO_TEST_SUITE_END();
BOOST_AUTO_TEST_SUITE(comparison_operators);

BOOST_AUTO_TEST_CASE(compare_empty)
{
  const MapT map1;
  const MapT map2;
  BOOST_TEST(!(map1 < map2));
  BOOST_TEST(!(map1 > map2));
  BOOST_TEST(map1 == map2);
  testMapComparison(map1, map2);
}

BOOST_AUTO_TEST_CASE(equality_operator_different_sizes)
{
  const MapT map1{ { 1, "" } };
  const MapT map2{ { 1, "" }, { 2, "" } };
  BOOST_TEST(!(map1 == map2));
  testMapComparison(map1, map2);
}

BOOST_AUTO_TEST_CASE(equality_operator_different_values)
{
  const MapT map1{ { 1, "" } };
  const MapT map2{ { 2, "" } };
  BOOST_TEST(!(map1 == map2));
  testMapComparison(map1, map2);
}

BOOST_AUTO_TEST_CASE(equality_operator_same)
{
  const MapT map1{ { 1, "" } };
  const MapT map2{ { 1, "" } };
  BOOST_TEST(map1 == map2);
  testMapComparison(map1, map2);
}

BOOST_AUTO_TEST_CASE(less_operator_different_sizes)
{
  const MapT map1{ { 1, "" } };
  const MapT map2{ { 1, "" }, { 2, "" } };
  BOOST_TEST(map1 < map2);
  testMapComparison(map1, map2);
}

BOOST_AUTO_TEST_CASE(less_operator_different_values)
{
  const MapT map1{ { 1, "" }, { 2, "" } };
  const MapT map2{ { 1, "" }, { 3, "" } };
  BOOST_TEST(map1 < map2);
  testMapComparison(map1, map2);
}

BOOST_AUTO_TEST_SUITE_END();

