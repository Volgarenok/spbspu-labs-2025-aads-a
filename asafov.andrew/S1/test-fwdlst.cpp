#include <boost/test/unit_test.hpp>
#include <forward_list.hpp>
#include <iostream>
#include <stdexcept>

BOOST_AUTO_TEST_CASE(push_back_and_size_test)
{
  asafov::ForwardList< size_t > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  BOOST_TEST(list.size() == 3);
}

BOOST_AUTO_TEST_CASE(cbegin_test)
{
  asafov::ForwardList< size_t > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  BOOST_TEST(*list.cbegin() == 1);
}

BOOST_AUTO_TEST_CASE(begin_test)
{
  asafov::ForwardList< size_t > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  BOOST_TEST(*list.begin() == 1);
}

BOOST_AUTO_TEST_CASE(cend_test)
{
  asafov::ForwardList< size_t > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  size_t size = 0;
  for (auto it = list.cbegin(); it != list.cend(); ++it, ++size);
  BOOST_TEST(size == 4);
}

BOOST_AUTO_TEST_CASE(end_test)
{
  asafov::ForwardList< size_t > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  size_t size = 0;
  for (auto it = list.begin(); it != list.end(); ++it, ++size);
  BOOST_TEST(size == 4);
}

BOOST_AUTO_TEST_CASE(front_test)
{
  asafov::ForwardList< size_t > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  BOOST_CHECK_EQUAL(list.front(), 1);
}

BOOST_AUTO_TEST_CASE(back_test)
{
  asafov::ForwardList< size_t > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  BOOST_CHECK_EQUAL(list.back(), 3);
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  asafov::ForwardList< size_t > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  asafov::ForwardList< size_t > copy(list);
  BOOST_TEST(copy.size() == 3);
  BOOST_CHECK_EQUAL(copy.front(), 1);
  BOOST_CHECK_EQUAL(copy.back(), 3);
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  asafov::ForwardList< size_t > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  asafov::ForwardList< size_t > moved(std::move(list));
  BOOST_TEST(moved.size() == 3);
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(assignment_operator_lvalue_test)
{
  asafov::ForwardList< size_t > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  asafov::ForwardList< size_t > assigned = list;
  BOOST_TEST(assigned.size() == 3);
  BOOST_CHECK_EQUAL(assigned.front(), 1);
  BOOST_CHECK_EQUAL(assigned.back(), 3);
  list.push_back(4);
  BOOST_TEST(assigned.size() == 3);
}

BOOST_AUTO_TEST_CASE(assignment_operator_rvalue_test)
{
  auto createList = []() -> asafov::ForwardList< size_t >
  {
    asafov::ForwardList< size_t > temp;
    temp.push_back(1);
    temp.push_back(2);
    temp.push_back(3);
    return temp;
  };

  asafov::ForwardList< size_t > assigned = createList();
  BOOST_TEST(assigned.size() == 3);
  BOOST_CHECK_EQUAL(assigned.front(), 1);
  BOOST_CHECK_EQUAL(assigned.back(), 3);
}

BOOST_AUTO_TEST_CASE(empty_test)
{
  asafov::ForwardList< size_t > list;
  BOOST_TEST(list.empty() == true);
}

BOOST_AUTO_TEST_CASE(nonempty_test)
{
  asafov::ForwardList< size_t > list;
  list.push_back(1);
  BOOST_TEST(list.empty() == false);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  asafov::ForwardList< size_t > list;
  list.push_back(1);
  list.push_back(2);
  list.pop_front();
  BOOST_TEST(list.size() == 1);
  BOOST_CHECK_EQUAL(list.front(), 2);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  asafov::ForwardList< size_t > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.clear();
  BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(remove_test)
{
  asafov::ForwardList< size_t > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.remove(2);
  BOOST_TEST(list.size() == 2);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 3);
}

BOOST_AUTO_TEST_CASE(remove_if_test)
{
  auto isOne = [](const size_t& a) -> bool
  {
    return 1 == a;
  };

  asafov::ForwardList< size_t > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.remove_if(isOne);
  BOOST_TEST(list.size() == 2);
  BOOST_CHECK_EQUAL(list.front(), 2);
}

BOOST_AUTO_TEST_CASE(assign_test)
{
  asafov::ForwardList< size_t > list;
  list.assign(5, 1);
  BOOST_TEST(list.size() == 5);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 1);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  asafov::ForwardList< size_t > list;
  list.push_back(1);
  asafov::ForwardList< size_t > list2;
  list2.push_back(2);
  list.swap(list2);
  BOOST_CHECK_EQUAL(list.front(), 2);
  BOOST_CHECK_EQUAL(list2.front(), 1);
}
