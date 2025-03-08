#include <boost/test/unit_test.hpp>

#include <sstream>

#include "CircularFwdList.hpp"

template < typename T >
std::ostream& operator<<(std::ostream& out, const zholobov::CircularFwdList< T >& list)
{
  if (list.size() != 0) {
    auto it = list.begin();
    out << *it++;
    for (; it != list.end(); ++it) {
      out << " " << *it;
    }
  }
  return out;
}

template < typename T >
std::string to_string(const zholobov::CircularFwdList< T >& list)
{
  std::stringstream ss;
  ss << list;
  return ss.str();
}

BOOST_AUTO_TEST_SUITE(CircularFwsList_Constructors)

BOOST_AUTO_TEST_CASE(default_ctor)
{
  zholobov::CircularFwdList< int > list;
  BOOST_TEST(list.size() == 0);
  BOOST_TEST(list.empty());
  BOOST_TEST(to_string(list) == "");
}

BOOST_AUTO_TEST_CASE(copy_ctor)
{
  {
    zholobov::CircularFwdList< int > list{};
    zholobov::CircularFwdList< int > list2(list);
    BOOST_TEST(to_string(list) == "");
    BOOST_TEST(to_string(list2) == "");
  }
  {
    zholobov::CircularFwdList< int > list{1, 2, 3};
    zholobov::CircularFwdList< int > list2(list);
    BOOST_TEST(list.size() == 3);
    BOOST_TEST(to_string(list) == "1 2 3");
    BOOST_TEST(list2.size() == 3);
    BOOST_TEST(to_string(list2) == "1 2 3");
  }
}

BOOST_AUTO_TEST_CASE(move_ctor)
{
  {
    zholobov::CircularFwdList< int > list{};
    zholobov::CircularFwdList< int > list2(std::move(list));
    BOOST_TEST(to_string(list) == "");
    BOOST_TEST(to_string(list2) == "");
  }
  {
    zholobov::CircularFwdList< int > list{1, 2, 3};
    zholobov::CircularFwdList< int > list2(std::move(list));
    BOOST_TEST(list.size() == 0);
    BOOST_TEST(to_string(list) == "");
    BOOST_TEST(list2.size() == 3);
    BOOST_TEST(to_string(list2) == "1 2 3");
  }
}

BOOST_AUTO_TEST_CASE(initializer_list_ctor)
{
  {
    zholobov::CircularFwdList< int > list{};
    BOOST_TEST(to_string(list) == "");
  }
  {
    zholobov::CircularFwdList< int > list{1, 2, 3};
    BOOST_TEST(list.size() == 3);
    BOOST_TEST(to_string(list) == "1 2 3");
  }
}

