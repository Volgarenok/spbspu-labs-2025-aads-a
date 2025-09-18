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
  for (int i = 1; i <= 10; i++)
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
  for (int i = 1; i <= 3; i++)
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
  for (int i = 1; i <= 10; i++)
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
  for (int i = 1; i <= 10; i++)
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
  for (int i = 1; i <= 10; i++)
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
  for (int i = 1; i <= 10; i++)
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
  for (int i = 1; i <= 10; i++)
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
  for (int i = 1; i <= 10; i++)
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
  for (int i = 1; i <= 10; i++)
  {
    first_fwd_list.push_front(i);
  }
  for (int i = 10; i >= 1; i--)
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
  for (int i = 10; i >= 1; i--)
  {
    second_fwd_list.push_front(i);
  }
  second_fwd_list = first_fwd_list;
  out << second_fwd_list.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(pop_front_method)

BOOST_AUTO_TEST_CASE(pop_front_empty_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  fwd_list.pop_front();
  out << fwd_list.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(pop_front_one_element_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  fwd_list.push_front(1);
  fwd_list.pop_front();
  out << fwd_list.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(pop_front_two_element_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  fwd_list.push_front(1);
  fwd_list.push_front(2);
  fwd_list.pop_front();
  auto it = fwd_list.cbegin();
  out << *it << " " << *(++it);
  BOOST_TEST(out.str() == "1 1");
}

BOOST_AUTO_TEST_CASE(pop_front_three_element_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  fwd_list.push_front(1);
  fwd_list.push_front(2);
  fwd_list.push_front(3);
  fwd_list.pop_front();
  auto it = fwd_list.cbegin();
  out << *it << " " << *(++it) << " " <<  *(++it);
  BOOST_TEST(out.str() == "2 1 2");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(remove_method)

BOOST_AUTO_TEST_CASE(remove_element_from_empty_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  fwd_list.remove(1);
  out << fwd_list.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(remove_element_from_one_element_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  fwd_list.push_front(1);
  fwd_list.remove(1);
  out << fwd_list.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(remove_element_from_list_filled_with_it)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  for (int i = 0; i < 10; i++)
  {
    fwd_list.push_front(1);
  }
  fwd_list.remove(1);
  out << fwd_list.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(remove_element_from_list_which_are_not_there)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  for (int i = 1; i <= 10; i++)
  {
    fwd_list.push_front(i);
  }
  fwd_list.remove(11);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  BOOST_TEST(out.str() == "10 9 8 7 6 5 4 3 2 1");
}

BOOST_AUTO_TEST_CASE(remove_element_from_start_of_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  for (int i = 1; i <= 10; i++)
  {
    fwd_list.push_front(i);
  }
  fwd_list.remove(10);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it;
  BOOST_TEST(out.str() == "9 8 7 6 5 4 3 2 1 9");
}

BOOST_AUTO_TEST_CASE(remove_element_from_end_of_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  for (int i = 1; i <= 10; i++)
  {
    fwd_list.push_front(i);
  }
  fwd_list.remove(1);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it;
  BOOST_TEST(out.str() == "10 9 8 7 6 5 4 3 2 10");
}

BOOST_AUTO_TEST_CASE(remove_elements_from_list_which_are_there)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  for (int i = 1; i <= 10; i++)
  {
    fwd_list.push_front(i);
  }
  fwd_list.remove(1);
  fwd_list.remove(10);
  fwd_list.remove(7);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  BOOST_TEST(out.str() == "9 8 6 5 4 3 2");
}

BOOST_AUTO_TEST_CASE(remove_elements_from_list_to_make_it_empty)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  for (int i = 1; i <= 3; i++)
  {
    fwd_list.push_front(i);
  }
  for (int i = 1; i <= 3; i++)
  {
    fwd_list.remove(i);
  }
  out << fwd_list.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(are_equal_comparasion_operator)

BOOST_AUTO_TEST_CASE(compare_empty_lists)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  petrov::ForwardRingList< int > second_fwd_list = {};
  out << (first_fwd_list == second_fwd_list);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(compare_empty_list_with_not_empty)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  petrov::ForwardRingList< int > second_fwd_list = {};
  second_fwd_list.push_front(1);
  out << (first_fwd_list == second_fwd_list);
  BOOST_TEST(out.str() == "0");
}

BOOST_AUTO_TEST_CASE(compare_two_equal_not_empty_lists)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  petrov::ForwardRingList< int > second_fwd_list = {};
  for (int i = 1; i <= 3; i++)
  {
    first_fwd_list.push_front(i);
    second_fwd_list.push_front(i);
  }
  out << (first_fwd_list == second_fwd_list);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(compare_two_not_equal_not_empty_lists_different_size)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  petrov::ForwardRingList< int > second_fwd_list = {};
  for (int i = 1; i <= 3; i++)
  {
    first_fwd_list.push_front(i);
    second_fwd_list.push_front(4 - i);
  }
  second_fwd_list.pop_front();
  out << (first_fwd_list == second_fwd_list);
  BOOST_TEST(out.str() == "0");
}

BOOST_AUTO_TEST_CASE(compare_two_not_equal_not_empty_lists_one_size)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  petrov::ForwardRingList< int > second_fwd_list = {};
  for (int i = 1; i <= 3; i++)
  {
    first_fwd_list.push_front(i);
    second_fwd_list.push_front(4 - i);
  }
  out << (first_fwd_list == second_fwd_list);
  BOOST_TEST(out.str() == "0");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(are_not_equal_comparasion_operator)

BOOST_AUTO_TEST_CASE(compare_empty_lists)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  petrov::ForwardRingList< int > second_fwd_list = {};
  out << (first_fwd_list != second_fwd_list);
  BOOST_TEST(out.str() == "0");
}

BOOST_AUTO_TEST_CASE(compare_empty_list_with_not_empty)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  petrov::ForwardRingList< int > second_fwd_list = {};
  second_fwd_list.push_front(1);
  out << (first_fwd_list != second_fwd_list);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(compare_two_equal_not_empty_lists)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  petrov::ForwardRingList< int > second_fwd_list = {};
  for (int i = 1; i <= 3; i++)
  {
    first_fwd_list.push_front(i);
    second_fwd_list.push_front(i);
  }
  out << (first_fwd_list != second_fwd_list);
  BOOST_TEST(out.str() == "0");
}

BOOST_AUTO_TEST_CASE(compare_two_not_equal_not_empty_lists_different_size)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  petrov::ForwardRingList< int > second_fwd_list = {};
  for (int i = 1; i <= 3; i++)
  {
    first_fwd_list.push_front(i);
    second_fwd_list.push_front(4 - i);
  }
  second_fwd_list.pop_front();
  out << (first_fwd_list != second_fwd_list);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(compare_two_not_equal_not_empty_lists_one_size)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first_fwd_list = {};
  petrov::ForwardRingList< int > second_fwd_list = {};
  for (int i = 1; i <= 3; i++)
  {
    first_fwd_list.push_front(i);
    second_fwd_list.push_front(4 - i);
  }
  out << (first_fwd_list != second_fwd_list);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(remove_if_method)

bool isEqualToOne(int number)
{
  return number == 1;
}

bool isEqualToFive(int number)
{
  return number == 5;
}

bool isEqualToSeven(int number)
{
  return number == 7;
}

