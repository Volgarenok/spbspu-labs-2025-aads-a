#include <boost/test/unit_test.hpp>
#include "forward_list.hpp"
#include <iostream>
#include <stdexcept>

BOOST_AUTO_TEST_CASE(push_back_and_size_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  BOOST_TEST(list.size() == 4);
}

BOOST_AUTO_TEST_CASE(cbegin_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  BOOST_TEST(*list.cbegin() == 1);
}

BOOST_AUTO_TEST_CASE(begin_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  BOOST_TEST(*list.begin() == 1);
}

BOOST_AUTO_TEST_CASE(cend_test)
{
  asafov::Forward_list<size_t> list;
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
  asafov::Forward_list<size_t> list;
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
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  bool test = true;
  try
  {
    test = list.front() == 1;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  BOOST_TEST(test);
}

BOOST_AUTO_TEST_CASE(back_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  bool test = true;
  try
  {
    test = list.back() == 4;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  BOOST_TEST(test);
}

BOOST_AUTO_TEST_CASE(empty_test)
{
  asafov::Forward_list<size_t> list;
  BOOST_TEST(list.empty() == true);
}

BOOST_AUTO_TEST_CASE(nonempty_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  BOOST_TEST(list.empty() == false);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  list.pop_front();
  BOOST_TEST(list.size() == 3);
  bool test = true;
  try
  {
    test = list.front() == 2;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  BOOST_TEST(test);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  list.clear();
  BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(remove_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  list.remove(1);
  BOOST_TEST(list.size() == 3);
  bool test = true;
  try
  {
    test = list.front() == 2;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  BOOST_TEST(test);
}

BOOST_AUTO_TEST_CASE(remove_if_test)
{
  auto f = [](const size_t& a) -> bool
  {
    return a == 1;
  };
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  list.remove_if(f);
  BOOST_TEST(list.size() == 3);
  bool test = true;
  try
  {
    test = list.front() == 2;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  BOOST_TEST(test);
}

BOOST_AUTO_TEST_CASE(assign_test)
{
  asafov::Forward_list<size_t> list;
  list.assign(5, 1);
  BOOST_TEST(list.size() == 5);
  bool test = true;
  try
  {
    test = list.front() == 1;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  BOOST_TEST(test);
  try
  {
    test = list.back() == 1;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  BOOST_TEST(test);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  asafov::Forward_list<size_t> list2;
  list2.push_back(2);
  list.swap(list2);
  bool test = true;
  try
  {
    test = list.front() == 2;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  BOOST_TEST(test);
  try
  {
    test = list2.front() == 1;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  BOOST_TEST(test);
}

