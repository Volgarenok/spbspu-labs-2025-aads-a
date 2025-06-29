#include <boost/test/unit_test.hpp>
#include <boost/hash2/xxhash.hpp>
#include <string>
#include <iostream>
#include "cuckoo-hash-map.h"

using namespace savintsev;

BOOST_AUTO_TEST_CASE(hm_access_to_data)
{
  HashMap< int, std::string > book(10);

  book[1] = "apple";
  book[2] = "orange";
  book[3] = "banana";

  BOOST_TEST(book[1] == "apple");
  BOOST_TEST(book[2] == "orange");
  BOOST_TEST(book[3] == "banana");

  BOOST_TEST(book.size() == 3);

  HashMap< std::string, std::string > cringe;

  cringe["lox1"] = "apple";
  cringe["lox2"] = "orange";
  cringe["lox3"] = "banana";

  BOOST_TEST(cringe["lox1"] == "apple");
  BOOST_TEST(cringe["lox2"] == "orange");
  BOOST_TEST(cringe["lox3"] == "banana");

  BOOST_TEST(cringe.size() == 3);

  cringe.erase("lox2");

  BOOST_TEST(cringe.size() == 2);

  size_t counter = 0;

  try
  {
    cringe.at("lox2");
  }
  catch (const std::out_of_range & e)
  {
    counter++;
  }

  BOOST_TEST(counter == 1);

  cringe.clear();

  BOOST_TEST(cringe.size() == 0);

  try
  {
    cringe.at("lox1");
  }
  catch (const std::out_of_range & e)
  {
    counter++;
  }

  BOOST_TEST(counter == 2);

  cringe["lox1"] = "none";
  BOOST_TEST(cringe["lox1"] == "none");

  HashMap< int, int > lol(10);

  for (int i = 4; i < 1000; ++i)
  {
    lol[i] = i;
  }

  for (int i = 4; i < 1000; ++i)
  {
    BOOST_TEST(lol[i] == i);
  }

}

BOOST_AUTO_TEST_CASE(hm_default_constructor)
{
  HashMap< int, std::string > hm;
  BOOST_TEST(hm.size() == 0);
  BOOST_TEST(hm.empty());
  BOOST_TEST(hm.capacity() > 0);
}

BOOST_AUTO_TEST_CASE(hm_sized_constructor)
{
  const size_t test_size = 100;
  HashMap< int, double > hm(test_size);
  BOOST_TEST(hm.size() == 0);
  BOOST_TEST(hm.capacity() >= test_size);
}

BOOST_AUTO_TEST_CASE(hm_init_constructor)
{
  HashMap< int, char > hm = {{1, 'a'}, {2, 'b'}, {3, 'c'}};
  BOOST_TEST(hm.size() == 3);

  BOOST_TEST(hm[1] == 'a');
  BOOST_TEST(hm[2] == 'b');
  BOOST_TEST(hm[3] == 'c');
}

BOOST_AUTO_TEST_CASE(hm_size_and_empty)
{
  HashMap< std::string, int > hm;
  BOOST_TEST(hm.empty());

  hm["test"] = 42;
  BOOST_TEST(hm.size() == 1);
  BOOST_TEST(!hm.empty());

  hm.clear();
  BOOST_TEST(hm.empty());
  BOOST_TEST(hm.size() == 0);

  hm["test"] = 52;
  BOOST_TEST(hm["test"] == 52);
  BOOST_TEST(hm.size() == 1);
}

BOOST_AUTO_TEST_CASE(hm_clear_noexcept)
{
  HashMap< int, int > hm;
  BOOST_TEST(noexcept(hm.clear()));

  hm.insert({1, 10});
  hm.insert({2, 20});
  hm.clear();
  BOOST_TEST(hm.empty());
}