BOOST_AUTO_TEST_CASE(remove_if_element_from_empty_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  fwd_list.remove_if(isEqualToOne);
  out << fwd_list.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(remove_if_element_from_one_element_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  fwd_list.push_front(1);
  fwd_list.remove_if(isEqualToOne);
  out << fwd_list.empty();
  BOOST_TEST(out.str() == "1");
}

bool isLessThanTen(int number)
{
  return number < 10;
}

BOOST_AUTO_TEST_CASE(remove_if_element_from_list_filled_with_it)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  for (int i = 0; i < 10; i++)
  {
    fwd_list.push_front(1);
  }
  fwd_list.remove_if(isLessThanTen);
  out << fwd_list.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(remove_if_element_from_list_which_are_not_there)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  for (int i = 2; i <= 10; i++)
  {
    fwd_list.push_front(i);
  }
  fwd_list.remove_if(isEqualToOne);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  BOOST_TEST(out.str() == "10 9 8 7 6 5 4 3 2");
}

BOOST_AUTO_TEST_CASE(remove_if_element_from_start_of_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  for (int i = 1; i <= 10; i++)
  {
    fwd_list.push_front(i);
  }
  fwd_list.reverse();
  fwd_list.remove_if(isEqualToOne);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  BOOST_TEST(out.str() == "2 3 4 5 6 7 8 9 10");
}

BOOST_AUTO_TEST_CASE(remove_if_element_from_end_of_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  for (int i = 1; i <= 10; i++)
  {
    fwd_list.push_front(i);
  }
  fwd_list.remove_if(isEqualToOne);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  BOOST_TEST(out.str() == "10 9 8 7 6 5 4 3 2");
}

BOOST_AUTO_TEST_CASE(remove_if_elements_from_list_which_are_there)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  for (int i = 1; i <= 10; i++)
  {
    fwd_list.push_front(i);
  }
  fwd_list.remove_if(isEqualToFive);
  fwd_list.remove_if(isEqualToOne);
  fwd_list.remove_if(isEqualToSeven);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  BOOST_TEST(out.str() == "10 9 8 6 4 3 2");
}

BOOST_AUTO_TEST_CASE(remove_if_elements_from_list_to_make_it_empty)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list = {};
  fwd_list.push_front(1);
  fwd_list.push_front(5);
  fwd_list.push_front(7);
  fwd_list.remove_if(isEqualToFive);
  fwd_list.remove_if(isEqualToSeven);
  fwd_list.remove_if(isEqualToOne);
  out << fwd_list.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(fill_constructor)

BOOST_AUTO_TEST_CASE(fill_with_zero_element)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(0, 1);
  out << fwd_list.empty() << " " << fwd_list.size();
  BOOST_TEST(out.str() == "1 0");
}

BOOST_AUTO_TEST_CASE(fill_with_one_element)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(1, 1);
  auto it = fwd_list.cbegin();
  out << *(it++);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(fill_with_three_elements)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(3, 3);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  BOOST_TEST(out.str() == "3 3 3");
}

BOOST_AUTO_TEST_CASE(fill_with_five_elements)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(5, 5);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  BOOST_TEST(out.str() == "5 5 5 5 5");
}

BOOST_AUTO_TEST_CASE(fill_with_seven_elements)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(7, 7);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  BOOST_TEST(out.str() == "7 7 7 7 7 7 7");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(splice_method_all_list)

BOOST_AUTO_TEST_CASE(splice_empty_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(7, 7);
  petrov::ForwardRingList< int > second;
  auto pos = first.cbegin();
  for (size_t i = 0; i < 6; i++)
  {
    ++pos;
  }
  first.splice(pos, second);
  auto it = first.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != first.cend());
  out << " " << second.empty();
  BOOST_TEST(out.str() == "7 7 7 7 7 7 7 1");
}

BOOST_AUTO_TEST_CASE(splice_to_one_element_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(1, 1);
  petrov::ForwardRingList< int > second(3, 3);
  auto pos = first.cbegin();
  first.splice(pos, second);
  auto it = first.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != first.cend());
  out << " " << *it;
  out << " " << second.empty();
  BOOST_TEST(out.str() == "1 3 3 3 1 1");
}

BOOST_AUTO_TEST_CASE(splice_not_empty_list_after_middle)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(7, 7);
  petrov::ForwardRingList< int > second(3, 3);
  auto pos = first.cbegin();
  for (size_t i = 0; i < 3; i++)
  {
    ++pos;
  }
  first.splice(pos, second);
  auto it = first.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != first.cend());
  out << " " << second.empty();
  BOOST_TEST(out.str() == "7 7 7 7 3 3 3 7 7 7 1");
}

BOOST_AUTO_TEST_CASE(splice_not_empty_list_after_head)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(7, 7);
  petrov::ForwardRingList< int > second(3, 3);
  auto pos = first.cbegin();
  first.splice(pos, second);
  auto it = first.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != first.cend());
  out << " " << second.empty();
  BOOST_TEST(out.str() == "7 3 3 3 7 7 7 7 7 7 1");
}

BOOST_AUTO_TEST_CASE(splice_not_empty_list_after_tail)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(7, 7);
  first.push_front(5);
  petrov::ForwardRingList< int > second(3, 3);
  auto pos = first.cend();
  first.splice(pos, second);
  auto it = first.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != first.cend());
  out << " " << *it;
  out << " " << second.empty() << " " << first.size();
  BOOST_TEST(out.str() == "5 7 7 7 7 7 7 7 3 3 3 5 1 11");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(splice_method_one_node)

BOOST_AUTO_TEST_CASE(splice_node_to_middle)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(7, 7);
  petrov::ForwardRingList< int > second;
  for (int i = 1; i <= 3; i++)
  {
    second.push_front(i);
  }
  second.reverse();
  auto pos = first.cbegin();
  ++pos;
  auto it = second.cbegin();
  first.splice(pos, second, it);
  auto it_1 = first.cbegin();
  out << *(it_1++);
  do
  {
    out << " " << *it_1;
  }
  while (it_1++ != first.cend());
  auto it_2 = second.cbegin();
  out << " " << *(it_2++);
  do
  {
    out << " " << *it_2;
  }
  while (it_2++ != second.cend());
  BOOST_TEST(out.str() == "7 7 2 7 7 7 7 7 1 3");
}

BOOST_AUTO_TEST_CASE(splice_to_one_element_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(1, 1);
  petrov::ForwardRingList< int > second;
  for (int i = 1; i <= 3; i++)
  {
    second.push_front(i);
  }
  second.reverse();
  auto pos = first.cbegin();
  auto it = second.cbegin();
  first.splice(pos, second, it);
  auto it_1 = first.cbegin();
  out << *(it_1++);
  do
  {
    out << " " << *it_1;
  }
  while (it_1++ != first.cend());
  out << " " << *it_1;
  auto it_2 = second.cbegin();
  out << " " << *(it_2++);
  do
  {
    out << " " << *it_2;
  }
  while (it_2++ != second.cend());
  BOOST_TEST(out.str() == "1 2 1 1 3");
}

BOOST_AUTO_TEST_CASE(splice_node_after_head)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(7, 7);
  petrov::ForwardRingList< int > second;
  for (int i = 1; i <= 3; i++)
  {
    second.push_front(i);
  }
  second.reverse();
  auto pos = first.cbegin();
  auto it = second.cbegin();
  first.splice(pos, second, it);
  auto it_1 = first.cbegin();
  out << *(it_1++);
  do
  {
    out << " " << *it_1;
  }
  while (it_1++ != first.cend());
  auto it_2 = second.cbegin();
  out << " " << *(it_2++);
  do
  {
    out << " " << *it_2;
  }
  while (it_2++ != second.cend());
  BOOST_TEST(out.str() == "7 2 7 7 7 7 7 7 1 3");
}

