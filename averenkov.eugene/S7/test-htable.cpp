#include <boost/test/unit_test.hpp>
#include <hashTable.hpp>

using namespace averenkov;
using IntStringTable = HashTable< int, std::string >;
using StringIntTable = HashTable< std::string, int >;

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  IntStringTable table;
  BOOST_TEST(table.empty());
  BOOST_TEST(table.size() == 0);
}

BOOST_AUTO_TEST_CASE(InsertAndLocateElements)
{
  IntStringTable table;
  auto insertion_result = table.insert({ 42, "answer" });
  BOOST_TEST(insertion_result.second);
  BOOST_TEST(insertion_result.first->key == 42);
  BOOST_TEST(insertion_result.first->value == "answer");

  auto search_result = table.find(42);
  BOOST_TEST(search_result->value == "answer");

  auto duplicate_insert = table.insert({ 42, "duplicate" });
  BOOST_TEST(!duplicate_insert.second);
  BOOST_TEST(duplicate_insert.first->value == "answer");
}

BOOST_AUTO_TEST_CASE(AccessOperations)
{
  IntStringTable table;
  table[777] = "lucky";
  BOOST_TEST(table.at(777) == "lucky");
  BOOST_TEST(table[777] == "lucky");

  table[999] = "emergency";
  BOOST_TEST(table.at(999) == "emergency");
}

BOOST_AUTO_TEST_CASE(RemoveElements)
{
  IntStringTable table;
  table.insert({ 1, "first" });
  table.insert({ 2, "second" });
  table.insert({ 3, "third" });

  BOOST_TEST(table.size() == 3);
  BOOST_TEST(table.erase(2) == 1);
  BOOST_TEST(table.size() == 2);

  auto iterator = table.find(1);
  table.erase(iterator);
  BOOST_TEST(table.size() == 1);
}

BOOST_AUTO_TEST_CASE(TableResizing)
{
  IntStringTable table;
  size_t starting_size = table.size();

  for (int index = 0; index < 15; ++index)
  {
    table.insert({ index, "value_" + std::to_string(index) });
  }

  BOOST_TEST(table.size() > starting_size);
  BOOST_TEST(table.load_factor() < table.max_load_factor());
}

BOOST_AUTO_TEST_CASE(ClearAllData)
{
  IntStringTable table;
  table.insert({ 10, "ten" });
  table.insert({ 20, "twenty" });

  BOOST_TEST(!table.empty());
  table.clear();
  BOOST_TEST(table.empty());
  BOOST_TEST(table.size() == 0);
}

BOOST_AUTO_TEST_CASE(LoadFactorControl)
{
  IntStringTable table;
  table.max_load_factor(0.4f);

  for (int number = 0; number < 8; ++number)
  {
    table.insert({ number, "num_" + std::to_string(number) });
  }

  BOOST_TEST(table.load_factor() <= table.max_load_factor());
}

BOOST_AUTO_TEST_CASE(SwapTables)
{
  IntStringTable first_table;
  first_table.insert({ 1, "alpha" });

  IntStringTable second_table;
  second_table.insert({ 2, "beta" });

  first_table.swap(second_table);

  BOOST_TEST(first_table.find(2)->value == "beta");
  BOOST_TEST(second_table.find(1)->value == "alpha");
}

BOOST_AUTO_TEST_CASE(StringKeyOperations)
{
  StringIntTable table;
  table.insert({ "apple", 5 });
  table.insert({ "banana", 3 });

  BOOST_TEST(table.find("apple")->value == 5);
  BOOST_TEST(table.find("banana")->value == 3);
  BOOST_TEST(table.size() == 2);
}

BOOST_AUTO_TEST_CASE(IteratorTraversal)
{
  IntStringTable table;
  table.insert({ 1, "one" });
  table.insert({ 2, "two" });
  table.insert({ 3, "three" });

  size_t count = 0;
  for (auto it = table.begin(); it != table.end(); ++it)
  {
    ++count;
  }
  BOOST_TEST(count == 3);
}

BOOST_AUTO_TEST_CASE(ConstIteratorAccess)
{
  IntStringTable table;
  table.insert({ 5, "five" });

  const IntStringTable& const_table = table;
  auto const_it = const_table.find(5);
  BOOST_TEST(const_it->value == "five");
}

BOOST_AUTO_TEST_CASE(TableExpansion)
{
  IntStringTable table;

  for (int i = 0; i < 50; ++i)
  {
    table.insert({ i, "element" });
  }

  BOOST_TEST(table.size() == 50);
  BOOST_TEST(table.load_factor() <= table.max_load_factor());
}

BOOST_AUTO_TEST_CASE(MoveOperations)
{
  IntStringTable original;
  original.insert({ 1, "movi" });

  IntStringTable moved = std::move(original);
  BOOST_TEST(moved.find(1)->value == "movi");
  BOOST_TEST(original.empty());
}

BOOST_AUTO_TEST_SUITE_END()
