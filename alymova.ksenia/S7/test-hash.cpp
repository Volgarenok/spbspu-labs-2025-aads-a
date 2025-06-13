#include <boost/test/unit_test.hpp>
#include <exception>
#include "hash-table.hpp"

BOOST_AUTO_TEST_CASE(test_constructors_operators)
{
  using Map = alymova::HashTable< int, std::string >;

  Map table1;
  BOOST_TEST(table1.empty());

  Map table2(table1);
  BOOST_TEST(table2.empty());

  table1.emplace(1, "cat");
  Map table3(table1);
  BOOST_TEST(table3.size() == 1);
  BOOST_TEST(table3.begin()->first == 1);

  Map table4(std::move(table3));
  BOOST_TEST(table4.size() == 1);
  BOOST_TEST(table4.begin()->second == "cat");
  BOOST_TEST(table3.size() == 0);

  Map table5(table4.begin(), table4.end());
  BOOST_TEST(table5.size() == 1);
  BOOST_TEST(table5.begin()->second == "cat");

  Map table6({{2, "d"}, {3, "p"}});
  BOOST_TEST(table6.size() == 2);
  BOOST_TEST(table6.begin()->second == "d");

  table1 = table6;
  BOOST_TEST(table1.size() == 2);
  BOOST_TEST(table1.begin()->second == "d");

  table1 = Map({{1, "ccc"}, {2, "ddd"}, {3, "ppp"}});
  BOOST_TEST(table1.size() == 3);
  BOOST_TEST(table1.begin()->second == "ccc");
}
BOOST_AUTO_TEST_CASE(test_clear_swap)
{
  using Map = alymova::HashTable< int, std::string >;

  Map table1;
  Map table2({{1, "aaa"}, {2, "bbb"}, {3, "ccc"}});
  table1.swap(table2);
  BOOST_TEST(table1.size() == 3);
  BOOST_TEST(table1.begin()->first == 1);
  BOOST_TEST(table2.empty());

  table1.clear();
  BOOST_TEST(table1.empty());
  BOOST_TEST((table1.begin() == table1.end()));
}
BOOST_AUTO_TEST_CASE(test_iterators_size)
{
  using Map = alymova::HashTable< int, std::string >;

  Map table1;
  BOOST_TEST((table1.begin() == table1.end()));
  BOOST_TEST(table1.empty());
  BOOST_TEST(table1.size() == 0);

  std::pair< int, std::string >p(1, "a");
  table1.insert(p);
  auto it = table1.begin();
  BOOST_TEST((*it == p));
  BOOST_TEST(it->first == 1);
  BOOST_TEST(!table1.empty());
  BOOST_TEST(table1.size() == 1);

  ++it;
  BOOST_TEST((it == table1.end()));

  const Map table2;
  BOOST_TEST((table2.begin() == table2.end()));
  BOOST_TEST((table2.begin() == table2.cbegin()));
}
BOOST_AUTO_TEST_CASE(test_load_factor_rehash)
{
  using Map = alymova::HashTable< int, std::string >;

  Map table1;
  BOOST_TEST(table1.load_factor() == 0.0f);
  BOOST_TEST(table1.max_load_factor() == 0.7f);

  for (size_t i = 0; i < 6; i++)
  {
    table1.emplace(i, "aaa");
  }
  BOOST_TEST(table1.load_factor() > 0.5f);

  table1.max_load_factor(0.8f);
  BOOST_TEST(table1.max_load_factor() == 0.8f);

  table1.max_load_factor(0.5f);
  BOOST_TEST(table1.max_load_factor() == 0.5f);
  BOOST_TEST(table1.load_factor() < 0.5f);
  BOOST_TEST(table1.size() == 6);

  Map table2;
  table2.rehash();
  BOOST_TEST(table2.empty());
}
BOOST_AUTO_TEST_CASE(test_find)
{
  using Map = alymova::HashTable< int, std::string >;

  Map table1;
  BOOST_TEST((table1.find(1) == table1.end()));

  table1.emplace(1, "aaa");
  BOOST_TEST(table1.find(1)->first == 1);

  BOOST_TEST((table1.find(12) == table1.end()));

  table1.emplace(2, "bbb");
  BOOST_TEST((table1.find(12) == table1.end()));

  table1.emplace(12, "rrr");
  BOOST_TEST(table1.find(12)->first == 12);

  BOOST_TEST(table1.at(12) == "rrr");
  try
  {
    std::string value = table1.at(300);
  }
  catch (const std::out_of_range& e)
  {
    BOOST_TEST((table1.find(300) == table1.end()));
  }
}
BOOST_AUTO_TEST_CASE(test_insert_emplace)
{
  using Map = alymova::HashTable< int, std::string >;

  Map table1;
  table1.emplace(1, "aaa");
  BOOST_TEST(table1.size() == 1);
  BOOST_TEST(table1.begin()->first == 1);

  table1.emplace(1, "bbb");
  BOOST_TEST(table1.size() == 2);
  BOOST_TEST(table1.begin()->second == "aaa");

  table1.emplace(3, "ccc");
  table1.emplace(1, "ddd");
  BOOST_TEST(table1.size() == 4);

  table1.emplace(10, "fff");
  table1.emplace(10, "rrr");
  BOOST_TEST(table1.size() == 6);
  BOOST_TEST(table1.begin()->first == 10);
  BOOST_TEST(table1.begin()->second == "rrr");

  table1.erase(1);
  table1.emplace(9, "lll");
  table1.emplace(9, "jjj");
  table1.emplace(9, "iii");
  BOOST_TEST(table1.size() == 6);
  BOOST_TEST(table1.begin()->first == 9);
  BOOST_TEST(table1.begin()->second == "iii");

  table1.emplace(4, "www");
  table1.emplace(5, "ttt");
  BOOST_TEST(table1.size() == 8);
  BOOST_TEST(table1.load_factor() < 0.4f);

  auto hint1 = table1.cend();
  table1.emplace_hint(hint1, 100, "qqq");
  BOOST_TEST(table1.size() == 9);

  hint1 = table1.find(100);
  table1.emplace_hint(hint1, 10, "sss");
  BOOST_TEST(table1.size() == 10);

  table1.emplace_hint(hint1, 100, "xxx");
  BOOST_TEST(table1.size() == 10);

  Map table2;
  std::pair< int, std::string > p1(1, "aaa");
  auto it = table2.insert(p1);
  BOOST_TEST(table2.size() == 1);
  BOOST_TEST(table2.begin()->first == 1);
  BOOST_TEST((it == table2.find(1)));

  it = table2.insert({2, "bbb"});
  BOOST_TEST(table2.size() == 2);
  BOOST_TEST((it == table2.find(2)));

  table2.insert(table1.begin(), table1.end());
  BOOST_TEST(table2.size() == 12);
  BOOST_TEST(table1.size() == 10);
  BOOST_TEST((table1.begin() != table1.end()));

  auto hint2 = table2.find(1);
  table2.emplace_hint(hint2, 1, "ooo");
  BOOST_TEST(table2.size() == 12);
}
BOOST_AUTO_TEST_CASE(test_erase)
{
  using Map = alymova::HashTable< int, std::string >;

  Map table1({{1, "aaa"}});
  auto it = table1.erase(table1.begin());
  BOOST_TEST(table1.size() == 0);
  BOOST_TEST((it == table1.end()));

  table1.emplace(1, "aaa");
  table1.emplace(2, "bbb");
  it = table1.erase(table1.begin());
  BOOST_TEST(table1.size() == 1);
  BOOST_TEST(it->first == 2);

  table1.emplace(2, "ccc");
  table1.emplace(2, "ddd");
  it = table1.erase(table1.begin());
  BOOST_TEST(table1.size() == 2);
  BOOST_TEST(it->second == "ccc");

  table1.emplace(10, "ttt");
  table1.emplace(10, "rrr");
  it = table1.erase(table1.find(10));
  BOOST_TEST(table1.size() == 3);
  BOOST_TEST(it->second == "rrr");

  it = table1.erase(table1.cbegin());
  BOOST_TEST(table1.size() == 2);
  BOOST_TEST(it->second == "ddd");

  size_t cnt = table1.erase(100);
  BOOST_TEST(table1.size() == 2);
  BOOST_TEST(cnt == 0);

  cnt = table1.erase(2);
  BOOST_TEST(table1.size() == 1);
  BOOST_TEST(table1.begin()->first == 10);
  BOOST_TEST(cnt == 1);

  it = table1.erase(table1.begin(), table1.begin());
  BOOST_TEST(table1.size() == 1);
  BOOST_TEST((it == table1.begin()));

  it = table1.erase(table1.cbegin(), table1.cend());
  BOOST_TEST(table1.empty());
  BOOST_TEST((it == table1.end()));
}