BOOST_AUTO_TEST_CASE(splice_node_after_tail)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(7, 7);
  first.push_front(5);
  petrov::ForwardRingList< int > second;
  for (int i = 1; i <= 3; i++)
  {
    second.push_front(i);
  }
  second.reverse();
  auto pos = first.cbegin();
  for (size_t i = 0; i < 7; i++)
  {
    ++pos;
  }
  auto it = second.cbegin();
  first.splice(pos, first, it);
  auto it_1 = first.cbegin();
  out << *(it_1++);
  do
  {
    out << " " << *it_1;
  }
  while (it_1++ != first.cend());
  out << " " << *it_1;
  auto it_2 = second.cbegin();
  out << " " << *(it_2++);
  do
  {
    out << " " << *it_2;
  }
  while (it_2++ != second.cend());
  BOOST_TEST(out.str() == "5 7 7 7 7 7 7 7 2 5 1 3");
}

BOOST_AUTO_TEST_CASE(splice_node_from_tail)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(3, 4);
  petrov::ForwardRingList< int > second;
  for (int i = 1; i <= 3; i++)
  {
    second.push_front(i);
  }
  second.reverse();
  auto pos = first.cbegin();
  auto it = second.cbegin();
  ++it;
  first.splice(pos, second, it);
  auto it_1 = first.cbegin();
  out << *(it_1++);
  do
  {
    out << " " << *it_1;
  }
  while (it_1++ != first.cend());
  auto it_2 = second.cbegin();
  out << " " << *(it_2++);
  do
  {
    out << " " << *it_2;
  }
  while (it_2++ != second.cend());
  out << " " << *it_2;
  BOOST_TEST(out.str() == "4 3 4 4 1 2 1");
}

BOOST_AUTO_TEST_CASE(splice_node_from_head)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(3, 4);
  petrov::ForwardRingList< int > second;
  for (int i = 1; i <= 3; i++)
  {
    second.push_front(i);
  }
  second.reverse();
  auto pos = first.cbegin();
  auto it = second.cbegin();
  ++it;
  ++it;
  first.splice(pos, second, it);
  auto it_1 = first.cbegin();
  out << *(it_1++);
  do
  {
    out << " " << *it_1;
  }
  while (it_1++ != first.cend());
  auto it_2 = second.cbegin();
  out << " " << *(it_2++);
  do
  {
    out << " " << *it_2;
  }
  while (it_2++ != second.cend());
  out << " " << *it_2;
  BOOST_TEST(out.str() == "4 1 4 4 2 3 2");
}

BOOST_AUTO_TEST_CASE(splice_node_from_one_element_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(3, 4);
  petrov::ForwardRingList< int > second(1, 5);
  second.reverse();
  auto pos = first.cbegin();
  auto it = second.cbegin();
  first.splice(pos, second, it);
  auto it_1 = first.cbegin();
  out << *(it_1++);
  do
  {
    out << " " << *it_1;
  }
  while (it_1++ != first.cend());
  out << " " << second.empty();
  BOOST_TEST(out.str() == "4 5 4 4 1");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(splice_method_range)

BOOST_AUTO_TEST_CASE(first_eq_last_middle)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(1, 1);
  petrov::ForwardRingList< int > second;
  for (int i = 1; i <= 5; i++)
  {
    second.push_front(i);
  }
  second.reverse();
  auto pos = first.cbegin();
  auto it = second.cbegin();
  ++it;
  ++it;
  auto first_it = it;
  auto last_it = it;
  first.splice(pos, second, first_it, last_it);
  auto it_1 = first.cbegin();
  out << *(it_1++);
  do
  {
    out << " " << *it_1;
  }
  while (it_1++ != first.cend());
  out << " " << *it_1;
  auto it_2 = second.cbegin();
  out << " " << *(it_2++) << " " << *(it_2++) << " " << second.size();
  BOOST_TEST(out.str() == "1 4 5 1 2 1 3 3 1");
}

BOOST_AUTO_TEST_CASE(first_eq_last_begin)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(2, 1);
  petrov::ForwardRingList< int > second;
  for (int i = 1; i <= 5; i++)
  {
    second.push_front(i);
  }
  second.reverse();
  auto pos = first.cbegin();
  ++pos;
  auto it = second.cbegin();
  auto first_it = it;
  auto last_it = it;
  first.splice(pos, second, first_it, last_it);
  auto it_1 = first.cbegin();
  out << *(it_1++);
  do
  {
    out << " " << *it_1;
  }
  while (it_1++ != first.cend());
  out << " " << *it_1;
  auto it_2 = second.cbegin();
  out << " " << *(it_2++) << " " << *(it_2++);
  BOOST_TEST(out.str() == "1 1 2 3 4 5 1 1 1");
}

BOOST_AUTO_TEST_CASE(first_eq_last_end)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(3, 10);
  petrov::ForwardRingList< int > second;
  for (int i = 1; i <= 5; i++)
  {
    second.push_front(i);
  }
  second.reverse();
  auto pos = first.cbegin();
  ++pos;
  ++pos;
  auto it = second.cbegin();
  ++it;
  ++it;
  ++it;
  ++it;
  auto first_it = it;
  auto last_it = it;
  first.splice(pos, second, first_it, last_it);
  auto it_1 = first.cbegin();
  out << *(it_1++);
  do
  {
    out << " " << *it_1;
  }
  while (it_1++ != first.cend());
  out << " " << *it_1;
  auto it_2 = second.cbegin();
  out << " " << *(it_2++) << " " << *(it_2++);
  BOOST_TEST(out.str() == "10 10 10 1 2 3 4 10 5 5");
}

BOOST_AUTO_TEST_CASE(splice_one_el_btw)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(3, 10);
  petrov::ForwardRingList< int > second;
  for (int i = 1; i <= 5; i++)
  {
    second.push_front(i);
  }
  second.reverse();
  auto pos = first.cbegin();
  ++pos;
  auto first_it = second.cbegin();
  auto it = second.cbegin();
  ++it;
  ++it;
  auto last_it = it;
  first.splice(pos, second, first_it, last_it);
  auto it_1 = first.cbegin();
  out << *(it_1++);
  do
  {
    out << " " << *it_1;
  }
  while (it_1++ != first.cend());
  out << " " << *it_1;
  auto it_2 = second.cbegin();
  out << " " << *(it_2++);
  do
  {
    out << " " << *it_2;
  }
  while (it_2++ != second.cend());
  out << " " << *it_2;
  BOOST_TEST(out.str() == "10 10 2 10 10 1 3 4 5 1");
}

BOOST_AUTO_TEST_CASE(splice_three_el_btw)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(3, 10);
  petrov::ForwardRingList< int > second;
  for (int i = 1; i <= 5; i++)
  {
    second.push_front(i);
  }
  second.reverse();
  auto pos = first.cbegin();
  ++pos;
  auto first_it = second.cbegin();
  auto last_it = second.cend();
  first.splice(pos, second, first_it, last_it);
  auto it_1 = first.cbegin();
  out << *(it_1++);
  do
  {
    out << " " << *it_1;
  }
  while (it_1++ != first.cend());
  out << " " << *it_1;
  auto it_2 = second.cbegin();
  out << " " << *(it_2++);
  do
  {
    out << " " << *it_2;
  }
  while (it_2++ != second.cend());
  out << " " << *it_2;
  BOOST_TEST(out.str() == "10 10 2 3 4 10 10 1 5 1");
}

BOOST_AUTO_TEST_CASE(splice_three_el_end)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(3, 10);
  petrov::ForwardRingList< int > second;
  for (int i = 1; i <= 5; i++)
  {
    second.push_front(i);
  }
  second.reverse();
  auto pos = first.cbegin();
  ++pos;
  auto last_it = second.cbegin();
  auto first_it = second.cbegin();
  ++first_it;
  first.splice(pos, second, first_it, last_it);
  auto it_1 = first.cbegin();
  out << *(it_1++);
  do
  {
    out << " " << *it_1;
  }
  while (it_1++ != first.cend());
  out << " " << *it_1;
  auto it_2 = second.cbegin();
  out << " " << *(it_2++);
  do
  {
    out << " " << *it_2;
  }
  while (it_2++ != second.cend());
  out << " " << *it_2;
  BOOST_TEST(out.str() == "10 10 3 4 5 10 10 1 2 1");
}

