#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>
#include <sstream>
#include "forward_ring_list.hpp"

BOOST_AUTO_TEST_SUITE(forward_ring_list_iterator)


BOOST_AUTO_TEST_SUITE(comparison_operators)

BOOST_AUTO_TEST_CASE(not_equal)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list;
  fwd_list.push_front(1);
  fwd_list.push_front(2);
  fwd_list.push_front(3);
  auto it_begin = fwd_list.begin();
  auto it_end = fwd_list.end();
  out << std::boolalpha << (it_begin != it_end);
  BOOST_TEST(out.str() == "true");
}

BOOST_AUTO_TEST_CASE(equal)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list;
  fwd_list.push_front(1);
  fwd_list.push_front(2);
  fwd_list.push_front(3);
  auto it_begin = fwd_list.begin();
  auto it_end = fwd_list.end();
  out << std::boolalpha << (it_begin == (++it_end));
  BOOST_TEST(out.str() == "true");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(prefix_increment)

BOOST_AUTO_TEST_CASE(prefix_increment_one_element)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list;
  fwd_list.push_front(1);
  auto it = fwd_list.begin();
  out << *(++it);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(prefix_increment_two_elements)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list;
  fwd_list.push_front(1);
  fwd_list.push_front(2);
  auto it = fwd_list.begin();
  out << *(++it);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(ring_test)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list;
  fwd_list.push_front(1);
  fwd_list.push_front(2);
  auto it = fwd_list.begin();
  ++it;
  out << *(++it);
  BOOST_TEST(out.str() == "2");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(postfix_increment)

BOOST_AUTO_TEST_CASE(postfix_increment_one_element)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list;
  fwd_list.push_front(1);
  auto it = fwd_list.begin();
  out << *(it++);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(postfix_increment_two_element)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list;
  fwd_list.push_front(1);
  fwd_list.push_front(2);
  auto it = fwd_list.begin();
  out << *(it++);
  BOOST_TEST(out.str() == "2");
}

BOOST_AUTO_TEST_CASE(ring_test)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list;
  fwd_list.push_front(1);
  fwd_list.push_front(2);
  auto it = fwd_list.begin();
  it++;
  it++;
  out << *it;
  BOOST_TEST(out.str() == "2");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE_END()




BOOST_AUTO_TEST_SUITE(forward_ring_list)


BOOST_AUTO_TEST_SUITE(push_front_method)

BOOST_AUTO_TEST_CASE(push_front_one_element)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list;
  fwd_list.push_front(1);
  out << *fwd_list.begin();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(push_front_two_elements)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list;
  fwd_list.push_front(1);
  fwd_list.push_front(2);
  out << *fwd_list.begin() << " " << *fwd_list.end();
  BOOST_TEST(out.str() == "2 1");
}

BOOST_AUTO_TEST_CASE(push_front_many_elements)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list;
  for (size_t i = 1; i <= 10; i++)
  {
    fwd_list.push_front(i);
  }
  for (auto it = fwd_list.begin(); it != fwd_list.end(); ++it)
  {
    out << *it << " ";
  }
  out << *fwd_list.end();
  BOOST_TEST(out.str() == "10 9 8 7 6 5 4 3 2 1");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(reverse_method)

BOOST_AUTO_TEST_CASE(reverse_empty_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  fwd_list.reverse();
  out << fwd_list.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(reverse_list_with_one_element)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  fwd_list.push_front(1);
  fwd_list.reverse();
  out << *fwd_list.begin() << " " << *fwd_list.end();
  BOOST_TEST(out.str() == "1 1");
}

BOOST_AUTO_TEST_CASE(reverse_list_with_two_elements)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  fwd_list.push_front(1);
  fwd_list.push_front(2);
  fwd_list.reverse();
  out << *fwd_list.begin() << " " << *fwd_list.end();
  BOOST_TEST(out.str() == "1 2");
}

BOOST_AUTO_TEST_CASE(reverse_list_with_three_elements)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  for (size_t i = 1; i <= 3; i++)
  {
    fwd_list.push_front(i);
  }
  fwd_list.reverse();
  for (auto it = fwd_list.begin(); it != fwd_list.end(); ++it)
  {
    out << *it << " ";
  }
  out << *fwd_list.end();
  BOOST_TEST(out.str() == "1 2 3");
}

BOOST_AUTO_TEST_CASE(reverse_list_with_many_elements)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  for (size_t i = 1; i <= 10; i++)
  {
    fwd_list.push_front(i);
  }
  fwd_list.reverse();
  for (auto it = fwd_list.begin(); it != fwd_list.end(); ++it)
  {
    out << *it << " ";
  }
  out << *fwd_list.end();
  BOOST_TEST(out.str() == "1 2 3 4 5 6 7 8 9 10");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(copy_constuctor)

