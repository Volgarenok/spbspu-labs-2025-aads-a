#include <boost/test/unit_test.hpp>
#include <sstream>
#include <random>
#include <avl_tree.hpp>

BOOST_AUTO_TEST_SUITE(stress_tests)

BOOST_AUTO_TEST_CASE(stress_test)
{
  std::ostringstream out;
  petrov::AVLTree< int, int > tree;
  for (size_t i = 0; i < 1000000; ++i)
  {
    tree.insert({ i, i });
  }
  tree.clear();
  out << tree.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(stress_test_2)
{
  std::ostringstream out;
  petrov::AVLTree< int, int > tree;
  for (size_t i = 1000000; i != 0; --i)
  {
    tree.insert({ i, i });
  }
  tree.clear();
  out << tree.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(stress_test_3)
{
  std::ostringstream out;
  petrov::AVLTree< int, int > tree;
  for (size_t i = 0; i < 53; ++i)
  {
    tree.insert({ i, i });
  }
  tree.clear();
  out << tree.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(stress_test_7)
{
  std::ostringstream out;
  petrov::AVLTree< int, int > tree;
  for (size_t i = 0; i < 500; ++i)
  {
    tree.insert({ rand(), i });
  }
  tree.clear();
  out << tree.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_SUITE_END()