BOOST_AUTO_TEST_CASE(splice_three_el_begin)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(3, 10);
  petrov::ForwardRingList< int > second;
  for (int i = 1; i <= 5; i++)
  {
    second.push_front(i);
  }
  second.reverse();
  auto pos = first.cbegin();
  ++pos;
  auto last_it = second.cbegin();
  ++last_it;
  ++last_it;
  ++last_it;
  auto first_it = second.cend();
  first.splice(pos, second, first_it, last_it);
  auto it_1 = first.cbegin();
  out << *(it_1++);
  do
  {
    out << " " << *it_1;
  }
  while (it_1++ != first.cend());
  out << " " << *it_1;
  auto it_2 = second.cbegin();
  out << " " << *(it_2++);
  do
  {
    out << " " << *it_2;
  }
  while (it_2++ != second.cend());
  out << " " << *it_2;
  BOOST_TEST(out.str() == "10 10 1 2 3 10 10 4 5 4");
}

BOOST_AUTO_TEST_CASE(splice_with_cycle_two)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(3, 10);
  petrov::ForwardRingList< int > second;
  for (int i = 1; i <= 5; i++)
  {
    second.push_front(i);
  }
  second.reverse();
  auto pos = first.cbegin();
  ++pos;
  auto last_it = second.cbegin();
  ++last_it;
  auto first_it = second.cbegin();
  ++first_it;
  ++first_it;
  ++first_it;
  first.splice(pos, second, first_it, last_it);
  auto it_1 = first.cbegin();
  out << *(it_1++);
  do
  {
    out << " " << *it_1;
  }
  while (it_1++ != first.cend());
  out << " " << *it_1;
  auto it_2 = second.cbegin();
  out << " " << *(it_2++);
  do
  {
    out << " " << *it_2;
  }
  while (it_2++ != second.cend());
  out << " " << *it_2;
  BOOST_TEST(out.str() == "10 10 5 1 10 10 2 3 4 2");
}

BOOST_AUTO_TEST_CASE(splice_one_btw)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(3, 10);
  petrov::ForwardRingList< int > second;
  for (int i = 1; i <= 5; i++)
  {
    second.push_front(i);
  }
  second.reverse();
  auto pos = first.cbegin();
  ++pos;
  auto first_it = ++second.cbegin();
  auto last_it = second.cbegin();
  ++last_it;
  ++last_it;
  ++last_it;
  first.splice(pos, second, first_it, last_it);
  auto it_1 = first.cbegin();
  out << *(it_1++);
  do
  {
    out << " " << *it_1;
  }
  while (it_1++ != first.cend());
  out << " " << *it_1;
  auto it_2 = second.cbegin();
  out << " " << *(it_2++);
  do
  {
    out << " " << *it_2;
  }
  while (it_2++ != second.cend());
  out << " " << *it_2;
  BOOST_TEST(out.str() == "10 10 3 10 10 1 2 4 5 1");
}

BOOST_AUTO_TEST_CASE(splice_with_cycle_two_size_test)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(3, 10);
  petrov::ForwardRingList< int > second;
  for (int i = 1; i <= 5; i++)
  {
    second.push_front(i);
  }
  second.reverse();
  auto pos = first.cbegin();
  ++pos;
  auto last_it = second.cbegin();
  ++last_it;
  auto first_it = second.cbegin();
  ++first_it;
  ++first_it;
  ++first_it;
  first.splice(pos, second, first_it, last_it);
  out << first.size() << " " << second.size();
  BOOST_TEST(out.str() == "5 3");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(assign_method)

BOOST_AUTO_TEST_CASE(assign_empty)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(5, 5);
  fwd_list.assign(0, 1);
  out << fwd_list.empty() << " " << fwd_list.size();
  BOOST_TEST(out.str() == "1 0");
}

BOOST_AUTO_TEST_CASE(assign_one)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(5, 5);
  fwd_list.assign(1, 1);
  auto it = fwd_list.cbegin();
  out << *(it++) << " " << *(it++) << " " << *it << " " << fwd_list.size();
  BOOST_TEST(out.str() == "1 1 1 1");
}

BOOST_AUTO_TEST_CASE(assign_two)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(5, 5);
  fwd_list.assign(2, 2);
  auto it = fwd_list.cbegin();
  out << *(it++) << " " << *(it++) << " " << *it << " " << fwd_list.size();
  BOOST_TEST(out.str() == "2 2 2 2");
}

BOOST_AUTO_TEST_CASE(assign_five)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(5, 5);
  fwd_list.assign(5, 10);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size();
  BOOST_TEST(out.str() == "10 10 10 10 10 10 5");
}

BOOST_AUTO_TEST_CASE(assign_seven)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(5, 5);
  fwd_list.assign(7, 5);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size();
  BOOST_TEST(out.str() == "5 5 5 5 5 5 5 5 7");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(initializer_list_constructor)

BOOST_AUTO_TEST_CASE(zero_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list {};
  out << fwd_list.empty() << " " << fwd_list.size();
  BOOST_TEST(out.str() == "1 0");
}

BOOST_AUTO_TEST_CASE(one_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 1 };
  auto it = fwd_list.cbegin();
  out << *(it++) << " " << *(it++) << " " << fwd_list.size();
  BOOST_TEST(out.str() == "1 1 1");
}

BOOST_AUTO_TEST_CASE(three_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 1, 2, 3 };
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size();
  BOOST_TEST(out.str() == "1 2 3 1 3");
}

BOOST_AUTO_TEST_CASE(five_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 1, 2, 3, 4, 5 };
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size();
  BOOST_TEST(out.str() == "1 2 3 4 5 1 5");
}

BOOST_AUTO_TEST_CASE(seven_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 1, 2, 3, 4, 5, 6, 7 };
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size();
  BOOST_TEST(out.str() == "1 2 3 4 5 6 7 1 7");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(range_constructor)

BOOST_AUTO_TEST_CASE(zero_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1 };
  auto first_it = first.cbegin();
  auto second_it = first.cend();
  petrov::ForwardRingList< int > second(first_it, second_it);
  out << second.empty() << " " << second.size();
  BOOST_TEST(out.str() == "1 0");
}

BOOST_AUTO_TEST_CASE(one_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2 };
  auto first_it = first.cbegin();
  auto second_it = first.cend();
  petrov::ForwardRingList< int > second(first_it, second_it);
  auto it = second.cbegin();
  out << *(it++) << " " << *(it++) << " " << second.size();
  BOOST_TEST(out.str() == "1 1 1");
}

BOOST_AUTO_TEST_CASE(two_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 3 };
  auto first_it = first.cbegin();
  auto second_it = first.cend();
  petrov::ForwardRingList< int > second(first_it, second_it);
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size();
  auto oth_it = first.cbegin();
  out << " " << *(oth_it++);
  do
  {
    out << " " << *oth_it;
  }
  while (oth_it++ != first.cend());
  out << " " << *oth_it;
  BOOST_TEST(out.str() == "1 2 1 2 1 2 3 1");
}

BOOST_AUTO_TEST_CASE(five_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 3, 4, 5, 6 };
  auto first_it = first.cbegin();
  auto second_it = first.cend();
  petrov::ForwardRingList< int > second(first_it, second_it);
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size();
  auto oth_it = first.cbegin();
  out << " " << *(oth_it++);
  do
  {
    out << " " << *oth_it;
  }
  while (oth_it++ != first.cend());
  out << " " << *oth_it;
  BOOST_TEST(out.str() == "1 2 3 4 5 1 5 1 2 3 4 5 6 1");
}

