#include <boost/test/unit_test.hpp>
#include "HashTable.hpp"

BOOST_AUTO_TEST_SUITE(HashTableTest)

BOOST_AUTO_TEST_CASE(TestEmptyHashTable)
{
  gavrilova::HashTable< int, std::string > table;
  BOOST_TEST(table.empty());
  BOOST_TEST(table.size() == 0);
  BOOST_CHECK(table.find(42) == table.end());
  BOOST_CHECK_THROW(table.at(42), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(TestInsertAndFind)
{
  gavrilova::HashTable< int, std::string > table;

  auto result1 = table.insert({10, "ten"});
  BOOST_TEST(result1.second);
  BOOST_CHECK(result1.first->first == 10);
  BOOST_CHECK(!table.empty());
  BOOST_TEST(table.size() == 1);

  auto it1 = table.find(10);
  BOOST_CHECK(it1 != table.end());
  BOOST_TEST(it1->second == "ten");
  BOOST_TEST(table.at(10) == "ten");

  auto result2 = table.insert({5, "five"});
  BOOST_TEST(result2.second);
  BOOST_TEST(table.size() == 2);
  BOOST_CHECK(table.find(5) != table.end());
  BOOST_TEST(table.at(5) == "five");

  table.insert({15, "fifteen"});
  table.insert({20, "twenty"});
  BOOST_TEST(table.size() == 4);
  BOOST_CHECK(table.find(15) != table.end());
  BOOST_CHECK(table.find(20) != table.end());
  BOOST_TEST(table.at(15) == "fifteen");
  BOOST_TEST(table.at(20) == "twenty");

  auto result_duplicate = table.insert({15, "another_fifteen"});
  BOOST_TEST(!result_duplicate.second);
  BOOST_TEST(table.size() == 4);
  BOOST_TEST(table.at(15) == "fifteen");
}

BOOST_AUTO_TEST_CASE(TestOperatorBrackets)
{
  gavrilova::HashTable< int, std::string > table;
  table[10] = "ten";
  BOOST_TEST(table.size() == 1);
  BOOST_TEST(table[10] == "ten");

  table[5] = "five";
  BOOST_TEST(table.size() == 2);
  BOOST_TEST(table[5] == "five");

  const int key = 15;
  table[key] = "fifteen";
  BOOST_TEST(table.size() == 3);
  BOOST_TEST(table[key] == "fifteen");
}

BOOST_AUTO_TEST_CASE(TestErase)
{
  gavrilova::HashTable< int, std::string > table;
  table.insert({10, "ten"});
  table.insert({5, "five"});
  table.insert({15, "fifteen"});

  BOOST_TEST(table.erase(5) == 1);
  BOOST_TEST(table.size() == 2);
  BOOST_CHECK(table.find(5) == table.end());

  BOOST_TEST(table.erase(999) == 0);
  BOOST_TEST(table.size() == 2);

  auto it = table.find(10);
  table.erase(it);
  BOOST_TEST(table.size() == 1);
  BOOST_CHECK(table.find(10) == table.end());
}

BOOST_AUTO_TEST_CASE(TestClear)
{
  gavrilova::HashTable< int, std::string > table;
  table.insert({10, "ten"});
  table.insert({5, "five"});

  table.clear();
  BOOST_TEST(table.empty());
  BOOST_TEST(table.size() == 0);
  BOOST_CHECK(table.find(10) == table.end());
}

BOOST_AUTO_TEST_CASE(TestRehash)
{
  gavrilova::HashTable< int, std::string > table;
  size_t initial_buckets = table.bucket_count();

  for (int i = 0; i < 100; ++i) {
    table.insert({i, "value_" + std::to_string(i)});
  }

  BOOST_TEST(table.bucket_count() > initial_buckets);
  BOOST_TEST(table.size() == 100);
  BOOST_TEST(table.load_factor() < table.max_load_factor());

  for (int i = 0; i < 100; ++i) {
    BOOST_CHECK(table.find(i) != table.end());
  }
}

BOOST_AUTO_TEST_CASE(TestMoveConstructor)
{
  gavrilova::HashTable< int, std::string > table1;
  table1.insert({10, "ten"});
  table1.insert({5, "five"});

  gavrilova::HashTable< int, std::string > table2(std::move(table1));

  BOOST_TEST(table1.empty());
  BOOST_TEST(table2.size() == 2);
  BOOST_CHECK(table2.find(10) != table2.end());
  BOOST_TEST(table2.at(10) == "ten");
}

BOOST_AUTO_TEST_CASE(TestInitializerListConstructor)
{
  gavrilova::HashTable< int, std::string > table = {
      {1, "one"},
      {2, "two"},
      {3, "three"}};

  BOOST_TEST(table.size() == 3);
  BOOST_CHECK(table.find(1) != table.end());
  BOOST_CHECK(table.find(2) != table.end());
  BOOST_CHECK(table.find(3) != table.end());
}

BOOST_AUTO_TEST_CASE(TestLoadFactor)
{
  gavrilova::HashTable< int, std::string > table;
  table.max_load_factor(0.5f);
  BOOST_TEST(table.max_load_factor() == 0.5f);

  for (int i = 0; i < 10; ++i) {
    table.insert({i, "value"});
    BOOST_TEST(table.load_factor() <= table.max_load_factor());
  }
}

BOOST_AUTO_TEST_SUITE_END()
