#include <boost/test/unit_test.hpp>
#include "fwd_list.hpp"
#include "fwd_iterator.hpp"
#include "fwd_list_node.hpp"

BOOST_AUTO_TEST_CASE(constructor_default)
{
  zakirov::FwdList< int > test_list;
  BOOST_TEST(test_list.empty() == true);
  BOOST_TEST(test_list.size() == 0);
}

BOOST_AUTO_TEST_CASE(constructor_initlist)
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

BOOST_AUTO_TEST_CASE(constructor_fill)
{
  constexpr size_t quantity = 5;
  constexpr int value = 1;
  zakirov::FwdList< int > test_list(quantity, value);
  BOOST_TEST(test_list.empty() == false);
  BOOST_TEST(test_list.size() == 5);
  zakirov::FwdIterator< int > test_iter = test_list.begin();
  for (size_t i = 0; i < 5; ++i, ++test_iter)
  {
    BOOST_TEST(*test_iter == 1);
  }
}

BOOST_AUTO_TEST_CASE(constructor_range)
{
  zakirov::FwdList< int > primal_list({0, 1, 2, 3, 4});
  zakirov::FwdList< int > test_list(primal_list.begin(), primal_list.end());
  BOOST_TEST(test_list.empty() == false);
  BOOST_TEST(test_list.size() == 5);
  zakirov::FwdIterator< int > test_iter = test_list.begin();
  for (size_t i = 0; i < 5; ++i, ++test_iter)
  {
    BOOST_TEST(*test_iter == i);
  }
}

BOOST_AUTO_TEST_CASE(constructor_copy)
{
  zakirov::FwdList< int > primal_list({0, 1, 2, 3, 4});
  zakirov::FwdList< int > test_list(primal_list);
  BOOST_TEST(test_list.empty() == false);
  BOOST_TEST(test_list.size() == primal_list.size());
  zakirov::FwdIterator< int > primal_iter = primal_list.begin();
  zakirov::FwdIterator< int > test_iter = test_list.begin();
  for (size_t i = 0; i < test_list.size(); ++i, ++primal_iter, ++test_iter)
  {
    BOOST_TEST(*primal_iter == *test_iter);
  }
}

BOOST_AUTO_TEST_CASE(constructor_move)
{
  zakirov::FwdList< int > test_list(zakirov::FwdList< int >({0, 1, 2, 3, 4}));
  BOOST_TEST(test_list.empty() == false);
  BOOST_TEST(test_list.size() == 5);
  zakirov::FwdIterator< int > test_iter = test_list.begin();
  for (size_t i = 0; i < test_list.size(); ++i, ++test_iter)
  {
    BOOST_TEST(*test_iter == i);
  }
}

BOOST_AUTO_TEST_CASE(assignment_operator_lvalue)
{
  zakirov::FwdList< int > lvalue_test_lst;
  zakirov::FwdList< int > lvalue_data_lst({0, 1, 2, 3, 4});
  lvalue_test_lst = lvalue_data_lst;
  BOOST_TEST(lvalue_test_lst.empty() == false);
  BOOST_TEST(lvalue_test_lst.size() == 5);
  zakirov::FwdIterator< int > test_iter = lvalue_test_lst.begin();
  for (size_t i = 0; i < 5; ++i, ++test_iter)
  {
    BOOST_TEST(*test_iter == i);
  }
}

BOOST_AUTO_TEST_CASE(assignment_operator_rvalue)
{
  zakirov::FwdList< int > rvalue_data_lst({0, 1, 2, 3, 4});
  zakirov::FwdList< int > rvalue_test_lst;
  rvalue_test_lst = std::move(rvalue_data_lst);
  BOOST_TEST(rvalue_test_lst.empty() == false);
  BOOST_TEST(rvalue_test_lst.size() == 5);
  zakirov::FwdIterator< int > test_iter = rvalue_test_lst.begin();
  for (size_t i = 0; i < 5; ++i, ++test_iter)
  {
    BOOST_TEST(*test_iter == i);
  }
}

BOOST_AUTO_TEST_CASE(assignment_operator_initlist)
{
  std::initializer_list< int > init_list_data_arr = {0, 1, 2, 3, 4};
  zakirov::FwdList< int > init_list_test_lst = init_list_data_arr;
  BOOST_TEST(init_list_test_lst.empty() == false);
  BOOST_TEST(init_list_test_lst.size() == 5);
  zakirov::FwdIterator< int > test_iter = init_list_test_lst.begin();
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