BOOST_AUTO_TEST_CASE(make_list_from_other_which_is_empty)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list;
  petrov::ForwardRingList< int > second_fwd_list(first_fwd_list);
  out << second_fwd_list.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(make_list_from_other_with_one_element)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list;
  first_fwd_list.push_front(1);
  petrov::ForwardRingList< int > second_fwd_list(first_fwd_list);
  out << *second_fwd_list.begin();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(make_list_from_other_with_two_elements)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list;
  first_fwd_list.push_front(1);
  first_fwd_list.push_front(2);
  petrov::ForwardRingList< int > second_fwd_list(first_fwd_list);
  out << *second_fwd_list.begin() << " " << *second_fwd_list.end();
  BOOST_TEST(out.str() == "2 1");
}

BOOST_AUTO_TEST_CASE(make_list_from_other_with_many_elements)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  for (size_t i = 1; i <= 10; i++)
  {
    first_fwd_list.push_front(i);
  }
  petrov::ForwardRingList< int > second_fwd_list(first_fwd_list);
  for (auto it = second_fwd_list.begin(); it != second_fwd_list.end(); ++it)
  {
    out << *it << " ";
  }
  out << *second_fwd_list.end();
  BOOST_TEST(out.str() == "10 9 8 7 6 5 4 3 2 1");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(clear_method)

BOOST_AUTO_TEST_CASE(clear_empty_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  first_fwd_list.clear();
  out << first_fwd_list.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(clear_list_with_one_elements)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  first_fwd_list.push_front(1);
  first_fwd_list.clear();
  out << first_fwd_list.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(clear_list_with_many_elements)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  for (size_t i = 1; i <= 10; i++)
  {
    first_fwd_list.push_front(i);
  }
  first_fwd_list.clear();
  out << first_fwd_list.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(assign)

BOOST_AUTO_TEST_CASE(assign_empty_list_from_other)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  petrov::ForwardRingList< int > second_fwd_list = {};
  for (size_t i = 1; i <= 10; i++)
  {
    first_fwd_list.push_front(i);
  }
  second_fwd_list = first_fwd_list;
  for (auto it = second_fwd_list.begin(); it != second_fwd_list.end(); ++it)
  {
    out << *it << " ";
  }
  out << *second_fwd_list.end();
  BOOST_TEST(out.str() == "10 9 8 7 6 5 4 3 2 1");
}

BOOST_AUTO_TEST_CASE(assign_one_element_list_from_other)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  petrov::ForwardRingList< int > second_fwd_list = {};
  for (size_t i = 1; i <= 10; i++)
  {
    first_fwd_list.push_front(i);
  }
  second_fwd_list.push_front(1);
  second_fwd_list = first_fwd_list;
  for (auto it = second_fwd_list.begin(); it != second_fwd_list.end(); ++it)
  {
    out << *it << " ";
  }
  out << *second_fwd_list.end();
  BOOST_TEST(out.str() == "10 9 8 7 6 5 4 3 2 1");
}

BOOST_AUTO_TEST_CASE(assign_two_element_list_from_other)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  petrov::ForwardRingList< int > second_fwd_list = {};
  for (size_t i = 1; i <= 10; i++)
  {
    first_fwd_list.push_front(i);
  }
  second_fwd_list.push_front(1);
  second_fwd_list.push_front(2);
  second_fwd_list = first_fwd_list;
  for (auto it = second_fwd_list.begin(); it != second_fwd_list.end(); ++it)
  {
    out << *it << " ";
  }
  out << *second_fwd_list.end();
  BOOST_TEST(out.str() == "10 9 8 7 6 5 4 3 2 1");
}

BOOST_AUTO_TEST_CASE(assign_many_element_list_from_other)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  petrov::ForwardRingList< int > second_fwd_list = {};
  for (size_t i = 1; i <= 10; i++)
  {
    first_fwd_list.push_front(i);
  }
  for (size_t i = 10; i >= 1; i--)
  {
    second_fwd_list.push_front(i);
  }
  second_fwd_list = first_fwd_list;
  for (auto it = second_fwd_list.begin(); it != second_fwd_list.end(); ++it)
  {
    out << *it << " ";
  }
  out << *second_fwd_list.end();
  BOOST_TEST(out.str() == "10 9 8 7 6 5 4 3 2 1");
}

BOOST_AUTO_TEST_CASE(assign_many_element_list_from_empty_other)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  petrov::ForwardRingList< int > second_fwd_list = {};
  for (size_t i = 10; i >= 1; i--)
  {
    second_fwd_list.push_front(i);
  }
  second_fwd_list = first_fwd_list;
  out << second_fwd_list.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
