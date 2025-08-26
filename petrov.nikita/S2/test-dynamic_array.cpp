#include <boost/test/unit_test.hpp>
#include <sstream>
#include <dynamic_array.hpp>

BOOST_AUTO_TEST_SUITE(copy_constructor)

BOOST_AUTO_TEST_CASE(make_array_from_empty_array)
{
  std::ostringstream out;
  petrov::DynamicArray< int > first_array;
  petrov::DynamicArray< int > second_array(first_array);
  out << first_array.empty() << " " << second_array.empty();
  BOOST_TEST(out.str() == "1 1");
}

BOOST_AUTO_TEST_CASE(make_array_from_one_element_array)
{
  std::ostringstream out;
  petrov::DynamicArray< int > first_array;
  first_array.push_back(1);
  petrov::DynamicArray< int > second_array(first_array);
  out << first_array.front() << " " << second_array.front();
  BOOST_TEST(out.str() == "1 1");
}

BOOST_AUTO_TEST_CASE(make_array_from_five_element_array)
{
  std::ostringstream out;
  petrov::DynamicArray< int > first_array;
  for (size_t i = 1; i <= 5; i++)
  {
    first_array.push_back(i);
  }
  petrov::DynamicArray< int > second_array(first_array);
  out << second_array[0];
  for (size_t i = 1; i < 5; i++)
  {
    out << " " << second_array[i];
  }
  BOOST_TEST(out.str() == "1 2 3 4 5");
}

BOOST_AUTO_TEST_CASE(make_array_from_six_element_array)
{
  std::ostringstream out;
  petrov::DynamicArray< int > first_array;
  for (size_t i = 1; i <= 6; i++)
  {
    first_array.push_back(i);
  }
  petrov::DynamicArray< int > second_array(first_array);
  out << second_array[0];
  for (size_t i = 1; i < 6; i++)
  {
    out << " " << second_array[i];
  }
  BOOST_TEST(out.str() == "1 2 3 4 5 6");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(assign)

BOOST_AUTO_TEST_CASE(assign_array_from_empty_element_array)
{
  std::ostringstream out;
  petrov::DynamicArray< int > first_array;
  petrov::DynamicArray< int > second_array;
  second_array.push_back(1);
  second_array = first_array;
  out << second_array.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(assign_array_from_one_element_array)
{
  std::ostringstream out;
  petrov::DynamicArray< int > first_array;
  petrov::DynamicArray< int > second_array;
  first_array.push_back(1);
  second_array = first_array;
  out << second_array[0];
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(assign_array_from_five_element_array)
{
  std::ostringstream out;
  petrov::DynamicArray< int > first_array;
  for (size_t i = 1; i <= 5; i++)
  {
    first_array.push_back(i);
  }
  petrov::DynamicArray< int > second_array;
  second_array = first_array;
  out << second_array[0];
  for (size_t i = 1; i < 5; i++)
  {
    out << " " << second_array[i];
  }
  BOOST_TEST(out.str() == "1 2 3 4 5");
}

BOOST_AUTO_TEST_CASE(assign_array_from_six_element_array)
{
  std::ostringstream out;
  petrov::DynamicArray< int > first_array;
  for (size_t i = 1; i <= 6; i++)
  {
    first_array.push_back(i);
  }
  petrov::DynamicArray< int > second_array;
  second_array = first_array;
  out << second_array[0];
  for (size_t i = 1; i < 6; i++)
  {
    out << " " << second_array[i];
  }
  BOOST_TEST(out.str() == "1 2 3 4 5 6");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(push_back_metod)

BOOST_AUTO_TEST_CASE(push_back_one_element)
{
  std::ostringstream out;
  petrov::DynamicArray< int > array;
  array.push_back(1);
  out << array.front();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(push_back_three_elements)
{
  std::ostringstream out;
  petrov::DynamicArray< int > array;
  for (size_t i = 1; i <= 3; i++)
  {
    array.push_back(i);
  }
  out << array[0];
  for (size_t i = 1; i < 3; i++)
  {
    out << " " << array[i];
  }
  BOOST_TEST(out.str() == "1 2 3");
}

BOOST_AUTO_TEST_CASE(push_back_five_elements)
{
  std::ostringstream out;
  petrov::DynamicArray< int > array;
  for (size_t i = 1; i <= 5; i++)
  {
    array.push_back(i);
  }
  out << array[0];
  for (size_t i = 1; i < 5; i++)
  {
    out << " " << array[i];
  }
  BOOST_TEST(out.str() == "1 2 3 4 5");
}

BOOST_AUTO_TEST_CASE(push_back_six_elements)
{
  std::ostringstream out;
  petrov::DynamicArray< int > array;
  for (size_t i = 1; i <= 6; i++)
  {
    array.push_back(i);
  }
  out << array[0];
  for (size_t i = 1; i < 6; i++)
  {
    out << " " << array[i];
  }
  BOOST_TEST(out.str() == "1 2 3 4 5 6");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(array_pop_back_method)

BOOST_AUTO_TEST_CASE(pop_back_one_element)
{
  std::ostringstream out;
  petrov::DynamicArray< int > array;
  array.push_back(1);
  array.push_back(2);
  array.push_back(3);
  array.pop_back();
  out << array.back();
  BOOST_TEST(out.str() == "2");
}

BOOST_AUTO_TEST_CASE(pop_back_three_element)
{
  std::ostringstream out;
  petrov::DynamicArray< int > array;
  for (size_t i = 1; i <= 5; i++)
  {
    array.push_back(i);
  }
  for (size_t i = 1; i <= 3; i++)
  {
    array.pop_back();
  }
  out << array[0];
  for (size_t i = 1; i < array.size(); i++)
  {
    out << " " << array[i];
  }
  BOOST_TEST(out.str() == "1 2");
}

BOOST_AUTO_TEST_CASE(pop_back_five_elements)
{
  std::ostringstream out;
  petrov::DynamicArray< int > array;
  for (size_t i = 1; i <= 6; i++)
  {
    array.push_back(i);
  }
  for (size_t i = 1; i <= 5; i++)
  {
    array.pop_back();
  }
  out << array[0];
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(pop_back_all_elements)
{
  std::ostringstream out;
  petrov::DynamicArray< int > array;
  for (size_t i = 1; i <= 6; i++)
  {
    array.push_back(i);
  }
  for (size_t i = 1; i <= 6; i++)
  {
    array.pop_back();
  }
  out << array.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_SUITE_END()
