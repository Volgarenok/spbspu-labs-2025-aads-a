#include <boost/test/unit_test.hpp>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include <hashtable/hash_table.hpp>

using namespace zholobov;

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  HashTable< int, std::string > table;
  BOOST_TEST(table.empty());
  BOOST_TEST(table.size() == 0);
}

BOOST_AUTO_TEST_CASE(RangeConstructor)
{
  std::vector< std::pair< int, std::string > > elements = {{1, "one"}, {2, "two"}, {3, "three"}};
  HashTable< int, std::string > table(elements.begin(), elements.end());
  BOOST_TEST(table.size() == 3);
  BOOST_TEST(table.at(1) == "one");
  BOOST_TEST(table.at(2) == "two");
  BOOST_TEST(table.at(3) == "three");
}

BOOST_AUTO_TEST_CASE(InitializerListConstructor)
{
  HashTable< int, std::string > table{{1, "one"}, {2, "two"}, {3, "three"}};
  BOOST_TEST(table.size() == 3);
  BOOST_TEST(table.at(1) == "one");
  BOOST_TEST(table.at(2) == "two");
}

BOOST_AUTO_TEST_CASE(IteratorAccess)
{
  HashTable< int, std::string > table{{1, "one"}, {2, "two"}};

  auto it = table.begin();
  BOOST_TEST((it->first == 1 || it->first == 2));
  ++it;
  BOOST_TEST((it->first == 1 || it->first == 2));
  ++it;

  BOOST_CHECK(it == table.end());
}

BOOST_AUTO_TEST_CASE(AtMethod)
{
  HashTable< int, std::string > table{{1, "one"}, {2, "two"}};
  BOOST_TEST(table.at(1) == "one");

  bool exception_thrown = false;
  try {
    table.at(3);
  } catch (const std::out_of_range&) {
    exception_thrown = true;
  }
  BOOST_TEST(exception_thrown);
}

BOOST_AUTO_TEST_CASE(SubscriptOperator)
{
  HashTable< int, std::string > table;
  table[1] = "one";
  BOOST_TEST(table[1] == "one");
  table[1] = "new_one";
  BOOST_TEST(table[1] == "new_one");
}

BOOST_AUTO_TEST_CASE(InsertAndErase)
{
  HashTable< int, std::string > table;
  auto insert_result1 = table.insert(std::make_pair(1, "one"));
  BOOST_TEST(insert_result1.second);
  BOOST_TEST(insert_result1.first->first == 1);

  auto insert_result2 = table.insert(std::make_pair(1, "duplicate"));
  BOOST_TEST(!insert_result2.second);

  BOOST_TEST(table.erase(1) == 1);
  BOOST_TEST(table.erase(999) == 0);
}

BOOST_AUTO_TEST_CASE(EmplaceTest)
{
  HashTable< std::string, int > table;
  auto emplace_result = table.emplace("key", 42);
  BOOST_TEST(emplace_result.second);
  BOOST_TEST(emplace_result.first->first == "key");
  BOOST_TEST(emplace_result.first->second == 42);
}

BOOST_AUTO_TEST_CASE(SwapTest)
{
  HashTable< int, std::string > table1{{1, "one"}};
  HashTable< int, std::string > table2{{2, "two"}};

  table1.swap(table2);

  BOOST_TEST(table1.size() == 1);
  BOOST_TEST(table1.at(2) == "two");
  BOOST_TEST(table2.size() == 1);
  BOOST_TEST(table2.at(1) == "one");
}

BOOST_AUTO_TEST_CASE(FindAndCount)
{
  HashTable< int, std::string > table{{1, "one"}, {2, "two"}};

  auto it = table.find(1);
  bool found = (it != table.end());
  BOOST_TEST(found);
  BOOST_TEST(it->second == "one");

  bool notFound = (table.find(3) == table.end());
  BOOST_TEST(notFound);
  BOOST_TEST(table.count(1) == 1);
  BOOST_TEST(table.count(3) == 0);
}

BOOST_AUTO_TEST_CASE(LoadFactor)
{
  HashTable< int, std::string > table;
  table.max_load_factor(0.7f);
  BOOST_TEST(table.max_load_factor() == 0.7f);

  for (int i = 0; i < 100; ++i) {
    table.insert(std::make_pair(i, std::to_string(i)));
  }
  BOOST_TEST(table.load_factor() > 0.0f);
}

BOOST_AUTO_TEST_CASE(RehashTest)
{
  HashTable< int, std::string > table;
  table.rehashFactor(100);

  for (int i = 0; i < 50; ++i) {
    table.insert(std::make_pair(i, std::to_string(i)));
  }
  BOOST_TEST(table.size() == 50);
}

BOOST_AUTO_TEST_CASE(ClearTest)
{
  HashTable< int, std::string > table{{1, "one"}, {2, "two"}};
  table.clear();
  BOOST_TEST(table.empty());
  BOOST_TEST(table.size() == 0);
}

BOOST_AUTO_TEST_CASE(IteratorRangeErase)
{
  HashTable< int, std::string > table;
  for (int i = 0; i < 10; ++i) {
    table.insert(std::make_pair(i, std::to_string(i)));
  }

  std::unordered_set< int > keys_before;
  for (const auto& pair: table) {
    keys_before.insert(pair.first);
  }

  auto it = table.begin();
  std::advance(it, 2);
  auto end_it = it;
  std::advance(end_it, 4);
  table.erase(it, end_it);

  BOOST_CHECK(table.size() == 6);

  for (const auto& pair: table) {
    BOOST_CHECK(keys_before.find(pair.first) != keys_before.end());
  }
}

BOOST_AUTO_TEST_CASE(ConstIteratorTest)
{
  HashTable< int, std::string > table{{1, "one"}, {2, "two"}};
  const HashTable< int, std::string >& const_table = table;

  auto it = const_table.find(1);
  bool found = (it != const_table.cend());
  BOOST_TEST(found);
  BOOST_TEST(it->second == "one");

  bool notFound = (const_table.find(3) == const_table.cend());
  BOOST_TEST(notFound);
  BOOST_TEST(const_table.count(1) == 1);
}
