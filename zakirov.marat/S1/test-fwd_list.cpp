#include <boost/test/unit_test.hpp>
#include <list>
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

BOOST_AUTO_TEST_CASE(assignment_operator_lvalue_same)
{
  zakirov::FwdList< int > lvalue_data_lst({0, 1, 2, 3, 4});
  lvalue_data_lst = lvalue_data_lst;
  BOOST_TEST(lvalue_data_lst.empty() == false);
  BOOST_TEST(lvalue_data_lst.size() == 5);
  zakirov::FwdIterator< int > test_iter = lvalue_data_lst.begin();
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
  BOOST_TEST(rvalue_data_lst.empty() == true);
  BOOST_TEST(rvalue_data_lst.size() == 0);
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

BOOST_AUTO_TEST_CASE(front)
{
  zakirov::FwdList< int > front_test = {0, 1, 2, 3, 4};
  int value = front_test.front();
  BOOST_TEST(value == 0);
}

BOOST_AUTO_TEST_CASE(const_front)
{
  const zakirov::FwdList< int > front_test = {0, 1, 2, 3, 4};
  int value = front_test.front();
  BOOST_TEST(value == 0);
}

BOOST_AUTO_TEST_CASE(begin)
{
  zakirov::FwdList< int > begin_test = {0, 1, 2, 3, 4};
  zakirov::FwdIterator< int > begin_iter = begin_test.begin();
  BOOST_TEST(*begin_iter == 0);
  ++begin_iter;
  BOOST_TEST(*begin_iter == 1);
}

BOOST_AUTO_TEST_CASE(cbegin)
{
  zakirov::FwdList< int > begin_test = {0, 1, 2, 3, 4};
  zakirov::CFwdIterator< int > begin_iter = begin_test.cbegin();
  BOOST_TEST(*begin_iter == 0);
  ++begin_iter;
  BOOST_TEST(*begin_iter == 1);
}

BOOST_AUTO_TEST_CASE(before_begin)
{
  zakirov::FwdList< int > before_begin_test = {0, 1, 2, 3, 4};
  zakirov::FwdIterator< int > before_begin_iter = before_begin_test.before_begin();
  ++before_begin_iter;
  BOOST_TEST(*before_begin_iter == 0);
  ++before_begin_iter;
  BOOST_TEST(*before_begin_iter == 1);
}

BOOST_AUTO_TEST_CASE(cbefore_begin)
{
  zakirov::FwdList< int > before_begin_test = {0, 1, 2, 3, 4};
  zakirov::CFwdIterator< int > cbefore_begin_iter = before_begin_test.cbefore_begin();
  ++cbefore_begin_iter;
  BOOST_TEST(*cbefore_begin_iter == 0);
  ++cbefore_begin_iter;
  BOOST_TEST(*cbefore_begin_iter == 1);
}

BOOST_AUTO_TEST_CASE(end)
{
  zakirov::FwdList< int > end_test = {0, 1, 2, 3, 4};
  zakirov::FwdIterator< int > end_iter = end_test.end();
  ++end_iter;
  BOOST_TEST(*end_iter == 0);
  ++end_iter;
  BOOST_TEST(*end_iter == 1);
}

BOOST_AUTO_TEST_CASE(cend)
{
  zakirov::FwdList< int > cend_test = {0, 1, 2, 3, 4};
  zakirov::CFwdIterator< int > cend_iter = cend_test.cend();
  ++cend_iter;
  BOOST_TEST(*cend_iter == 0);
  ++cend_iter;
  BOOST_TEST(*cend_iter == 1);
}

BOOST_AUTO_TEST_CASE(push_front_lvalue)
{
  zakirov::FwdList< int > push_test = {0, 1, 2, 3, 4};
  BOOST_TEST(push_test.size() == 5);
  int value = 10;
  push_test.push_front(value);
  BOOST_TEST(push_test.size() == 6);
  BOOST_TEST(push_test.front() == 10);
}

BOOST_AUTO_TEST_CASE(push_front_rvalue)
{
  zakirov::FwdList< int > push_test = {0, 1, 2, 3, 4};
  BOOST_TEST(push_test.size() == 5);
  int value = 10;
  push_test.push_front(std::move(value));
  BOOST_TEST(push_test.size() == 6);
  BOOST_TEST(push_test.front() == 10);
}

BOOST_AUTO_TEST_CASE(pop_front)
{
  zakirov::FwdList< int > push_test = {0, 1, 2, 3, 4};
  BOOST_TEST(push_test.size() == 5);
  BOOST_TEST(push_test.front() == 0);
  push_test.pop_front();
  BOOST_TEST(push_test.size() == 4);
  BOOST_TEST(push_test.front() == 1);
}

BOOST_AUTO_TEST_CASE(insert_after)
{
  zakirov::FwdList< int > insert_test = {0, 1, 2, 3, 4};
  BOOST_TEST(insert_test.size() == 5);
  zakirov::FwdIterator< int > iter = insert_test.begin();
  insert_test.insert_after(iter, 10);
  ++iter;
  BOOST_TEST(insert_test.size() == 6);
  BOOST_TEST(*iter == 10);
}

BOOST_AUTO_TEST_CASE(erase_after)
{
  zakirov::FwdList< int > erase_test = {0, 1, 2, 3, 4};
  BOOST_TEST(erase_test.size() == 5);
  zakirov::FwdIterator< int > iter = erase_test.begin();
  erase_test.erase_after(iter);
  ++iter;
  BOOST_TEST(erase_test.size() == 4);
  BOOST_TEST(*iter == 2);
}

BOOST_AUTO_TEST_CASE(empty)
{
  zakirov::FwdList< int > test_list;
  BOOST_TEST(test_list.empty() == true);
  zakirov::FwdList< int > test_list_2({0, 1, 2, 3, 4});
  BOOST_TEST(test_list_2.empty() == false);
}

BOOST_AUTO_TEST_CASE(size)
{
  zakirov::FwdList< int > test_list;
  BOOST_TEST(test_list.size() == 0);
  zakirov::FwdList< int > test_list_2({0, 1, 2, 3, 4});
  BOOST_TEST(test_list_2.size() == 5);
}

BOOST_AUTO_TEST_CASE(swap)
{
  zakirov::FwdList< int > first_swap({1});
  zakirov::FwdList< int > second_swap({2, 3});
  BOOST_TEST(first_swap.front() == 1);
  BOOST_TEST(first_swap.size() == 1);
  BOOST_TEST(second_swap.front() == 2);
  BOOST_TEST(second_swap.size() == 2);
  first_swap.swap(second_swap);
  BOOST_TEST(first_swap.front() == 2);
  BOOST_TEST(first_swap.size() == 2);
  BOOST_TEST(second_swap.front() == 1);
  BOOST_TEST(second_swap.size() == 1);
}

BOOST_AUTO_TEST_CASE(splice_after_element)
{
  zakirov::FwdList< int > base({3, 4, 5});
  zakirov::FwdList< int > splice({0, 1, 2});
  BOOST_TEST(base.front() == 3);
  BOOST_TEST(base.size() == 3);
  BOOST_TEST(splice.front() == 0);
  BOOST_TEST(splice.size() == 3);
  zakirov::FwdIterator< int > iter = splice.before_begin();
  base.splice_after(base.before_begin(), splice, iter);
  BOOST_TEST(base.front() == 0);
  BOOST_TEST(base.size() == 4);
  BOOST_TEST(splice.front() == 1);
  BOOST_TEST(splice.size() == 2);
}

BOOST_AUTO_TEST_CASE(splice_fake_after_element)
{
  zakirov::FwdList< int > base({3, 4, 5});
  zakirov::FwdList< int > splice({0, 1, 2});
  BOOST_TEST(base.front() == 3);
  BOOST_TEST(base.size() == 3);
  BOOST_TEST(splice.front() == 0);
  BOOST_TEST(splice.size() == 3);
  zakirov::FwdIterator< int > iter = splice.begin();
  ++iter;
  ++iter;
  base.splice_after(base.before_begin(), splice, iter);
  BOOST_TEST(base.front() == 3);
  BOOST_TEST(base.size() == 3);
  BOOST_TEST(splice.front() == 0);
  BOOST_TEST(splice.size() == 3);
}


BOOST_AUTO_TEST_CASE(splice_after_range)
{
  zakirov::FwdList< int > base({3, 4, 5});
  zakirov::FwdList< int > splice({0, 1, 2});
  BOOST_TEST(base.front() == 3);
  BOOST_TEST(base.size() == 3);
  BOOST_TEST(splice.front() == 0);
  BOOST_TEST(splice.size() == 3);
  zakirov::FwdIterator< int > iter_first = splice.before_begin();
  zakirov::FwdIterator< int > iter_last = splice.begin();
  ++iter_last;
  ++iter_last;
  base.splice_after(base.before_begin(), splice, iter_first, iter_last);
  BOOST_TEST(base.front() == 0);
  BOOST_TEST(base.size() == 5);
  BOOST_TEST(splice.front() == 2);
  BOOST_TEST(splice.size() == 1);
}

BOOST_AUTO_TEST_CASE(splice_empty_after_range)
{
  zakirov::FwdList< int > base({3, 4, 5});
  zakirov::FwdList< int > splice({0, 1, 2});
  BOOST_TEST(base.front() == 3);
  BOOST_TEST(base.size() == 3);
  BOOST_TEST(splice.front() == 0);
  BOOST_TEST(splice.size() == 3);
  zakirov::FwdIterator< int > iter_first = splice.before_begin();
  zakirov::FwdIterator< int > iter_last = splice.before_begin();
  ++iter_last;
  base.splice_after(base.before_begin(), splice, iter_first, iter_last);
  BOOST_TEST(base.front() == 3);
  BOOST_TEST(base.size() == 3);
  BOOST_TEST(splice.front() == 0);
  BOOST_TEST(splice.size() == 3);
}

BOOST_AUTO_TEST_CASE(splice_after_full)
{
  zakirov::FwdList< int > base({3, 4, 5});
  zakirov::FwdList< int > splice({0, 1, 2});
  BOOST_TEST(base.front() == 3);
  BOOST_TEST(base.size() == 3);
  BOOST_TEST(splice.front() == 0);
  BOOST_TEST(splice.size() == 3);
  zakirov::FwdIterator< int > iter = base.before_begin();
  base.splice_after(iter, splice);
  BOOST_TEST(base.front() == 0);
  BOOST_TEST(base.size() == 6);
  BOOST_TEST(splice.size() == 0);
}


BOOST_AUTO_TEST_CASE(splice_after_rvalue_element)
{
  zakirov::FwdList< int > base({3, 4, 5});
  zakirov::FwdList< int > splice({0, 1, 2});
  BOOST_TEST(base.front() == 3);
  BOOST_TEST(base.size() == 3);
  BOOST_TEST(splice.front() == 0);
  BOOST_TEST(splice.size() == 3);
  zakirov::FwdIterator< int > iter = splice.begin();
  ++iter;
  ++iter;
  base.splice_after(base.before_begin(), std::move(splice), iter);
  BOOST_TEST(base.front() == 3);
  BOOST_TEST(base.size() == 3);
  BOOST_TEST(splice.front() == 0);
  BOOST_TEST(splice.size() == 3);
}

BOOST_AUTO_TEST_CASE(splice_after_rvalue_range)
{
  zakirov::FwdList< int > base({3, 4, 5});
  zakirov::FwdList< int > splice({0, 1, 2});
  BOOST_TEST(base.front() == 3);
  BOOST_TEST(base.size() == 3);
  BOOST_TEST(splice.front() == 0);
  BOOST_TEST(splice.size() == 3);
  zakirov::FwdIterator< int > iter_first = splice.before_begin();
  zakirov::FwdIterator< int > iter_last = splice.begin();
  ++iter_last;
  ++iter_last;
  base.splice_after(base.before_begin(), std::move(splice), iter_first, iter_last);
  BOOST_TEST(base.front() == 0);
  BOOST_TEST(base.size() == 5);
  BOOST_TEST(splice.front() == 2);
  BOOST_TEST(splice.size() == 1);
}

BOOST_AUTO_TEST_CASE(splice_after_rvalue_full)
{
  zakirov::FwdList< int > base({3, 4, 5});
  zakirov::FwdList< int > splice({0, 1, 2});
  BOOST_TEST(base.front() == 3);
  BOOST_TEST(base.size() == 3);
  BOOST_TEST(splice.front() == 0);
  BOOST_TEST(splice.size() == 3);
  zakirov::FwdIterator< int > iter = base.before_begin();
  base.splice_after(iter, std::move(splice));
  BOOST_TEST(base.front() == 0);
  BOOST_TEST(base.size() == 6);
  BOOST_TEST(splice.size() == 0);
}

BOOST_AUTO_TEST_CASE(assign)
{
  constexpr size_t quantity = 5;
  constexpr int value = 1;
  zakirov::FwdList< int > base({0, 1, 2});
  BOOST_TEST(base.front() == 0);
  BOOST_TEST(base.size() == 3);
  base.assign(quantity, value);
  BOOST_TEST(base.front() == 1);
  BOOST_TEST(base.size() == 5);
}

BOOST_AUTO_TEST_CASE(assign_range)
{
  zakirov::FwdList< int > base({3, 4, 5});
  zakirov::FwdList< int > splice({0, 1, 2});
  BOOST_TEST(base.front() == 3);
  BOOST_TEST(base.size() == 3);
  zakirov::FwdIterator< int > iter = splice.begin();
  ++iter;
  base.assign(splice.begin(), iter);
  BOOST_TEST(base.front() == 0);
  BOOST_TEST(base.size() == 1);
}

BOOST_AUTO_TEST_CASE(assign_init_list)
{
  zakirov::FwdList< int > base({0, 1, 2});
  BOOST_TEST(base.front() == 0);
  BOOST_TEST(base.size() == 3);
  base.assign({3, 4, 5, 6});
  BOOST_TEST(base.front() == 3);
  BOOST_TEST(base.size() == 4);
}

BOOST_AUTO_TEST_CASE(clear)
{
  zakirov::FwdList< int > base({0, 1, 2});
  BOOST_TEST(base.front() == 0);
  BOOST_TEST(base.size() == 3);
  base.clear();
  BOOST_TEST(base.empty() == true);
}

BOOST_AUTO_TEST_CASE(remove_if)
{
  zakirov::FwdList< int > base({8, 0, 8, 8, 1, 8, 2, 4});
  BOOST_TEST(base.front() == 8);
  BOOST_TEST(base.size() == 8);
  base.remove_if([&](const int & value)
  {
    return value % 2 == 0;
  });
  BOOST_TEST(base.front() == 1);
  BOOST_TEST(base.size() == 1);
}

BOOST_AUTO_TEST_CASE(remove_test)
{
  zakirov::FwdList< int > base({8, 0, 8, 8, 1, 8, 2, 4});
  BOOST_TEST(base.front() == 8);
  BOOST_TEST(base.size() == 8);
  base.remove(8);
  BOOST_TEST(base.front() == 0);
  BOOST_TEST(base.size() == 4);
}
