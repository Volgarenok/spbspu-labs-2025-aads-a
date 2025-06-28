#include <boost/test/unit_test.hpp>
#include <hash_table/definition.hpp>

BOOST_AUTO_TEST_SUITE(constructors_tests)
BOOST_AUTO_TEST_CASE(default_constructor_test)
{
  maslevtsov::HashTable< int, int > table;
  BOOST_TEST(table.empty());
}

BOOST_AUTO_TEST_CASE(range_constructor_test)
{
  maslevtsov::HashTable< int, int > table1 = {{0, 0}, {1, 1}};
  maslevtsov::HashTable< int, int > table2(table1.begin(), table1.end());
  auto it = table2.cbegin();
  BOOST_TEST((it++)->first == 0);
  BOOST_TEST(it->first == 1);
  BOOST_TEST(table2.size() == 2);
}

BOOST_AUTO_TEST_CASE(initializer_list_constructor_test)
{
  maslevtsov::HashTable< int, int > table = {{0, 0}, {1, 1}};
  auto it = table.cbegin();
  BOOST_TEST((it++)->first == 0);
  BOOST_TEST(it->first == 1);
  BOOST_TEST(table.size() == 2);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(lookup_tests)
BOOST_AUTO_TEST_CASE(brackets_operator_test)
{
  maslevtsov::HashTable< int, int > table = {{0, 0}, {1, 1}};
  table[2] = 2;
  auto it = table.cbegin();
  BOOST_TEST((it++)->first == 0);
  BOOST_TEST((it++)->first == 1);
  BOOST_TEST(it->first == 2);
  BOOST_TEST(table.size() == 3);
  table[2] = 3;
  it = table.cbegin();
  BOOST_TEST((it++)->first == 0);
  BOOST_TEST((it++)->first == 1);
  BOOST_TEST(it->first == 2);
  BOOST_TEST(it->second == 3);
  BOOST_TEST(table.size() == 3);
}

BOOST_AUTO_TEST_CASE(at_test)
{
  maslevtsov::HashTable< int, int > table = {{0, 0}};
  try {
    table.at(1);
  } catch (const std::out_of_range&) {
    BOOST_TEST(true);
  }
  BOOST_TEST(table.at(0) == 0);
}

BOOST_AUTO_TEST_CASE(find_test)
{
  maslevtsov::HashTable< int, int > table = {{0, 0}, {1, 1}, {2, 2}};
  BOOST_TEST(table.find(0)->first == 0);
  BOOST_TEST(table.find(1)->first == 1);
  BOOST_TEST(table.find(2)->first == 2);
  BOOST_TEST((table.find(3) == table.end()));
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(iterators_tests)
BOOST_AUTO_TEST_CASE(begin_test)
{
  maslevtsov::HashTable< int, int > table = {{0, 0}};
  BOOST_TEST(table.begin()->first == 0);
  BOOST_TEST(table.begin()->second == 0);
  BOOST_TEST(table.cbegin()->first == 0);
  BOOST_TEST(table.cbegin()->second == 0);
}

BOOST_AUTO_TEST_CASE(end_test)
{
  maslevtsov::HashTable< int, int > table = {{0, 0}, {1, 1}};
  auto it = table.begin();
  auto c_it = table.cbegin();
  ++(++it);
  ++(++c_it);
  BOOST_TEST((it == table.end()));
  BOOST_TEST((c_it == table.cend()));
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(capacity_tests)
BOOST_AUTO_TEST_CASE(empty_test)
{
  maslevtsov::HashTable< int, int > table;
  BOOST_TEST(table.empty());
  table[0] = 0;
  BOOST_TEST(!table.empty());
}

BOOST_AUTO_TEST_CASE(size_test)
{
  maslevtsov::HashTable< int, int > table;
  BOOST_TEST(table.size() == 0);
  table[0] = 0;
  BOOST_TEST(table.size() == 1);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(modifiers_tests)
BOOST_AUTO_TEST_CASE(clear_test)
{
  maslevtsov::HashTable< int, int > table;
  table.clear();
  BOOST_TEST(table.empty());
  table[0] = 0;
  table.clear();
  BOOST_TEST(table.empty());
}

BOOST_AUTO_TEST_CASE(insert_emplace_test)
{
  maslevtsov::HashTable< int, int > table;
  table.insert(std::make_pair(0, 0));
  BOOST_TEST(table.insert(std::make_pair(0, 0)).first->first == 0);
  BOOST_TEST(table.size() == 1);
  BOOST_TEST(table.insert(std::make_pair(0, 0)).first->first == 0);
  BOOST_TEST(!table.insert(std::make_pair(0, 0)).second);

  maslevtsov::HashTable< int, int > src = {{0, 0}, {1, 1}};
  table.clear();
  table.insert(src.begin(), src.end());
  auto it = table.cbegin();
  BOOST_TEST((it++)->first == 0);
  BOOST_TEST(it->first == 1);
  BOOST_TEST(table.size() == 2);

  table.clear();
  for (int i = 0; i != 3; ++i) {
    table[i] = i;
  }
  BOOST_TEST(table.insert(table.cbegin(), std::make_pair(0, 0))->first == 0);
  BOOST_TEST(table.size() == 3);
  BOOST_TEST(table.emplace_hint(table.cbegin(), 0, 0)->first == 0);
  BOOST_TEST(table.size() == 3);

  table.clear();
  for (int i = 0; i != 128; ++i) {
    table.emplace(i, i);
  }
  BOOST_TEST(table.size() == 128);
  BOOST_TEST(table.emplace_hint(table.cbegin(), 0, 0)->first == 0);
  BOOST_TEST(table.size() == 128);
}

BOOST_AUTO_TEST_CASE(erase_test)
{
  maslevtsov::HashTable< int, int > table = {{0, 0}, {1, 1}, {2, 2}};
  auto next = table.erase(table.find(1));
  auto it = table.cbegin();
  BOOST_TEST(next->first == 2);
  BOOST_TEST((it++)->first == 0);
  BOOST_TEST(it->first == 2);
  BOOST_TEST(table.size() == 2);
  table.erase(table.cbegin(), table.cend());
  BOOST_TEST(table.empty());
  table[0] = 0;
  table[1] = 1;
  BOOST_TEST(table.erase(0) == 1);
  BOOST_TEST(table.size() == 1);
  BOOST_TEST(table.erase(2) == 0);
  BOOST_TEST(table.size() == 1);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  maslevtsov::HashTable< int, int > table1 = {{0, 0}, {1, 1}};
  maslevtsov::HashTable< int, int > table2 = {{2, 2}};
  table1.swap(table2);
  BOOST_TEST(table1.cbegin()->first == 2);
  BOOST_TEST(table1.size() == 1);
  BOOST_TEST(table2.cbegin()->first == 0);
  BOOST_TEST((++table2.cbegin())->first == 1);
  BOOST_TEST(table2.size() == 2);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(hash_policy_tests)
BOOST_AUTO_TEST_CASE(load_factor_test)
{
  maslevtsov::HashTable< int, int > table = {{0, 0}, {1, 1}, {2, 2}, {3, 3}};
  BOOST_TEST(table.load_factor() == 0.25);
}

BOOST_AUTO_TEST_CASE(max_load_factor_test)
{
  maslevtsov::HashTable< int, int > table;
  for (int i = 0; i != 32; ++i) {
    table[i] = i;
  }
  BOOST_TEST(table.max_load_factor() == 1.0);
  BOOST_TEST(table.load_factor() == 1.0);
  table.max_load_factor(0.5);
  BOOST_TEST(table.max_load_factor() == 0.5);
  BOOST_TEST(table.load_factor() == 0.5);
  BOOST_TEST(table.size() == 32);
}

BOOST_AUTO_TEST_CASE(rehash_test)
{
  maslevtsov::HashTable< int, int > table = {{0, 0}, {1, 1}, {2, 2}, {3, 3}};
  float prev_load_factor = table.load_factor();
  table.rehash(8);
  BOOST_TEST(table.load_factor() == prev_load_factor);
  table.rehash(62);
  BOOST_TEST(table.load_factor() == 0.0625);
}
BOOST_AUTO_TEST_SUITE_END()