BOOST_AUTO_TEST_CASE(hm_swap_test)
{
  HashMap< int, std::string > hm1;
  hm1[1] = "one";

  HashMap< int, std::string > hm2;
  hm2[2] = "two";

  hm1.swap(hm2);

  auto it1 = hm1.find(1);
  auto it2 = hm2.find(2);
  auto it3 = hm2.find(1);
  auto it4 = hm1.find(2);

  BOOST_TEST(hm1.size() == 1);
  BOOST_TEST(it1 == hm1.end());
  BOOST_TEST(it2 == hm2.end());
  BOOST_TEST(it3 != hm2.end());
  BOOST_TEST(it4 != hm1.end());

  BOOST_TEST(hm1.find(999) == hm1.end());
}

BOOST_AUTO_TEST_CASE(hm_at_access)
{
  HashMap< std::string, int > hm;
  hm["test"] = 42;

  BOOST_TEST(hm.at("test") == 42);
}

BOOST_AUTO_TEST_CASE(hm_operator_access)
{
  HashMap< int, std::string > hm;

  hm[1] = "one";
  BOOST_TEST(hm[1] == "one");

  int key = 2;
  hm[key] = "two";
  BOOST_TEST(hm[2] == "two");

  hm[std::move(key)] = "moved";
  BOOST_TEST(hm[2] == "moved");
}

BOOST_AUTO_TEST_CASE(hm_iterator_access)
{
  HashMap< int, char > hm = {{1, 'a'}, {2, 'b'}, {3, 'c'}};

  size_t count = 0;
  for (auto it = hm.begin(); it != hm.end(); ++it)
  {
    ++count;
  }
  BOOST_TEST(count == 3);

  count = 0;
  for (auto it = hm.cbegin(); it != hm.cend(); ++it)
  {
    ++count;
  }
  BOOST_TEST(count == 3);

  const auto & chm = hm;
  count = 0;
  for (auto it = chm.begin(); it != chm.end(); ++it)
  {
    ++count;
  }
  BOOST_TEST(count == 3);
}

BOOST_AUTO_TEST_CASE(hm_emplace_tests)
{
  HashMap< std::string, int > hm;

  auto it1 = hm.emplace("test", 42).first;
  BOOST_TEST(it1->second == 42);

  auto it2 = hm.emplace("test", 100);
  BOOST_TEST(!it2.second);
  BOOST_TEST(it2.first->second == 42);

  auto hint_it = hm.emplace_hint(hm.begin(), "hint", 99);
  BOOST_TEST(hint_it != hm.end());
  BOOST_TEST(hm["hint"] == 99);
}

BOOST_AUTO_TEST_CASE(hm_insert_tests)
{
  HashMap< int, std::string > hm;

  auto temp = hm.insert({1, "one"});
  auto it1 = temp.first;
  auto inserted1 = temp.second;
  BOOST_TEST(inserted1);
  BOOST_TEST(it1->second == "one");

  auto it2 = hm.insert(it1, {2, "two"});
  BOOST_TEST(it2 != hm.end());

  std::vector< std::pair< int, std::string > > vec = {{3, "three"}, {4, "four"}};
  hm.insert(vec.begin(), vec.end());
  BOOST_TEST(hm.size() == 4);
}

BOOST_AUTO_TEST_CASE(hm_find_tests)
{
  const HashMap< int, std::string > hm = {{1, "one"}, {2, "two"}};

  auto it1 = hm.find(1);
  BOOST_TEST(it1 != hm.end());
  BOOST_TEST(it1->second == "one");

  auto it2 = hm.find(99);
  BOOST_TEST(it2 == hm.end());

  const auto cit = hm.find(2);
  BOOST_TEST(cit != hm.cend());
  BOOST_TEST(cit->second == "two");
}

BOOST_AUTO_TEST_CASE(hm_load_factor_tests)
{
  HashMap< int, int > hm(10);
  hm.max_load_factor(0.5f);

  BOOST_TEST(hm.max_load_factor() == 0.5f);
  BOOST_TEST(hm.load_factor() == 0.0f);

  for (int i = 0; i < 5; ++i)
  {
    hm[i] = i;
  }

  BOOST_TEST(hm.load_factor() <= hm.max_load_factor());

  hm.rehash(100);
  BOOST_TEST(hm.capacity() >= 100);
  BOOST_TEST(hm.size() == 5);
}