BOOST_AUTO_TEST_CASE(fill)
{
  {
    zholobov::CircularFwdList< int > list(0);
    BOOST_TEST(list.size() == 0);
    BOOST_TEST(to_string(list) == "");
  }
  {
    zholobov::CircularFwdList< int > list(5);
    BOOST_TEST(list.size() == 5);
    BOOST_TEST(to_string(list) == "0 0 0 0 0");
  }
  {
    zholobov::CircularFwdList< int > list(0, 2);
    BOOST_TEST(list.size() == 0);
    BOOST_TEST(to_string(list) == "");
  }
  {
    zholobov::CircularFwdList< int > list(5, 2);
    BOOST_TEST(list.size() == 5);
    BOOST_TEST(to_string(list) == "2 2 2 2 2");
  }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(CircularFwsList_Access)

BOOST_AUTO_TEST_CASE(front)
{
  zholobov::CircularFwdList< int > list{1, 2, 3};
  BOOST_TEST(list.front() == 1);
}

BOOST_AUTO_TEST_CASE(back)
{
  zholobov::CircularFwdList< int > list{1, 2, 3};
  BOOST_TEST(list.back() == 3);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(CircularFwsList_Capacity)

BOOST_AUTO_TEST_CASE(empty)
{
  {
    zholobov::CircularFwdList< int > list{};
    BOOST_TEST(list.empty() == true);
  }
  {
    zholobov::CircularFwdList< int > list{1, 2, 3};
    BOOST_TEST(list.empty() == false);
  }
}

BOOST_AUTO_TEST_CASE(size)
{
  {
    zholobov::CircularFwdList< int > list{};
    BOOST_TEST(list.size() == 0);
  }
  {
    zholobov::CircularFwdList< int > list{1, 2, 3};
    BOOST_TEST(list.size() == 3);
  }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(CircularFwsList_Modifiers)

BOOST_AUTO_TEST_CASE(push_pop_front)
{
  zholobov::CircularFwdList< int > list;
  list.push_front(10);
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(to_string(list) == "10");
  list.push_front(11);
  BOOST_TEST(list.size() == 2);
  BOOST_TEST(to_string(list) == "11 10");
  list.pop_front();
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(to_string(list) == "10");
  list.pop_front();
  BOOST_TEST(list.size() == 0);
  BOOST_TEST(to_string(list) == "");
}

BOOST_AUTO_TEST_CASE(push_pop_back)
{
  zholobov::CircularFwdList< int > list;
  list.push_back(10);
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(to_string(list) == "10");
  list.push_back(11);
  BOOST_TEST(list.size() == 2);
  BOOST_TEST(to_string(list) == "10 11");
  list.pop_back();
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(to_string(list) == "10");
  list.pop_back();
  BOOST_TEST(list.size() == 0);
  BOOST_TEST(to_string(list) == "");
}

BOOST_AUTO_TEST_CASE(swap)
{
  zholobov::CircularFwdList< int > list1{1, 2, 3};
  zholobov::CircularFwdList< int > list2{4, 5, 6, 7, 8};
  list1.swap(list2);
  BOOST_TEST(list1.size() == 5);
  BOOST_TEST(list2.size() == 3);
  BOOST_TEST(to_string(list1) == "4 5 6 7 8");
  BOOST_TEST(to_string(list2) == "1 2 3");
}

BOOST_AUTO_TEST_CASE(clear)
{
  zholobov::CircularFwdList< int > list{1, 2, 3};
  list.clear();
  BOOST_TEST(list.size() == 0);
  BOOST_TEST(to_string(list) == "");
}

BOOST_AUTO_TEST_CASE(assign)
{
  zholobov::CircularFwdList< int > list{1, 2, 3};
  list.assign(5, 1);
  BOOST_TEST(to_string(list) == "1 1 1 1 1");
}

BOOST_AUTO_TEST_CASE(opeartor_assign)
{
  {
    zholobov::CircularFwdList< int > list{1, 2, 3};
    zholobov::CircularFwdList< int > list2{};
    list = list2;
    BOOST_TEST(to_string(list) == "");
    BOOST_TEST(to_string(list2) == "");
  }
  {
    zholobov::CircularFwdList< int > list{};
    zholobov::CircularFwdList< int > list2{1, 2, 3};
    list = list2;
    BOOST_TEST(to_string(list) == "1 2 3");
    BOOST_TEST(to_string(list2) == "1 2 3");
  }
  {
    zholobov::CircularFwdList< int > list{1, 2, 3};
    zholobov::CircularFwdList< int > list2{10, 20, 30, 40};
    list = list2;
    BOOST_TEST(to_string(list) == "10 20 30 40");
    BOOST_TEST(to_string(list2) == "10 20 30 40");
  }
}

BOOST_AUTO_TEST_CASE(opeartor_move_assign)
{
  {
    zholobov::CircularFwdList< int > list{1, 2, 3};
    zholobov::CircularFwdList< int > list2{};
    list = std::move(list2);
    BOOST_TEST(to_string(list) == "");
    BOOST_TEST(to_string(list2) == "");
  }
  {
    zholobov::CircularFwdList< int > list{};
    zholobov::CircularFwdList< int > list2{1, 2, 3};
    list = std::move(list2);
    BOOST_TEST(to_string(list) == "1 2 3");
    BOOST_TEST(to_string(list2) == "");
  }
  {
    zholobov::CircularFwdList< int > list{1, 2, 3};
    zholobov::CircularFwdList< int > list2{10, 20, 30, 40};
    list = std::move(list2);
    BOOST_TEST(to_string(list) == "10 20 30 40");
    BOOST_TEST(to_string(list2) == "");
  }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(CircularFwsList_Operations)

BOOST_AUTO_TEST_CASE(remove)
{
  zholobov::CircularFwdList< int > list{1, 2, 3, 2, 5, 6, 7, 2};
  list.remove(2);
  BOOST_TEST(to_string(list) == "1 3 5 6 7");
}

BOOST_AUTO_TEST_CASE(remove_if)
{
  zholobov::CircularFwdList< int > list{1, 2, 3, 2, 5, 6, 7, 2, 9, 0};
  list.remove_if([](int value) { return value <= 2; });
  BOOST_TEST(to_string(list) == "3 5 6 7 9");
}

BOOST_AUTO_TEST_CASE(splice_after)
{
  {
    zholobov::CircularFwdList< int > list1{1, 2, 3};
    zholobov::CircularFwdList< int > list2{10, 20, 30};
    list1.splice_after(list1.cbegin(), list2);
    BOOST_TEST(to_string(list1) == "1 10 20 30 2 3");
    BOOST_TEST(to_string(list2) == "");
  }
  {
    zholobov::CircularFwdList< int > list1{1, 2, 3, 4, 5};
    zholobov::CircularFwdList< int > list2{10, 11, 12};
    list1.splice_after(list1.cbegin(), list2, ++list2.cbegin());
    BOOST_TEST(to_string(list1) == "1 11 2 3 4 5");
    BOOST_TEST(to_string(list2) == "10 12");
  }
  {
    zholobov::CircularFwdList< int > list1{1, 2, 3, 4, 5};
    zholobov::CircularFwdList< int > list2{10, 11, 12};
    list1.splice_after(list1.cbegin(), list2, list2.cbegin(), list2.cend());
    BOOST_TEST(to_string(list1) == "1 11 12 2 3 4 5");
    BOOST_TEST(to_string(list2) == "10");
  }
  {
    zholobov::CircularFwdList< int > list1{1, 2, 3};
    zholobov::CircularFwdList< int > list2{10, 20, 30, 40, 50, 60};
    auto it_first = list2.cbegin();
    ++it_first;
    auto it_last = it_first;
    ++it_last;
    ++it_last;
    ++it_last;
    list1.splice_after(++list1.cbegin(), list2, it_first, it_last);
    BOOST_TEST(to_string(list1) == "1 2 30 40 3");
    BOOST_TEST(to_string(list2) == "10 20 50 60");
  }
  {
    zholobov::CircularFwdList< int > list1{1, 2, 3};
    zholobov::CircularFwdList< int > list2{10, 20, 30, 40, 50, 60};
    auto pos = list1.cbegin();
    ++pos;
    ++pos;
    auto it_first = list2.cbegin();
    ++it_first;
    ++it_first;
    list1.splice_after(pos, list2, it_first, list2.cend());
    BOOST_TEST(to_string(list1) == "1 2 3 40 50 60");
    BOOST_TEST(to_string(list2) == "10 20 30");
  }
}

BOOST_AUTO_TEST_SUITE_END()
