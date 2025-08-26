#include <boost/test/unit_test.hpp>
#include <sstream>
#include <queue.hpp>

BOOST_AUTO_TEST_SUITE(copy_constructor)

BOOST_AUTO_TEST_CASE(make_queue_from_empty_queue)
{
  std::ostringstream out;
  petrov::Queue< int > first_queue;
  petrov::Queue< int > second_queue(first_queue);
  out << first_queue.empty() << " " << second_queue.empty();
  BOOST_TEST(out.str() == "1 1");
}

BOOST_AUTO_TEST_CASE(make_queue_from_one_element_queue)
{
  std::ostringstream out;
  petrov::Queue< int > first_queue;
  first_queue.push(1);
  petrov::Queue< int > second_queue(first_queue);
  out << first_queue.front() << " " << second_queue.front();
  BOOST_TEST(out.str() == "1 1");
}

BOOST_AUTO_TEST_CASE(make_queue_from_five_element_queue)
{
  std::ostringstream out;
  petrov::Queue< int > first_queue;
  for (size_t i = 1; i <= 5; i++)
  {
    first_queue.push(i);
  }
  petrov::Queue< int > second_queue(first_queue);
  out << second_queue.front();
  second_queue.pop();
  for (size_t i = 1; i < 5; i++)
  {
    out << " " << second_queue.front();
    second_queue.pop();
  }
  BOOST_TEST(out.str() == "1 2 3 4 5");
}

