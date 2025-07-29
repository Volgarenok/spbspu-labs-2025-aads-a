#include <boost/test/unit_test.hpp>
#include <numeric>
#include <vector>
#include <tree/tree.hpp>

using aleksandrov::Tree;

BOOST_AUTO_TEST_CASE(traverse_LNR)
{
  Tree< int, int > tree;
  std::vector< int > keys;
  keys = { 1, 2, 3, 4, 5, 7, 8, 10, 11, 6, 9, 12 };
  for (size_t i = 0; i < keys.size(); ++i)
  {
    tree.insert({ keys[i], 0 });
  }
  std::vector< int > expected(12);
  std::iota(expected.begin(), expected.end(), 1);

  std::vector< int > result;
  for (auto it = tree.beginLNR(); it != tree.endLNR(); ++it)
  {
    result.push_back(it->first);
  }
  BOOST_TEST(std::equal(result.begin(), result.end(), expected.begin()));
}

