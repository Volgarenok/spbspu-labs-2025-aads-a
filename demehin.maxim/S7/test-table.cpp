#include <boost/test/unit_test.hpp>
#include "hash_table.hpp"

BOOST_AUTO_TEST_CASE(insert_and_access_test)
{
  demehin::HashTable< int, std::string > table;

  table[1] = "one";
  table.insert(std::make_pair(5, "five"));
  table[3] = "three";

  BOOST_TEST(table.size() == 3);
  BOOST_TEST(table[5] == "five");
  BOOST_TEST(table.at(3) == "three");
  BOOST_TEST(table[1] == "one");

  auto res1 = table.insert(std::make_pair(1, "yaone"));
  auto res2 = table.emplace(std::make_pair(6, "six"));

  BOOST_TEST(res1.second == false);
  BOOST_TEST(res2.second == true);
  BOOST_TEST(table[6] == "six");
  BOOST_TEST(table[1] == "one");
}

BOOST_AUTO_TEST_CASE(find_test)
{
  demehin::HashTable< int, std::string > table;

  table[3] = "three";
  table[5] = "five";

  auto it1 = table.find(3);
  auto it2 = table.find(5);
  BOOST_TEST(it1->second == "three");
  BOOST_TEST(it2->second == "five");
}

BOOST_AUTO_TEST_CASE(erase_test)
{
  demehin::HashTable< int, int > table;

  table[3] = 30;
  table[5] = 50;
  table[7] = 70;
  table[9] = 90;

  BOOST_TEST(table.erase(5) == 1);
  BOOST_TEST(table.size() == 3);

  bool isDeleted = false;
  try
  {
    table.at(5);
  }
  catch (...)
  {
    isDeleted = true;
  }
  BOOST_TEST(isDeleted);

  auto it = table.find(3);
  table.erase(it);
  BOOST_TEST(table.size() == 2);

  table.erase(table.begin(), table.end());
  BOOST_TEST(table.empty());
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  demehin::HashTable< int, std::string > table;

  table[1] = "one";
  table[2] = "two";
  table.clear();

  BOOST_TEST(table.empty());
  BOOST_TEST(table.size() == 0);
}

BOOST_AUTO_TEST_CASE(rehash_test)
{
  demehin::HashTable< int, int > table;

  for (int i = 0; i < 20; i++)
  {
    table[i] = i;
  }

  BOOST_TEST(table.size() == 20);
  BOOST_TEST(table.load_factor() <= table.max_load_factor());

  for (int i = 0; i < 20; i++)
  {
    BOOST_TEST(table[i] == i);
  }
}

BOOST_AUTO_TEST_CASE(load_factor_test)
{
  demehin::HashTable< int, int > table;

  BOOST_TEST(table.load_factor() == 0.0);

  table[1] = 1;
  table[2] = 2;
  table[3] = 3;

  float lf = table.load_factor();
  BOOST_TEST(lf > 0.0);
  BOOST_TEST(lf <= table.max_load_factor());

  table.max_load_factor(0.5);
  BOOST_TEST(table.max_load_factor() == 0.5);
}
