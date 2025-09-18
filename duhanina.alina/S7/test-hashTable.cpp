#include <boost/test/unit_test.hpp>
#include <HashTable.hpp>

using HashTable = duhanina::HashTable< int, std::string >;

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  HashTable table;
  BOOST_TEST(table.empty());
}

BOOST_AUTO_TEST_CASE(InsertAndFind)
{
  HashTable table;
  auto result = table.insert({ 1, "one" });
  BOOST_TEST(result.second);
  BOOST_TEST(result.first->first == 1);
  BOOST_TEST(result.first->second == "one");

  auto found = table.find(1);
  BOOST_TEST(found->second == "one");

  auto result2 = table.insert({ 1, "oneone" });
  BOOST_TEST(!result2.second);
  BOOST_TEST(result2.first->second == "one");
}

BOOST_AUTO_TEST_CASE(Emplace)
{
  HashTable table;
  auto result = table.emplace(2, "two");
  BOOST_TEST(result.second);
  BOOST_TEST(result.first->first == 2);
  BOOST_TEST(result.first->second == "two");

  auto result2 = table.emplace(2, "twotwo");
  BOOST_TEST(!result2.second);
  BOOST_TEST(result2.first->second == "two");
}

BOOST_AUTO_TEST_CASE(AtAndOperatorBracket)
{
  HashTable table;
  table[1] = "one";
  BOOST_TEST(table.at(1) == "one");
  BOOST_TEST(table[1] == "one");
}

BOOST_AUTO_TEST_CASE(Erase)
{
  HashTable table;
  table.insert({ 1, "one" });
  table.insert({ 2, "two" });
  table.insert({ 3, "three" });

  BOOST_TEST(table.size() == 3);
  BOOST_TEST(table.erase(2) == 1);
  BOOST_TEST(table.size() == 2);

  auto it = table.find(1);
  table.erase(it);
  BOOST_TEST(table.size() == 1);
}

BOOST_AUTO_TEST_CASE(Rehash)
{
  HashTable table;
  size_t initial_size = table.size();
  for (int i = 0; i < 10; ++i)
  {
    table.insert({ i, "i" });
  }
  BOOST_TEST(table.size() > initial_size);
  BOOST_TEST(table.load_factor() < table.max_load_factor());
}

BOOST_AUTO_TEST_CASE(Clear)
{
  HashTable table;
  table.insert({ 1, "one" });
  table.insert({ 2, "two" });

  BOOST_TEST(!table.empty());
  table.clear();
  BOOST_TEST(table.empty());
}

BOOST_AUTO_TEST_CASE(LoadFactor)
{
  HashTable table;
  table.max_load_factor(0.5);

  for (int i = 0; i < 6; ++i)
  {
    table.insert({ i, "i" });
  }

  BOOST_TEST(table.load_factor() <= table.max_load_factor());
}

BOOST_AUTO_TEST_CASE(Swap)
{
  HashTable table1;
  table1.insert({ 1, "one" });

  HashTable table2;
  table2.insert({ 2, "two" });

  table1.swap(table2);

  BOOST_TEST(table1.find(2)->second == "two");
  BOOST_TEST(table2.find(1)->second == "one");
}