BOOST_AUTO_TEST_CASE(seven_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 3, 4, 5, 6, 7, 8 };
  auto first_it = first.cbegin();
  auto second_it = first.cend();
  petrov::ForwardRingList< int > second(first_it, second_it);
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size();
  auto oth_it = first.cbegin();
  out << " " << *(oth_it++);
  do
  {
    out << " " << *oth_it;
  }
  while (oth_it++ != first.cend());
  out << " " << *oth_it;
  BOOST_TEST(out.str() == "1 2 3 4 5 6 7 1 7 1 2 3 4 5 6 7 8 1");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(assign_operator_with_initializer_list)

BOOST_AUTO_TEST_CASE(empty_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list;
  fwd_list = { 1, 2, 3, 4 };
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size();
  BOOST_TEST(out.str() == "1 2 3 4 1 4");
}

BOOST_AUTO_TEST_CASE(zero_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(3, 5);
  fwd_list = {};
  out << fwd_list.empty() << " " << fwd_list.size();
  BOOST_TEST(out.str() == "1 0");
}

BOOST_AUTO_TEST_CASE(one_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(3, 5);
  fwd_list = { 1 };
  auto it = fwd_list.cbegin();
  out << *(it++) << " " << *(it++) << " " << fwd_list.size();
  BOOST_TEST(out.str() == "1 1 1");
}

BOOST_AUTO_TEST_CASE(three_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(1, 1);
  fwd_list = { 1, 2, 3 };
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size();
  BOOST_TEST(out.str() == "1 2 3 1 3");
}

BOOST_AUTO_TEST_CASE(five_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(2, 2);
  fwd_list = { 1, 2, 3, 4, 5 };
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size();
  BOOST_TEST(out.str() == "1 2 3 4 5 1 5");
}

BOOST_AUTO_TEST_CASE(seven_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(3, 4);
  fwd_list = { 1, 2, 3, 4, 5, 6, 7 };
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size();
  BOOST_TEST(out.str() == "1 2 3 4 5 6 7 1 7");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(insert_one)

BOOST_AUTO_TEST_CASE(insert_to_one)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(1, 1);
  auto ret_it = fwd_list.insert(fwd_list.cbegin(), 2);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "1 2 1 2 2");
}

BOOST_AUTO_TEST_CASE(insert_to_two)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list{ 1, 2 };
  auto ret_it = fwd_list.insert(fwd_list.cend(), 4);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "1 2 4 1 3 4");
}

BOOST_AUTO_TEST_CASE(insert_to_three)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list{ 1, 2, 3 };
  auto ret_it = fwd_list.insert(++fwd_list.cbegin(), 4);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "1 2 4 3 1 4 4");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(insert_fill)

BOOST_AUTO_TEST_CASE(insert_nothing)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(2, 1);
  auto ret_it = fwd_list.insert(fwd_list.cbegin(), 0ull, 2);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "1 1 1 2 1");
}

BOOST_AUTO_TEST_CASE(insert_to_one)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(1, 1);
  auto ret_it = fwd_list.insert(fwd_list.cbegin(), 1ull, 2);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "1 2 1 2 2");
}

BOOST_AUTO_TEST_CASE(insert_to_two)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list{ 1, 2 };
  auto ret_it = fwd_list.insert(fwd_list.cend(), 2ull, 4);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "1 2 4 4 1 4 4");
}

BOOST_AUTO_TEST_CASE(insert_to_three)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 1, 2, 3 };
  auto ret_it = fwd_list.insert(++fwd_list.cbegin(), 3ull, 4);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "1 2 4 4 4 3 1 6 4");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(insert_range)

BOOST_AUTO_TEST_CASE(zero_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1 };
  auto first_it = first.cbegin();
  auto second_it = first.cend();
  petrov::ForwardRingList< int > second { 1, 2, 3 };
  auto ret_it = second.insert(++second.cbegin(), first_it, second_it);
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "1 2 3 1 3 2");
}

BOOST_AUTO_TEST_CASE(one_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2 };
  auto first_it = first.cbegin();
  auto second_it = first.cend();
  petrov::ForwardRingList< int > second { 3, 4 };
  auto ret_it = second.insert(++second.cbegin(), first_it, second_it);
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "3 4 1 3 3 1");
}

BOOST_AUTO_TEST_CASE(two_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 3 };
  auto first_it = first.cbegin();
  auto second_it = first.cend();
  petrov::ForwardRingList< int > second { 3, 4, 5 };
  auto ret_it = second.insert(++second.cbegin(), first_it, second_it);
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "3 4 1 2 5 3 5 2");
}

BOOST_AUTO_TEST_CASE(three_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 3, 4, 5 };
  auto first_it = ++first.cbegin();
  auto second_it = first.cend();
  petrov::ForwardRingList< int > second { 10, 9, 8 };
  auto ret_it = second.insert(++second.cbegin(), first_it, second_it);
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "10 9 2 3 4 8 10 6 4");
}

BOOST_AUTO_TEST_CASE(four_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 3, 4, 5, 6 };
  auto first_it = ++first.cbegin();
  auto second_it = first.cend();
  petrov::ForwardRingList< int > second { 10, 9, 8 };
  auto pos = ++second.cbegin();
  auto ret_it = second.insert(pos, first_it, second_it);
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "10 9 2 3 4 5 8 10 7 5");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(insert_initializer_list)

BOOST_AUTO_TEST_CASE(zero_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > second { 1, 2, 3 };
  auto ret_it = second.insert(++second.cbegin(), { });
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "1 2 3 1 3 2");
}

BOOST_AUTO_TEST_CASE(one_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > second { 3, 4 };
  auto ret_it = second.insert(++second.cbegin(), { 1 });
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "3 4 1 3 3 1");
}

BOOST_AUTO_TEST_CASE(two_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > second { 3, 4, 5 };
  auto ret_it = second.insert(++second.cbegin(), { 1, 2 });
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "3 4 1 2 5 3 5 2");
}

BOOST_AUTO_TEST_CASE(three_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > second { 10, 9, 8 };
  auto ret_it = second.insert(++second.cbegin(), { 2, 3, 4 });
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "10 9 2 3 4 8 10 6 4");
}

BOOST_AUTO_TEST_CASE(four_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > second { 10, 9, 8 };
  auto pos = ++second.cbegin();
  auto ret_it = second.insert(pos, { 2, 3, 4, 5 });
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "10 9 2 3 4 5 8 10 7 5");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(assign_range)

BOOST_AUTO_TEST_CASE(zero_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1 };
  auto first_it = first.cbegin();
  auto second_it = first.cend();
  petrov::ForwardRingList< int > second { 1, 2, 3 };
  second.assign(first_it, second_it);
  out << second.empty() << " " << second.size();
  BOOST_TEST(out.str() == "1 0");
}

BOOST_AUTO_TEST_CASE(one_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2 };
  auto first_it = first.cbegin();
  auto second_it = first.cend();
  petrov::ForwardRingList< int > second { 3, 4 };
  second.assign(first_it, second_it);
  auto it = second.cbegin();
  out << *(it++);
  out << " " << *it << " " << second.size();
  BOOST_TEST(out.str() == "1 1 1");
}

BOOST_AUTO_TEST_CASE(two_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 3 };
  auto first_it = first.cbegin();
  auto second_it = first.cend();
  petrov::ForwardRingList< int > second { 3, 4, 5 };
  second.assign(first_it, second_it);
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size();
  BOOST_TEST(out.str() == "1 2 1 2");
}

