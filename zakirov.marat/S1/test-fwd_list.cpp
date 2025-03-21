#include <boost/test/unit_test.hpp>
#include "fwd_list.hpp"
#include "fwd_iterator.hpp"
#include "fwd_list_node.hpp"

BOOST_AUTO_TEST_CASE(default_constructor)
{
  zakirov::FwdList< int > test_list;
  BOOST_TEST(test_list.empty() == true);
  BOOST_TEST(test_list.size() == 0);
}

BOOST_AUTO_TEST_CASE(initializer_list_constructor)
{
  zakirov::FwdList< int > test_list({0, 1, 2, 3, 4});
  BOOST_TEST(test_list.empty() == false);
  BOOST_TEST(test_list.size() == 5);
  zakirov::FwdIterator< int > test_iter = test_list.begin();
  for (size_t i = 0; i < 5; ++i, ++test_iter)
  {
    BOOST_TEST(*test_iter == i);
  }
}

BOOST_AUTO_TEST_CASE(fill_constructor)
{
  zakirov::FwdList< int > test_list(5, 1);
  BOOST_TEST(test_list.empty() == false);
  BOOST_TEST(test_list.size() == 5);
  zakirov::FwdIterator< int > test_iter = test_list.begin();
  for (size_t i = 0; i < 5; ++i, ++test_iter)
  {
    BOOST_TEST(*test_iter == 1);
  }
}

BOOST_AUTO_TEST_CASE(range_constructor)
{
  int test_array[5] = {0, 1, 2, 3, 4};
  zakirov::FwdList< int > test_list(test_array, test_array + 5);
  BOOST_TEST(test_list.empty() == false);
  BOOST_TEST(test_list.size() == 5);
  zakirov::FwdIterator< int > test_iter = test_list.begin();
  for (size_t i = 0; i < 5; ++i, ++test_iter)
  {
    BOOST_TEST(*test_iter == i);
  }
}

BOOST_AUTO_TEST_CASE(size)
{
  zakirov::FwdList< int > test_list;
  BOOST_TEST(test_list.size() == 0);
  zakirov::FwdList< int > test_list_2({0, 1, 2, 3, 4});
  BOOST_TEST(test_list_2.size() == 5);
}

BOOST_AUTO_TEST_CASE(empty)
{
  zakirov::FwdList< int > test_list;
  BOOST_TEST(test_list.empty() == true);
  zakirov::FwdList< int > test_list_2({0, 1, 2, 3, 4});
  BOOST_TEST(test_list_2.empty() == false);
}
