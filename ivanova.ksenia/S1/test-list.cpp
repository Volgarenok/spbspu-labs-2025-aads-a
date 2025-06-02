#include <algorithm>
#include <vector>

#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  using list_t = ivanova::List<int>;

  list_t list;
  BOOST_TEST(list.empty());
  BOOST_TEST(list.size() == 0);
  BOOST_TEST(list.begin() == list.end());
}

template <typename Iter1, typename Iter2>
bool areEqual(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2)
{
  while (first1 != last1 && first2 != last2 && *first1 == *first2)
  {
    ++first1;
    ++first2;
  }
  return first1 == last1 && first2 == last2;
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  using list_t = ivanova::List<int>;

  list_t list;
  list.push_back(1);
  list.push_back(2);

  list_t list2 = list;
  BOOST_TEST(list.size() == list2.size());
  BOOST_TEST(areEqual(list.begin(), list.end(), list2.begin(), list2.end()));
  list.push_back(3);
  BOOST_TEST(!areEqual(list.begin(), list.end(), list2.begin(), list2.end()));
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  using list_t = ivanova::List<int>;

  list_t list;
  list.push_back(1);
  list.push_back(2);

  list_t list2 = std::move(list);

  BOOST_TEST(list.empty());
  BOOST_TEST(list2.size() == 2);

  std::vector<int> check = {1, 2};
  BOOST_TEST(areEqual(list.begin(), list.end(), check.begin(), check.end()));
}