BOOST_AUTO_TEST_CASE(three_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 3, 4, 5 };
  auto first_it = ++first.cbegin();
  auto second_it = first.cend();
  petrov::ForwardRingList< int > second { 10, 9, 8 };
  second.assign(first_it, second_it);
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size();
  BOOST_TEST(out.str() == "2 3 4 2 3");
}

BOOST_AUTO_TEST_CASE(four_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 3, 4, 5, 6 };
  auto first_it = ++first.cbegin();
  auto second_it = first.cend();
  petrov::ForwardRingList< int > second { 10, 9, 8 };
  auto pos = ++second.cbegin();
  second.assign(first_it, second_it);
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size();
  BOOST_TEST(out.str() == "2 3 4 5 2 4");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(assign_initializer_list)

BOOST_AUTO_TEST_CASE(zero_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > second { 1, 2, 3 };
  second.assign({  });
  out << second.empty() << " " << second.size();
  BOOST_TEST(out.str() == "1 0");
}

BOOST_AUTO_TEST_CASE(one_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > second { 3, 4 };
  second.assign({ 1 });
  auto it = second.cbegin();
  out << *(it++);
  out << " " << *it << " " << second.size();
  BOOST_TEST(out.str() == "1 1 1");
}

BOOST_AUTO_TEST_CASE(two_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > second { 3, 4, 5 };
  second.assign({ 1, 2 });
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size();
  BOOST_TEST(out.str() == "1 2 1 2");
}

BOOST_AUTO_TEST_CASE(three_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > second { 10, 9, 8 };
  second.assign({ 2, 3, 4 });
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size();
  BOOST_TEST(out.str() == "2 3 4 2 3");
}

BOOST_AUTO_TEST_CASE(four_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > second { 10, 9, 8 };
  auto pos = ++second.cbegin();
  second.assign({ 2, 3, 4, 5 });
  auto it = second.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != second.cend());
  out << " " << *it << " " << second.size();
  BOOST_TEST(out.str() == "2 3 4 5 2 4");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(erase_one_el)

BOOST_AUTO_TEST_CASE(erase_itself)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(1, 1);
  auto it = fwd_list.erase(fwd_list.cbegin());
  out << fwd_list.empty() << " " << fwd_list.size() << " " << *it;
  BOOST_TEST(out.str() == "0 1 1");
}

BOOST_AUTO_TEST_CASE(erase_one)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list{ 1, 2 };
  auto ret_it = fwd_list.erase(fwd_list.cbegin());
  auto it = fwd_list.cbegin();
  out << *(it++) << " " << *(it++) << " " << *it << " ";
  out << fwd_list.empty() << " " << fwd_list.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "1 1 1 0 1 1");
}

BOOST_AUTO_TEST_CASE(erase_head)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list{ 1, 2, 3, 4, 5 };
  auto ret_it = fwd_list.erase(fwd_list.cend());
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size();
  BOOST_TEST(out.str() == "2 3 4 5 2 4");
}

BOOST_AUTO_TEST_CASE(erase_tail)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list{ 1, 2, 3, 4, 5 };
  auto del_aft_it = fwd_list.cbegin();
  ++del_aft_it;
  ++del_aft_it;
  ++del_aft_it;
  auto ret_it = fwd_list.erase(del_aft_it);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size();
  BOOST_TEST(out.str() == "1 2 3 4 1 4");
}

BOOST_AUTO_TEST_CASE(ya_erase_one)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list{ 1, 2 };
  auto ret_it = fwd_list.erase(fwd_list.cend());
  auto it = fwd_list.cbegin();
  out << *(it++) << " " << *(it++) << " " << *it << " ";
  out << fwd_list.empty() << " " << fwd_list.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "2 2 2 0 1 2");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(erase_range)

BOOST_AUTO_TEST_CASE(erase_itself)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list(1, 1);
  auto it = fwd_list.erase(fwd_list.cbegin(), fwd_list.cend());
  out << fwd_list.empty() << " " << fwd_list.size() << " " << *it;
  BOOST_TEST(out.str() == "0 1 1");
}

BOOST_AUTO_TEST_CASE(ya_erase_itself)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list{ 1, 2 };
  auto ret_it = fwd_list.erase(fwd_list.cbegin(), fwd_list.cend());
  auto it = fwd_list.cbegin();
  out << *(it++) << " " << *(it++) << " " << *it << " ";
  out << fwd_list.empty() << " " << fwd_list.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "1 2 1 0 2 1");
}

BOOST_AUTO_TEST_CASE(erase_one)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list{ 1, 2, 3 };
  auto ret_it = fwd_list.erase(fwd_list.cbegin(), fwd_list.cend());
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "1 3 1 2 3");
}

BOOST_AUTO_TEST_CASE(erase_tail)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list{ 1, 2, 3, 4, 5 };
  auto first_it = fwd_list.cbegin();
  ++first_it;
  ++first_it;
  auto ret_it = fwd_list.erase(first_it, fwd_list.cbegin());
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "1 2 3 1 3 1");
}

BOOST_AUTO_TEST_CASE(erase_head)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list{ 1, 2, 3, 4, 5 };
  auto first_it = fwd_list.cbegin();
  ++first_it;
  ++first_it;
  auto ret_it = fwd_list.erase(fwd_list.cend(), first_it);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "3 4 5 3 3 3");
}

BOOST_AUTO_TEST_CASE(erase_both)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list{ 1, 2, 3, 4, 5 };
  auto second_it = fwd_list.cbegin();
  ++second_it;
  ++second_it;
  auto first_it = fwd_list.cbegin();
  ++first_it;
  ++first_it;
  ++first_it;
  auto ret_it = fwd_list.erase(first_it, second_it);
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "3 4 3 2 3");
}

BOOST_AUTO_TEST_CASE(erase_btw)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list{ 1, 2, 3, 4, 5, 6, 7 };
  auto first_it = fwd_list.cbegin();
  ++first_it;
  ++first_it;
  auto ret_it = fwd_list.erase(first_it, fwd_list.cend());
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size() << " " << *ret_it;
  BOOST_TEST(out.str() == "1 2 3 7 1 4 7");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(comparasions_overload)

BOOST_AUTO_TEST_CASE(less_both_empty)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first;
  petrov::ForwardRingList< int > second;
  out << (first < second);
  BOOST_TEST(out.str() == "0");
}

BOOST_AUTO_TEST_CASE(less_first_empty)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first;
  petrov::ForwardRingList< int > second(1, 1);
  out << (first < second);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(less_second_empty)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(1, 1);
  petrov::ForwardRingList< int > second;
  out << (first < second);
  BOOST_TEST(out.str() == "0");
}

BOOST_AUTO_TEST_CASE(less_both_equal_1)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(1, 1);
  petrov::ForwardRingList< int > second(1, 1);
  out << (first < second);
  BOOST_TEST(out.str() == "0");
}

BOOST_AUTO_TEST_CASE(less_one)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(1, 1);
  petrov::ForwardRingList< int > second(1, 2);
  out << (first < second);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(less_both_equal_2)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(5, 5);
  petrov::ForwardRingList< int > second(5, 5);
  out << (first < second);
  BOOST_TEST(out.str() == "0");
}

BOOST_AUTO_TEST_CASE(less_start)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(5, 3);
  petrov::ForwardRingList< int > second(5, 5);
  out << (first < second);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(less_end)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 3, 4, 5 };
  petrov::ForwardRingList< int > second { 1, 2, 3, 4, 6 };
  out << (first < second);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(less_middle)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 2, 4, 5 };
  petrov::ForwardRingList< int > second { 1, 2, 3, 4, 5 };
  out << (first < second);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(more_start)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first(5, 3);
  petrov::ForwardRingList< int > second(5, 5);
  out << (second > first);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(more_end)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 3, 4, 5 };
  petrov::ForwardRingList< int > second { 1, 2, 3, 4, 6 };
  out << (second > first);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(more_middle)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 2, 4, 5 };
  petrov::ForwardRingList< int > second { 1, 2, 3, 4, 5 };
  out << (second > first);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(length_test_1)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 3, 4, 5, 6 };
  petrov::ForwardRingList< int > second { 1, 2, 3, 4, 5 };
  out << (first < second);
  BOOST_TEST(out.str() == "0");
}

