#include <boost/test/unit_test.hpp>
#include "tree.hpp"
#include "tree_iterator.hpp"

BOOST_AUTO_TEST_CASE(begin_test)
{
  lanovenko::Tree< int, std::string, std::less< int > > tree{};
  int matrix[] = {8, 6, 10, 12, 27, 30};
  for (size_t i = 0; i < sizeof(matrix)/sizeof(matrix[0]); i++)
  {
    tree.insert({matrix[i], ""});
  }
  lanovenko::TreeIterator< int, std::string, std::less< int > > b = tree.begin();
  BOOST_TEST((*b).first == 6);
}

BOOST_AUTO_TEST_CASE(size_empty_test)
{
  lanovenko::Tree< int, std::string, std::less< int > > tree{};
  int matrix[] = {8, 6, 10, 12, 27, 30};
  for (size_t i = 0; i < sizeof(matrix)/sizeof(matrix[0]); i++)
  {
    tree.insert({matrix[i], ""});
  }
  BOOST_TEST(tree.size() == 6);
  BOOST_TEST(tree.empty() == false);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  lanovenko::Tree< int, std::string, std::less< int > > tree{};
  int matrix[] = {8, 6, 10, 12, 27, 30};
  for (size_t i = 0; i < sizeof(matrix)/sizeof(matrix[0]); i++)
  {
    tree.insert({matrix[i], ""});
  }
  lanovenko::Tree< int, std::string, std::less< int > > tree2{};
  tree.swap(tree2);
  BOOST_TEST(tree.size() == 0);
  BOOST_TEST(tree2.size() != 0);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  lanovenko::Tree< int, std::string, std::less< int > > tree{};
  int matrix[] = {8, 6, 10, 12, 27, 30};
  for (size_t i = 0; i < sizeof(matrix)/sizeof(matrix[0]); i++)
  {
    tree.insert({matrix[i], ""});
  }
  tree.clear();
  BOOST_TEST(tree.size() == 0);
}


BOOST_AUTO_TEST_CASE(find_test)
{
  lanovenko::Tree< int, std::string, std::less< int > > tree{};
  int matrix[] = {8, 6, 10, 12, 27, 30};
  for (size_t i = 0; i < sizeof(matrix)/sizeof(matrix[0]); i++)
  {
    tree.insert({matrix[i], ""});
  }
  auto it = tree.find(8);
  BOOST_TEST((*it).first == 8);
}

BOOST_AUTO_TEST_CASE(erase_test)
{
  lanovenko::Tree< int, std::string, std::less< int > > tree{};
  int matrix[] = {8, 6, 10, 12, 27, 30};
  for(size_t i = 0; i < sizeof(matrix)/sizeof(matrix[0]); i++)
  {
    tree.insert({matrix[i], ""});
  }
  for (auto it = tree.begin(); it != tree.end(); it++)
  {
    tree.erase(it);
  }
}
