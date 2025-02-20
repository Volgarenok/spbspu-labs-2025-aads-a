#include <boost/test/unit_test.hpp>
#include <string>
#include "list.hpp"
#include "list-algorithms.hpp"

BOOST_AUTO_TEST_CASE(test_swap)
{
  std::string out;
  savintsev::List< std::string > meats;
  meats.push_back("hotdog");
  meats.push_back("pierogi");
  meats.push_back("beef");
  savintsev::List< std::string > vegetables;
  vegetables.push_back("latuce");
  vegetables.push_back("tomato");
  meats.swap(vegetables);
  BOOST_TEST(meats.front() == "latuce");
  BOOST_TEST(vegetables.front() == "hotdog");
  BOOST_TEST(meats.back() == "tomato");
  BOOST_TEST(vegetables.back() == "beef");
}

BOOST_AUTO_TEST_CASE(test_basic_interface)
{
  savintsev::List< std::string > list1;
  BOOST_TEST(list1.empty());
  BOOST_TEST(list1.size() == 0);
  list1.push_back("test");
  BOOST_TEST(!list1.empty());
  list1.push_front("another_test");
  BOOST_TEST(list1.size() == 2);
  list1.pop_front();
  BOOST_TEST(list1.size() == 1);
  BOOST_TEST(list1.front() == "test");
  list1.push_back("onemore_test");
  list1.pop_back();
  BOOST_TEST(list1.back() == "test");
  list1.clear();
  BOOST_TEST(list1.empty());
  BOOST_TEST(list1.size() == 0);
}

BOOST_AUTO_TEST_CASE(test_push_and_pop)
{
  savintsev::List< std::string > list1;
  list1.push_back("1");
  list1.push_back("2");
  list1.push_front("0");
  auto it = list1.begin();
  BOOST_TEST(*it == "0");
  ++it;
  BOOST_TEST(*it == "1");
  ++it;
  BOOST_TEST(*it == "2");
  list1.pop_front();
  auto jt = list1.begin();
  BOOST_TEST(*jt++ == "1");
  BOOST_TEST(*jt == "2");
}

BOOST_AUTO_TEST_CASE(test_begin_and_end)
{
  savintsev::List< std::string > list1;
  list1.push_back("front");
  list1.push_back("back");
  auto it = list1.begin();
  BOOST_TEST(*it == "front");
  auto jt = list1.end();
  BOOST_TEST(*--jt == "back");
}

BOOST_AUTO_TEST_CASE(test_front_and_back)
{
  savintsev::List< std::string > list1;
  list1.push_back("front");
  list1.push_back("back");
  BOOST_TEST(list1.front() == "front");
  BOOST_TEST(list1.back() == "back");
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  savintsev::List< std::string > list1;
  list1.push_back("banana");
  list1.push_back("orange");
  list1.push_front("apple");
  savintsev::List< std::string > list2(list1);
  auto it = list2.begin();
  BOOST_TEST(*it == "apple");
  ++it;
  BOOST_TEST(*it == "banana");
  ++it;
  BOOST_TEST(*it == "orange");
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  savintsev::List< std::string > list1;
  list1.push_back("banana");
  list1.push_back("orange");
  list1.push_front("apple");
  savintsev::List< std::string > list2(std::move(list1));
  auto it = list2.begin();
  BOOST_TEST(*it == "apple");
  ++it;
  BOOST_TEST(*it == "banana");
  ++it;
  BOOST_TEST(*it == "orange");
}

BOOST_AUTO_TEST_CASE(test_fill)
{
  savintsev::List< int > list1;
  for (size_t i = 0; i < 8; ++i)
  {
    list1.push_back(0);
  }
  savintsev::fill(list1.begin(), --list1.end(), 5);
  auto it = list1.begin();
  for (size_t i = 0; i < 7; ++i)
  {
    BOOST_TEST(*it == 5);
  }
  BOOST_TEST(list1.back() == 0);
}

BOOST_AUTO_TEST_CASE(test_remove)
{
  savintsev::List< int > list1;
  list1.push_back(0);
  list1.push_back(0);
  list1.push_back(4);
  list1.push_back(0);
  list1.push_back(4);
  list1.remove(4);
  for (auto it = list1.begin(); it != list1.end(); ++it)
  {
    BOOST_TEST(*it == 0);
  }
}

bool divisible_by_five(const int & value)
{
  return (value % 2);
}

struct is_even
{
  bool operator()(const int & value)
  {
    return (value % 2) == 0;
  }
};

BOOST_AUTO_TEST_CASE(test_remove_if)
{
  savintsev::List< int > list1;
  for (size_t i = 1; i <= 10; ++i)
  {
    list1.push_back(i);
  }
  list1.remove_if(divisible_by_five);
  list1.remove_if(is_even());
  size_t i = 1;
  for (auto it = list1.begin(); it != list1.end(); ++it)
  {
    BOOST_TEST(*it == i);
    i += (i == 3) ? 4 : 2;
  }
}

BOOST_AUTO_TEST_CASE(test_splice)
{
  savintsev::List< int > list1, list2;
  for (int i = 1; i <= 4; ++i)
  {
    list1.push_back(i);
    list2.push_back(i*10);
  }
  auto it = list1.begin();
  it++;
  list1.splice(it, list2);
  it = list1.begin();
  BOOST_TEST(*it++ == 1);
  BOOST_TEST(*it++ == 10);
  BOOST_TEST(*it++ == 20);
  BOOST_TEST(*it++ == 30);
  BOOST_TEST(*it++ == 40);
  BOOST_TEST(*it++ == 2);
  BOOST_TEST(*it++ == 3);
  BOOST_TEST(*it == 4);
  BOOST_TEST(list1.size() == 8);
  BOOST_TEST(list2.empty());
  list2.push_back(1);
  BOOST_TEST(list2.front() == 1);
  it = list1.begin();
  it++;
  it++;
  list2.splice(list2.begin(), list1, it);
  auto jt = list2.begin();
  BOOST_TEST(*jt++ == 20);
  BOOST_TEST(*jt == 1);
  BOOST_TEST(list1.size() == 7);
  BOOST_TEST(list2.size() == 2);
  list1.clear();
  list2.clear();
  for (int i = 1; i <= 7; ++i)
  {
    list1.push_back(i);
  }
  list2.push_back(2);
  list2.push_back(6);
  auto first = list1.begin();
  ++first;
  ++first;
  auto last = list1.end();
  --last;
  --last;
  jt = list2.begin();
  ++jt;
  list2.splice(jt, list1, first, last);
  jt = list2.begin();
  for (size_t i = 2; i <= 6; ++i)
  {
    BOOST_TEST(*jt == i);
    ++jt;
  }
  BOOST_TEST(list1.size() == 4);
  BOOST_TEST(list2.size() == 5);
}

BOOST_AUTO_TEST_CASE(test_assign)
{
  savintsev::List< int > list1;
  savintsev::List< int > list2;
  list1.assign(7, 100);
  auto it = list1.begin();
  for (size_t i = 0; i < 7; ++i)
  {
    BOOST_TEST(*it++ == 100);
  }
  list2.assign(list1.begin(), list1.end());
  auto jt = list2.begin();
  for (size_t i = 0; i < 7; ++i)
  {
    BOOST_TEST(*jt++ == 100);
  }
}
