#include <boost/test/unit_test.hpp>
#include <hash_table/hashTable.hpp>

BOOST_AUTO_TEST_CASE(test_insert_and_find)
{
  smirnov::HashTable< int, std::string > table;
  auto result1 = table.insert(1, "one");
  BOOST_TEST(result1.second);
  BOOST_TEST(result1.first->second == "one");
  auto result2 = table.insert(1, "uno");
  BOOST_TEST(!result2.second);
  BOOST_TEST(result2.first->second == "one");
  auto it3 = table.find(1);
  bool found1 = !(it3 == table.end());
  BOOST_TEST(found1);
  if (found1)
  {
    BOOST_TEST(it3->second == "one");
  }
  auto it4 = table.find(2);
  bool not_found2 = (it4 == table.end());
  BOOST_TEST(not_found2);
}

BOOST_AUTO_TEST_CASE(test_erase)
{
  smirnov::HashTable< int, std::string > table;
  table.insert(1, "one");
  table.insert(2, "two");
  size_t erasedCount1 = table.erase(1);
  BOOST_TEST(erasedCount1 == 1);
  auto it = table.find(1);
  bool not_found1 = (it == table.end());
  BOOST_TEST(not_found1);
  size_t erasedCount2 = table.erase(1);
  BOOST_TEST(erasedCount2 == 0);
  size_t erasedCount3 = table.erase(3);
  BOOST_TEST(erasedCount3 == 0);
}

BOOST_AUTO_TEST_CASE(test_load_factor)
{
  smirnov::HashTable< int, std::string > table;
  for (int i = 0; i < 100; ++i)
  {
    table.insert(i, "value");
  }
  float loadFactor = table.load_factor();
  bool valid_load = loadFactor <= 0.75f;
  BOOST_TEST(valid_load);
}

BOOST_AUTO_TEST_CASE(test_iterators)
{
  smirnov::HashTable< int, std::string > table;
  table.insert(1, "one");
  table.insert(2, "two");
  table.insert(3, "three");
  size_t count = 0;
  for (auto it = table.begin(); it != table.end(); ++it)
  {
    std::string val = it->second;
    bool is_one = (val == "one");
    bool is_two = (val == "two");
    bool is_three = (val == "three");
    bool is_valid = is_one || is_two || is_three;
    BOOST_TEST(is_valid);
    ++count;
  }
  BOOST_TEST(count == 3);
}

BOOST_AUTO_TEST_CASE(test_empty)
{
  smirnov::HashTable< int, std::string > table;
  BOOST_TEST(table.empty());
  BOOST_TEST(table.size() == 0);
  auto it = table.begin();
  bool is_end = (it == table.end());
  BOOST_TEST(is_end);
}

BOOST_AUTO_TEST_CASE(test_rehash)
{
  smirnov::HashTable< int, std::string > table;
  int N = 200;
  for (int i = 0; i < N; ++i)
  {
    table.insert(i, "val");
  }
  BOOST_TEST(table.size() == static_cast< size_t >(N));
  for (int i = 0; i < N; ++i)
  {
    auto it = table.find(i);
    bool found = !(it == table.end());
    BOOST_TEST(found);
    if (found)
    {
      BOOST_TEST(it->second == "val");
    }
  }
  for (int i = 0; i < N; i += 2)
  {
    size_t erased = table.erase(i);
    BOOST_TEST(erased == 1);
  }
  for (int i = 0; i < N; ++i)
  {
    auto it = table.find(i);
    if (i % 2 == 0)
    {
      bool not_found = (it == table.end());
      BOOST_TEST(not_found);
    }
    else
    {
      bool found = !(it == table.end());
      BOOST_TEST(found);
      if (found)
      {
        BOOST_TEST(it->second == "val");
      }
    }
  }
}