BOOST_AUTO_TEST_CASE(hm_overload)
{
  HashMap< int, int > book(2);

  for (int i = 1; i < 20; ++i)
  {
    book[i] = i * 10;
  }
  for (int i = 1; i < 20; ++i)
  {
    BOOST_TEST(book[i] == i * 10);
  }

  BOOST_TEST(book.capacity() > 2);
}

BOOST_AUTO_TEST_CASE(hm_basic_operations)
{
  savintsev::HashMap<std::string, int> hm;

  BOOST_TEST(hm.empty());
  BOOST_TEST(hm.size() == 0);

  hm["key1"] = 42;
  BOOST_TEST(!hm.empty());
  BOOST_TEST(hm.size() == 1);
  BOOST_TEST(hm["key1"] == 42);

  hm["key1"] = 100;
  BOOST_TEST(hm["key1"] == 100);
  BOOST_TEST(hm.size() == 1);

  hm["key2"] = 200;
  BOOST_TEST(hm.size() == 2);
  BOOST_TEST(hm["key2"] == 200);

  auto it = hm.find("key1");
  BOOST_TEST(it != hm.end());
  BOOST_TEST(it->second == 100);

  hm.erase("key1");
  BOOST_TEST(hm.size() == 1);
  BOOST_TEST(hm.find("key1") == hm.end());

  hm.clear();
  BOOST_TEST(hm.empty());
  BOOST_TEST(hm.size() == 0);
}

BOOST_AUTO_TEST_CASE(hm_collision_handling)
{
  savintsev::HashMap<std::string, int> hm(4);

  hm["key1"] = 1;
  hm["key2"] = 2;
  hm["key3"] = 3;
  hm["key4"] = 4;
  hm["key5"] = 5;

  BOOST_TEST(hm.size() == 5);

  BOOST_TEST(hm["key1"] == 1);
  BOOST_TEST(hm["key2"] == 2);
  BOOST_TEST(hm["key3"] == 3);
  BOOST_TEST(hm["key4"] == 4);
  BOOST_TEST(hm["key5"] == 5);

  hm.erase("key3");
  BOOST_TEST(hm.size() == 4);
  BOOST_TEST(hm.find("key3") == hm.end());
  BOOST_TEST(hm["key1"] == 1);
}

BOOST_AUTO_TEST_CASE(hm_iterator_test)
{
  savintsev::HashMap< std::string, int > hm;
  hm["a"] = 1;
  hm["b"] = 2;
  hm["c"] = 3;

  size_t count = 0;
  for (auto it = hm.begin(); it != hm.end(); ++it)
  {
    count++;
  }
  BOOST_TEST(count == 3);

  auto it = hm.find("b");
  BOOST_TEST(it != hm.end());
  BOOST_TEST(it->first == "b");
  BOOST_TEST(it->second == 2);

  BOOST_TEST(hm.find("x") == hm.end());
}

BOOST_AUTO_TEST_CASE(hm_erase_test)
{
  savintsev::HashMap<std::string, int> hm;
  hm["a"] = 1;
  hm["b"] = 2;
  hm["c"] = 3;

  BOOST_TEST(hm.erase("b") == 1);
  BOOST_TEST(hm.size() == 2);
  BOOST_TEST(hm.find("b") == hm.end());

  BOOST_TEST(hm.erase("x") == 0);
  BOOST_TEST(hm.size() == 2);

  auto it = hm.find("a");
  hm.erase(it);
  BOOST_TEST(hm.size() == 1);
  BOOST_TEST(hm.find("a") == hm.end());
  BOOST_TEST(hm["c"] == 3);
}

BOOST_AUTO_TEST_CASE(hm_emplace_insert_test)
{
  savintsev::HashMap< std::string, int > hm;
  auto res1 = hm.emplace("a", 1);
  BOOST_TEST(res1.second);
  BOOST_TEST(res1.first->second == 1);

  auto res2 = hm.emplace("a", 2);
  BOOST_TEST(!res2.second);
  BOOST_TEST(res2.first->second == 1);

  auto res3 = hm.insert({"b", 2});
  BOOST_TEST(res3.second);
  BOOST_TEST(hm["b"] == 2);
}