BOOST_AUTO_TEST_CASE(length_test_2)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 3, 4, 5 };
  petrov::ForwardRingList< int > second { 1, 2, 3, 4, 5, 6 };
  out << (first < second);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(less_equal_1)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 3, 4, 5, 6 };
  petrov::ForwardRingList< int > second { 1, 2, 3, 4, 5, 6 };
  out << (first <= second);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(less_equal_2)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 3, 4 };
  petrov::ForwardRingList< int > second { 1, 2, 3, 4, 5, 6 };
  out << (first <= second);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(more_equal_1)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 3, 4, 5, 6 };
  petrov::ForwardRingList< int > second { 1, 2, 3, 4, 5, 6 };
  out << (first >= second);
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(more_equal_2)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first { 1, 2, 3, 4 };
  petrov::ForwardRingList< int > second { 1, 2, 3, 4, 5, 6 };
  out << (first >= second);
  BOOST_TEST(out.str() == "0");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(emplace_pos)

BOOST_AUTO_TEST_CASE(emplace_after_head)
{
  std::ostringstream out;
  petrov::ForwardRingList< std::pair< int, char > > fwd_list { { 1, 'x' }, { 2, 'y' }, { 3, 'z' } };
  auto pos = fwd_list.cbegin();
  fwd_list.emplace_after(pos, 4, 'a');
  auto it = fwd_list.cbegin();
  out << it->first << " " << (it++)->second;
  do
  {
    out << " " << it->first << " " << it->second;
  }
  while (it++ != fwd_list.cend());
  out << " " << it->first << " " << it->second;
  BOOST_TEST(out.str() == "1 x 4 a 2 y 3 z 1 x");
}

BOOST_AUTO_TEST_CASE(emplace_in_middle)
{
  std::ostringstream out;
  petrov::ForwardRingList< std::pair< int, char > > fwd_list { { 1, 'x' }, { 2, 'y' }, { 3, 'z' } };
  auto pos = ++fwd_list.cbegin();
  fwd_list.emplace_after(pos, 4, 'a');
  auto it = fwd_list.cbegin();
  out << it->first << " " << (it++)->second;
  do
  {
    out << " " << it->first << " " << it->second;
  }
  while (it++ != fwd_list.cend());
  out << " " << it->first << " " << it->second;
  BOOST_TEST(out.str() == "1 x 2 y 4 a 3 z 1 x");
}

BOOST_AUTO_TEST_CASE(emplace_after_tail)
{
  std::ostringstream out;
  petrov::ForwardRingList< std::pair< int, char > > fwd_list { { 1, 'x' }, { 2, 'y' }, { 3, 'z' } };
  auto pos = ++fwd_list.cbegin();
  ++pos;
  fwd_list.emplace_after(pos, 4, 'a');
  auto it = fwd_list.cbegin();
  out << it->first << " " << (it++)->second;
  do
  {
    out << " " << it->first << " " << it->second;
  }
  while (it++ != fwd_list.cend());
  out << " " << it->first << " " << it->second;
  BOOST_TEST(out.str() == "1 x 2 y 3 z 4 a 1 x");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(emplace_front)

BOOST_AUTO_TEST_CASE(emplace_one_time)
{
  std::ostringstream out;
  petrov::ForwardRingList< std::pair< int, char > > fwd_list { { 1, 'x' }, { 2, 'y' }, { 3, 'z' } };
  fwd_list.emplace_front(4, 'a');
  auto it = fwd_list.cbegin();
  out << it->first << " " << (it++)->second;
  do
  {
    out << " " << it->first << " " << it->second;
  }
  while (it++ != fwd_list.cend());
  out << " " << it->first << " " << it->second;
  BOOST_TEST(out.str() == "4 a 1 x 2 y 3 z 4 a");
}

BOOST_AUTO_TEST_CASE(emplace_two_times)
{
  std::ostringstream out;
  petrov::ForwardRingList< std::pair< int, char > > fwd_list { { 1, 'x' }, { 2, 'y' }, { 3, 'z' } };
  fwd_list.emplace_front(4, 'a');
  fwd_list.emplace_front(5, 'b');
  auto it = fwd_list.cbegin();
  out << it->first << " " << (it++)->second;
  do
  {
    out << " " << it->first << " " << it->second;
  }
  while (it++ != fwd_list.cend());
  out << " " << it->first << " " << it->second;
  BOOST_TEST(out.str() == "5 b 4 a 1 x 2 y 3 z 5 b");
}

BOOST_AUTO_TEST_CASE(emplace_three_times)
{
  std::ostringstream out;
  petrov::ForwardRingList< std::pair< int, char > > fwd_list { { 1, 'x' }, { 2, 'y' }, { 3, 'z' } };
  fwd_list.emplace_front(4, 'a');
  fwd_list.emplace_front(5, 'b');
  fwd_list.emplace_front(6, 'c');
  auto it = fwd_list.cbegin();
  out << it->first << " " << (it++)->second;
  do
  {
    out << " " << it->first << " " << it->second;
  }
  while (it++ != fwd_list.cend());
  out << " " << it->first << " " << it->second;
  BOOST_TEST(out.str() == "6 c 5 b 4 a 1 x 2 y 3 z 6 c");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(sort_basic)

BOOST_AUTO_TEST_CASE(sort_one_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 3 };
  fwd_list.sort();
  auto it = fwd_list.cbegin();
  out << *(it++) << " " << *it;
  BOOST_TEST(out.str() == "3 3");
}

BOOST_AUTO_TEST_CASE(sort_two_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 3, 1 };
  fwd_list.sort();
  auto it = fwd_list.cbegin();
  out << *(it++) << " " << *(it++) << " " << *it;
  BOOST_TEST(out.str() == "1 3 1");
}

BOOST_AUTO_TEST_CASE(sort_usual)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 3, 7, 5, 6, 1, 4 };
  fwd_list.sort();
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it;
  BOOST_TEST(out.str() == "1 3 4 5 6 7 1");
}

BOOST_AUTO_TEST_CASE(sort_example)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 2, 8, 7, 1, 3, 5, 6, 4 };
  fwd_list.sort();
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it;
  BOOST_TEST(out.str() == "1 2 3 4 5 6 7 8 1");
}

BOOST_AUTO_TEST_CASE(sort_sorted_1)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 1, 2 };
  fwd_list.sort();
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it;
  BOOST_TEST(out.str() == "1 2 1");
}

BOOST_AUTO_TEST_CASE(sort_sorted_2)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 1, 2, 3, 4, 5 };
  fwd_list.sort();
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it;
  BOOST_TEST(out.str() == "1 2 3 4 5 1");
}

BOOST_AUTO_TEST_CASE(sort_sorted_3)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 1, 2, 3, 4, 5, 6, 7 };
  fwd_list.sort();
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it;
  BOOST_TEST(out.str() == "1 2 3 4 5 6 7 1");
}

BOOST_AUTO_TEST_CASE(sort_unsorted)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 7, 6, 5, 4, 3, 2, 1 };
  fwd_list.sort();
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it;
  BOOST_TEST(out.str() == "1 2 3 4 5 6 7 1");
}

BOOST_AUTO_TEST_CASE(sort_1)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 56, 55, 2, 3, 11 };
  fwd_list.sort();
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it;
  BOOST_TEST(out.str() == "2 3 11 55 56 2");
}

