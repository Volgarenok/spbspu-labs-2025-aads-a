#include <boost/test/unit_test.hpp>

#include <sstream>

#include "CircularFwdList.hpp"

template < typename T >
std::string to_string(const zholobov::CircularFwdList< T >& list)
{
  std::stringstream ss;
  if (list.size() != 0) {
    auto it = list.begin();
    ss << *it++;
    for (; it != list.end(); ++it) {
      ss << " " << *it;
    }
  }
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

BOOST_AUTO_TEST_CASE(range)
{
  std::vector< int > vec{1, 2, 3, 10, 11, 12};
  auto it1 = vec.begin() + 2;
  auto it2 = vec.end() - 1;
  zholobov::CircularFwdList< int > list(it1, it2);
  BOOST_TEST(to_string(list) == "3 10 11");
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

BOOST_AUTO_TEST_CASE(iterators)
{
  {
    zholobov::CircularFwdList< int > list;
    auto it = list.before_begin();
    bool is_equal = (++it == list.begin());
    BOOST_TEST(is_equal);
    is_equal = (list.begin() == list.end());
    BOOST_TEST(is_equal);
  }
  {
    zholobov::CircularFwdList< int > list{1, 2, 3};
    auto it = list.before_begin();
    bool is_equal = (++it == list.begin());
    BOOST_TEST(is_equal);
    is_equal = (list.begin() == list.end());
    BOOST_TEST(!is_equal);
  }
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
  {
    zholobov::CircularFwdList< int > list{1, 2, 3};
    list.assign(5, 1);
    BOOST_TEST(to_string(list) == "1 1 1 1 1");
  }
  {
    std::vector< int > vec{1, 2, 3, 10, 11, 12};
    auto it1 = vec.begin() + 2;
    auto it2 = vec.end() - 1;
    zholobov::CircularFwdList< int > list{40, 50, 60, 70};
    list.assign(it1, it2);
    BOOST_TEST(to_string(list) == "3 10 11");
  }
  {
    zholobov::CircularFwdList< int > list{40, 50, 60, 70};
    list.assign({3, 7, 5, 20});
    BOOST_TEST(to_string(list) == "3 7 5 20");
  }
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
  {
    zholobov::CircularFwdList< int > list{1, 2, 3};
    list = {10, 20, 30, 40};
    BOOST_TEST(to_string(list) == "10 20 30 40");
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

BOOST_AUTO_TEST_CASE(insert_after)
{
  {
    zholobov::CircularFwdList< int > list{1, 2, 3, 10, 11, 12};
    auto pos = list.cbegin();
    std::advance(pos, 3);
    int value = 42;
    auto result_it = list.insert_after(pos, value);
    BOOST_TEST(to_string(list) == "1 2 3 10 42 11 12");
    BOOST_TEST(*result_it == 42);
  }
  {
    zholobov::CircularFwdList< int > list{1, 2, 3, 10, 11, 12};
    auto pos = list.cbefore_begin();
    int value = 42;
    auto result_it = list.insert_after(pos, value);
    BOOST_TEST(to_string(list) == "42 1 2 3 10 11 12");
    BOOST_TEST(*result_it == 42);
  }

  {
    zholobov::CircularFwdList< std::string > list{"1", "2", "3", "10", "11", "12"};
    auto pos = list.cbegin();
    std::advance(pos, 3);
    std::string value = "value";
    auto result_it = list.insert_after(pos, std::move(value));
    BOOST_TEST(to_string(list) == "1 2 3 10 value 11 12");
    BOOST_TEST(value == "");
    BOOST_TEST(*result_it == "value");
  }
  {
    zholobov::CircularFwdList< std::string > list{"1", "2", "3", "10", "11", "12"};
    auto pos = list.cbefore_begin();
    std::string value = "value";
    auto result_it = list.insert_after(pos, std::move(value));
    BOOST_TEST(to_string(list) == "value 1 2 3 10 11 12");
    BOOST_TEST(value == "");
    BOOST_TEST(*result_it == "value");
  }

  {
    zholobov::CircularFwdList< int > list{1, 2, 3, 10, 11, 12};
    auto pos = list.cbegin();
    std::advance(pos, 3);
    auto result_it = list.insert_after(pos, 5, 42);
    BOOST_TEST(to_string(list) == "1 2 3 10 42 42 42 42 42 11 12");
    BOOST_TEST(*result_it == 42);
  }
  {
    zholobov::CircularFwdList< int > list{1, 2, 3, 10, 11, 12};
    auto pos = list.cbefore_begin();
    auto result_it = list.insert_after(pos, 5, 42);
    BOOST_TEST(to_string(list) == "42 42 42 42 42 1 2 3 10 11 12");
    BOOST_TEST(*result_it == 42);
  }

  {
    zholobov::CircularFwdList< int > list{1, 2, 3, 10, 11, 12};
    auto pos = list.cbegin();
    std::advance(pos, 3);
    auto result_it = list.insert_after(pos, {50, 60, 70});
    BOOST_TEST(to_string(list) == "1 2 3 10 50 60 70 11 12");
    BOOST_TEST(*result_it == 70);
  }
  {
    zholobov::CircularFwdList< int > list{1, 2, 3, 10, 11, 12};
    auto pos = list.cbefore_begin();
    auto result_it = list.insert_after(pos, {50, 60, 70});
    BOOST_TEST(to_string(list) == "50 60 70 1 2 3 10 11 12");
    BOOST_TEST(*result_it == 70);
  }

  {
    std::vector< int > vec{20, 21, 22, 23, 24, 25, 26};
    auto it1 = vec.begin() + 2;
    auto it2 = vec.end() - 2;
    zholobov::CircularFwdList< int > list{1, 2, 3, 10, 11, 12};
    auto pos = list.cbegin();
    std::advance(pos, 3);
    auto result_it = list.insert_after(pos, it1, it2);
    BOOST_TEST(to_string(list) == "1 2 3 10 22 23 24 11 12");
    BOOST_TEST(*result_it == 24);
  }
  {
    std::vector< int > vec{20, 21, 22, 23, 24, 25, 26};
    auto it1 = vec.begin() + 2;
    auto it2 = vec.end() - 2;
    zholobov::CircularFwdList< int > list{1, 2, 3, 10, 11, 12};
    auto pos = list.cbefore_begin();
    auto result_it = list.insert_after(pos, it1, it2);
    BOOST_TEST(to_string(list) == "22 23 24 1 2 3 10 11 12");
    BOOST_TEST(*result_it == 24);
  }
}

BOOST_AUTO_TEST_CASE(erase_after)
{
  {
    zholobov::CircularFwdList< int > list{1, 2, 3, 10, 11, 12};
    auto pos = list.cbegin();
    std::advance(pos, 3);
    auto result_it = list.erase_after(pos);
    BOOST_TEST(to_string(list) == "1 2 3 10 12");
    BOOST_TEST(*result_it == 12);
  }
  {
    zholobov::CircularFwdList< int > list{1, 2, 3, 10, 11, 12};
    auto pos = list.cbefore_begin();
    auto result_it = list.erase_after(pos);
    BOOST_TEST(to_string(list) == "2 3 10 11 12");
    BOOST_TEST(*result_it == 2);
  }

  {
    zholobov::CircularFwdList< int > list{1, 2, 3, 10, 11, 12};
    auto pos = list.cbegin();
    std::advance(pos, 2);
    auto last = list.cbegin();
    std::advance(last, 5);
    auto result_it = list.erase_after(pos, last);
    BOOST_TEST(to_string(list) == "1 2 3 12");
    BOOST_TEST(*result_it == 12);
  }
  {
    zholobov::CircularFwdList< int > list{1, 2, 3, 10, 11, 12};
    auto pos = list.cbefore_begin();
    auto last = list.cbegin();
    std::advance(last, 5);
    auto result_it = list.erase_after(pos, last);
    BOOST_TEST(to_string(list) == "12");
    BOOST_TEST(*result_it == 12);
  }
}

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
    auto it_last = ++it_first;
    std::advance(it_last, 3);
    list1.splice_after(++list1.cbegin(), list2, it_first, it_last);
    BOOST_TEST(to_string(list1) == "1 2 30 40 3");
    BOOST_TEST(to_string(list2) == "10 20 50 60");
  }
  {
    zholobov::CircularFwdList< int > list1{1, 2, 3};
    zholobov::CircularFwdList< int > list2{10, 20, 30, 40, 50, 60};
    auto pos = list1.cbegin();
    std::advance(pos, 2);
    auto it_first = list2.cbegin();
    std::advance(it_first, 2);
    list1.splice_after(pos, list2, it_first, list2.cend());
    BOOST_TEST(to_string(list1) == "1 2 3 40 50 60");
    BOOST_TEST(to_string(list2) == "10 20 30");
  }
}

BOOST_AUTO_TEST_SUITE_END()