BOOST_AUTO_TEST_CASE(make_queue_from_six_element_queue)
{
  std::ostringstream out;
  petrov::Queue< int > first_queue;
  for (size_t i = 1; i <= 6; i++)
  {
    first_queue.push(i);
  }
  petrov::Queue< int > second_queue(first_queue);
  out << second_queue.front();
  second_queue.pop();
  for (size_t i = 1; i < 6; i++)
  {
    out << " " << second_queue.front();
    second_queue.pop();
  }
  BOOST_TEST(out.str() == "1 2 3 4 5 6");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(assign)

BOOST_AUTO_TEST_CASE(assign_queue_from_empty_element_queue)
{
  std::ostringstream out;
  petrov::Queue< int > first_queue;
  petrov::Queue< int > second_queue;
  second_queue.push(1);
  second_queue = first_queue;
  out << second_queue.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(assign_queue_from_one_element_queue)
{
  std::ostringstream out;
  petrov::Queue< int > first_queue;
  petrov::Queue< int > second_queue;
  first_queue.push(1);
  second_queue = first_queue;
  out << second_queue.front();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(assign_queue_from_five_element_queue)
{
  std::ostringstream out;
  petrov::Queue< int > first_queue;
  for (size_t i = 1; i <= 5; i++)
  {
    first_queue.push(i);
  }
  petrov::Queue< int > second_queue;
  for (size_t i = 1; i <= 3; i++)
  {
    second_queue.push(i);
  }
  second_queue = first_queue;
  out << second_queue.front();
  second_queue.pop();
  for (size_t i = 1; i < 5; i++)
  {
    out << " " << second_queue.front();
    second_queue.pop();
  }
  BOOST_TEST(out.str() == "1 2 3 4 5");
}

BOOST_AUTO_TEST_CASE(make_queue_from_six_element_queue)
{
  std::ostringstream out;
  petrov::Queue< int > first_queue;
  for (size_t i = 1; i <= 6; i++)
  {
    first_queue.push(i);
  }
  petrov::Queue< int > second_queue;
  for (size_t i = 1; i <= 5; i++)
  {
    second_queue.push(i);
  }
  second_queue = first_queue;
  out << second_queue.front();
  second_queue.pop();
  for (size_t i = 1; i < 6; i++)
  {
    out << " " << second_queue.front();
    second_queue.pop();
  }
  BOOST_TEST(out.str() == "1 2 3 4 5 6");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(queue_push_method)

BOOST_AUTO_TEST_CASE(push_one_element)
{
  std::ostringstream out;
  petrov::Queue< int > queue;
  queue.push(1);
  out << queue.front();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(push_three_elements)
{
  std::ostringstream out;
  petrov::Queue< int > queue;
  for (size_t i = 1; i <= 3; i++)
  {
    queue.push(i);
  }
  out << queue.front();
  queue.pop();
  for (size_t i = 1; i < 3; i++)
  {
    out << " " << queue.front();
    queue.pop();
  }
  BOOST_TEST(out.str() == "1 2 3");
}

BOOST_AUTO_TEST_CASE(push_five_elements)
{
  std::ostringstream out;
  petrov::Queue< int > queue;
  for (size_t i = 1; i <= 5; i++)
  {
    queue.push(i);
  }
  out << queue.front();
  queue.pop();
  for (size_t i = 1; i < 5; i++)
  {
    out << " " << queue.front();
    queue.pop();
  }
  BOOST_TEST(out.str() == "1 2 3 4 5");
}

BOOST_AUTO_TEST_CASE(push_sixth_elements)
{
  std::ostringstream out;
  petrov::Queue< int > queue;
  for (size_t i = 1; i <= 6; i++)
  {
    queue.push(i);
  }
  out << queue.front();
  queue.pop();
  for (size_t i = 1; i < 6; i++)
  {
    out << " " << queue.front();
    queue.pop();
  }
  BOOST_TEST(out.str() == "1 2 3 4 5 6");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(queue_pop_method)

BOOST_AUTO_TEST_CASE(pop_one_element)
{
  std::ostringstream out;
  petrov::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  queue.push(3);
  queue.pop();
  out << queue.front();
  BOOST_TEST(out.str() == "2");
}

BOOST_AUTO_TEST_CASE(pop_three_elements)
{
  std::ostringstream out;
  petrov::Queue< int > queue;
  for (size_t i = 1; i <= 5; i++)
  {
    queue.push(i);
  }
  for (size_t i = 1; i <= 3; i++)
  {
    queue.pop();
  }
  out << queue.front();
  queue.pop();
  out << " " << queue.front();
  BOOST_TEST(out.str() == "4 5");
}

BOOST_AUTO_TEST_CASE(pop_all_elements)
{
  std::ostringstream out;
  petrov::Queue< int > queue;
  for (size_t i = 1; i <= 6; i++)
  {
    queue.push(i);
  }
  for (size_t i = 1; i <= 6; i++)
  {
    queue.pop();
  }
  out << queue.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(empty_method)

BOOST_AUTO_TEST_CASE(empty_queue)
{
  std::ostringstream out;
  petrov::Queue< int > queue;
  out << queue.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(make_empty_queue)
{
  std::ostringstream out;
  petrov::Queue< int > queue;
  queue.push(1);
  queue.pop();
  out << queue.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(not_empty_queue)
{
  std::ostringstream out;
  petrov::Queue< int > queue;
  queue.push(0);
  out << queue.empty();
  BOOST_TEST(out.str() == "0");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(back_method)

BOOST_AUTO_TEST_CASE(equal_front_and_back)
{
  std::ostringstream out;
  petrov::Queue< int > queue;
  queue.push(1);
  out << queue.front() << " " << queue.back();
  BOOST_TEST(out.str() == "1 1");
}

BOOST_AUTO_TEST_CASE(not_equal_front_and_back)
{
  std::ostringstream out;
  petrov::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  out << queue.front() << " " << queue.back();
  BOOST_TEST(out.str() == "1 2");
}

BOOST_AUTO_TEST_CASE(yet_another_not_equal_front_and_back)
{
  std::ostringstream out;
  petrov::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  queue.push(3);
  out << queue.front() << " " << queue.back();
  BOOST_TEST(out.str() == "1 3");
}

BOOST_AUTO_TEST_CASE(check_back_until_empty)
{
  std::ostringstream out;
  petrov::Queue< int > queue;
  for (size_t i = 1; i <= 5; i++)
  {
    queue.push(i);
  }
  for (size_t i = 1; i <= 5; i++)
  {
    out << queue.back() << " ";
    queue.pop();
  }
  out << queue.empty();
  BOOST_TEST(out.str() == "5 5 5 5 5 1");
}

BOOST_AUTO_TEST_SUITE_END()