BOOST_AUTO_TEST_CASE(sort_2)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 31, 61, 7, 43, 97 };
  fwd_list.sort();
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it;
  BOOST_TEST(out.str() == "7 31 43 61 97 7");
}

BOOST_AUTO_TEST_CASE(sort_3)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 70, 19, 98, 24, 34 };
  fwd_list.sort();
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it;
  BOOST_TEST(out.str() == "19 24 34 70 98 19");
}

BOOST_AUTO_TEST_CASE(sort_not_unique_1)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 3, 10, 5, 6, 2, 6, 10, 10, 2, 3 };
  fwd_list.sort();
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it;
  BOOST_TEST(out.str() == "2 2 3 3 5 6 6 10 10 10 2");
}

BOOST_AUTO_TEST_CASE(sort_not_unique_2)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 8, 5, 6, 9, 2, 3, 8, 6, 2, 6 };
  fwd_list.sort();
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it;
  BOOST_TEST(out.str() == "2 2 3 5 6 6 6 8 8 9 2");
}

BOOST_AUTO_TEST_CASE(sort_not_unique_3)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 3, 2, 6, 5, 6, 10, 8, 3, 2, 3 };
  fwd_list.sort();
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it;
  BOOST_TEST(out.str() == "2 2 3 3 3 5 6 6 8 10 2");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(sort_comp)

BOOST_AUTO_TEST_CASE(sort_one_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 3 };
  fwd_list.sort(std::greater< int >{});
  auto it = fwd_list.cbegin();
  out << *(it++) << " " << *it;
  BOOST_TEST(out.str() == "3 3");
}

BOOST_AUTO_TEST_CASE(sort_two_el)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 1, 3 };
  fwd_list.sort(std::greater< int >{});
  auto it = fwd_list.cbegin();
  out << *(it++) << " " << *(it++) << " " << *it;
  BOOST_TEST(out.str() == "3 1 3");
}

BOOST_AUTO_TEST_CASE(sort_usual)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 3, 7, 5, 6, 1, 4 };
  fwd_list.sort(std::greater< int >{});
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it;
  BOOST_TEST(out.str() == "7 6 5 4 3 1 7");
}

BOOST_AUTO_TEST_CASE(sort_sorted_1)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 2, 1 };
  fwd_list.sort(std::greater< int >{});
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it;
  BOOST_TEST(out.str() == "2 1 2");
}

BOOST_AUTO_TEST_CASE(sort_sorted_2)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 5, 4, 3, 2, 1 };
  fwd_list.sort(std::greater< int >{});
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it;
  BOOST_TEST(out.str() == "5 4 3 2 1 5");
}

BOOST_AUTO_TEST_CASE(sort_sorted_3)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 7, 6, 5, 4, 3, 2, 1 };
  fwd_list.sort(std::greater< int >{});
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it;
  BOOST_TEST(out.str() == "7 6 5 4 3 2 1 7");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(merge_method)

BOOST_AUTO_TEST_CASE(merge_empty)
{
  std::ostringstream out;
  petrov::ForwardRingList< double > first{ 4.2, 2.9, 3.1 };
  petrov::ForwardRingList< double > second;
  first.sort();
  first.merge(second);
  auto it = first.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != first.cend());
  out << " " << *it << " " << first.size() << " " << second.size();
  BOOST_TEST(out.str() == "2.9 3.1 4.2 2.9 3 0");
}

BOOST_AUTO_TEST_CASE(merge_example)
{
  std::ostringstream out;
  petrov::ForwardRingList< double > first{ 4.2, 2.9, 3.1 };
  petrov::ForwardRingList< double > second{ 1.4, 7.7, 3.1 };
  first.sort();
  second.sort();
  first.merge(second);
  auto it = first.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != first.cend());
  out << " " << *it << " " << first.size() << " " << second.size();
  BOOST_TEST(out.str() == "1.4 2.9 3.1 3.1 4.2 7.7 1.4 6 0");
}

BOOST_AUTO_TEST_CASE(merge_1)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first{ 1, 5, 6, 9, 4, 4, 4, 5, 10 };
  petrov::ForwardRingList< int > second{ 5, 0, 1, 9, 7 };
  first.sort();
  second.sort();
  first.merge(second);
  auto it = first.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != first.cend());
  out << " " << *it << " " << first.size() << " " << second.size();
  BOOST_TEST(out.str() == "0 1 1 4 4 4 5 5 5 6 7 9 9 10 0 14 0");
}

BOOST_AUTO_TEST_CASE(merge_example_comp)
{
  std::ostringstream out;
  petrov::ForwardRingList< double > first{ 4.2, 2.9, 3.1 };
  petrov::ForwardRingList< double > second{ 1.4, 7.7, 3.1 };
  first.sort(std::greater< double >{});
  second.sort(std::greater< double >{});
  first.merge(second, std::greater< double >{});
  auto it = first.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != first.cend());
  out << " " << *it << " " << first.size() << " " << second.size();
  BOOST_TEST(out.str() == "7.7 4.2 3.1 3.1 2.9 1.4 7.7 6 0");
}

BOOST_AUTO_TEST_CASE(merge_2)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > first{ 1, 5, 6, 9, 4, 4, 4, 5, 10 };
  petrov::ForwardRingList< int > second{ 5, 0, 1, 9, 7 };
  first.sort(std::greater< int >{});
  second.sort(std::greater< int >{});
  first.merge(second, std::greater< int >{});
  auto it = first.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != first.cend());
  out << " " << *it << " " << first.size() << " " << second.size();
  BOOST_TEST(out.str() == "10 9 9 7 6 5 5 5 4 4 4 1 1 0 10 14 0");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(unique_method)

BOOST_AUTO_TEST_CASE(unique_empty)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list;
  fwd_list.sort();
  fwd_list.unique();
  out << fwd_list.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(unique_one_el_list)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 5 };
  fwd_list.sort();
  fwd_list.unique();
  out << fwd_list.empty() << " " << *fwd_list.cbegin();
  out << " " << *(++fwd_list.cbegin()) << " " << fwd_list.size();
  BOOST_TEST(out.str() == "0 5 5 1");
}

BOOST_AUTO_TEST_CASE(unique_example)
{
  std::ostringstream out;
  petrov::ForwardRingList< double > fwd_list { 15.2, 73.0, 3.14, 15.85, 69.5,
                                               73.0, 3.99, 15.2, 69.2, 18.5 };
  fwd_list.sort();
  fwd_list.unique();
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size();
  BOOST_TEST(out.str() == "3.14 3.99 15.2 15.85 18.5 69.2 69.5 73 3.14 8");
}

BOOST_AUTO_TEST_CASE(unique_all)
{
  std::ostringstream out;
  petrov::ForwardRingList< int > fwd_list { 5, 5, 5, 5, 5 };
  fwd_list.unique();
  auto it = fwd_list.cbegin();
  out << *(it++);
  out << " " << *it << " " << fwd_list.size();
  BOOST_TEST(out.str() == "5 5 1");
}

BOOST_AUTO_TEST_CASE(unique_bin_pred)
{
  std::ostringstream out;
  petrov::ForwardRingList< double > fwd_list { 15.2, 73.0, 3.14, 15.85, 69.5,
                                               73.0, 3.99, 15.2, 69.2, 18.5 };
  fwd_list.unique(std::greater< double >{});
  auto it = fwd_list.cbegin();
  out << *(it++);
  do
  {
    out << " " << *it;
  }
  while (it++ != fwd_list.cend());
  out << " " << *it << " " << fwd_list.size();
  BOOST_TEST(out.str() == "15.2 3.14 15.2 2");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

