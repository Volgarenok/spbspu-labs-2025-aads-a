#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
#include <AVLtree.hpp>

using namespace tkach;
template< class Key, class Value >
void print(const AvlTree< Key, Value >& avltree, std::ostream& out)
{
  out << avltree.cbegin()->first << " " << avltree.cbegin()->second;
  for (auto it = ++avltree.cbegin(); it != avltree.cend(); ++it)
  {
    out << " " << it->first << " " << it->second;
  }
}

BOOST_AUTO_TEST_CASE(basic_contructor_test)
{
  AvlTree< size_t, std::string > tree;
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
}

BOOST_AUTO_TEST_CASE(tree_insert_rvalue_test)
{
  AvlTree< size_t, std::string > tree;
  tree.insert(std::make_pair(2, "second"));
  BOOST_TEST(!tree.empty());
  BOOST_TEST(tree.size() == 1);
  tree.insert(std::make_pair(1, "first"));
  BOOST_TEST(tree.size() == 2);
  tree.insert(std::make_pair(3, "third"));
  BOOST_TEST(tree.size() == 3);
  std::ostringstream out;
  print(tree, out);
  BOOST_TEST(out.str() == "1 first 2 second 3 third");
}

BOOST_AUTO_TEST_CASE(tree_insert_lvalue_test)
{
  AvlTree< size_t, std::string > tree;
  std::pair< size_t, std::string > pair = std::make_pair(2, "second");
  tree.insert(pair);
  BOOST_TEST(!tree.empty());
  BOOST_TEST(tree.size() == 1);
  pair = std::make_pair(1, "first");
  tree.insert(pair);
  BOOST_TEST(tree.size() == 2);
  pair = std::make_pair(3, "third");
  tree.insert(pair);
  BOOST_TEST(tree.size() == 3);
  std::ostringstream out;
  print(tree, out);
  BOOST_TEST(out.str() == "1 first 2 second 3 third");
}

BOOST_AUTO_TEST_CASE(copy_contructor_test)
{
  AvlTree< size_t, std::string > tree;
  for (size_t i = 0; i <= 5; ++i)
  {
    tree.insert(std::make_pair(i, std::to_string(i)));
  }
  AvlTree< size_t, std::string > tree2(tree);
  BOOST_TEST(tree2.size() == 6);
  std::ostringstream out;
  print(tree2, out);
  BOOST_TEST(out.str() == "0 0 1 1 2 2 3 3 4 4 5 5");
}

BOOST_AUTO_TEST_CASE(copy_move_contructor_test)
{
  AvlTree< size_t, std::string > tree;
  for (size_t i = 0; i <= 5; ++i)
  {
    tree.insert(std::make_pair(i, std::to_string(i)));
  }
  AvlTree< size_t, std::string > tree2(std::move(tree));
  BOOST_TEST(tree2.size() == 6);
  std::ostringstream out;
  print(tree2, out);
  BOOST_TEST(out.str() == "0 0 1 1 2 2 3 3 4 4 5 5");
}

BOOST_AUTO_TEST_CASE(tree_equal_operator_test)
{
  AvlTree< size_t, std::string > tree;
  for (size_t i = 0; i <= 5; ++i)
  {
    tree.insert(std::make_pair(i, std::to_string(i)));
  }
  AvlTree< size_t, std::string > tree2;
  tree2 = tree;
  BOOST_TEST(tree2.size() == 6);
  std::ostringstream out;
  print(tree2, out);
  BOOST_TEST(out.str() == "0 0 1 1 2 2 3 3 4 4 5 5");
}

BOOST_AUTO_TEST_CASE(tree_equal_move_operator_test)
{
  AvlTree< size_t, std::string > tree;
  for (size_t i = 0; i <= 5; ++i)
  {
    tree.insert(std::make_pair(i, std::to_string(i)));
  }
  AvlTree< size_t, std::string > tree2;
  tree2 = std::move(tree);
  BOOST_TEST(tree2.size() == 6);
  std::ostringstream out;
  print(tree2, out);
  BOOST_TEST(out.str() == "0 0 1 1 2 2 3 3 4 4 5 5");
}

BOOST_AUTO_TEST_CASE(tree_clear_test)
{
  AvlTree< size_t, std::string > tree;
  for (size_t i = 0; i <= 5; ++i)
  {
    tree.insert(std::make_pair(i, std::to_string(i)));
  }
  BOOST_TEST(tree.size() == 6);
  tree.clear();
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(tree_find_test)
{
  AvlTree< size_t, std::string > tree;
  for (size_t i = 0; i <= 5; ++i)
  {
    tree.insert(std::make_pair(i, std::to_string(i)));
  }
  BOOST_TEST((tree.find(6) == tree.end()));
  BOOST_TEST((tree.find(5) != tree.end()));
}

BOOST_AUTO_TEST_CASE(tree_const_find_test)
{
  AvlTree< size_t, std::string > tree;
  for (size_t i = 0; i <= 5; ++i)
  {
    tree.insert(std::make_pair(i, std::to_string(i)));
  }
  BOOST_TEST((static_cast< const AvlTree< size_t, std::string > >(tree).find(6) == tree.cend()));
  BOOST_TEST((static_cast< const AvlTree< size_t, std::string > >(tree).find(5) != tree.cend()));
}

BOOST_AUTO_TEST_CASE(tree_at_test)
{
  AvlTree< size_t, std::string > tree;
  for (size_t i = 0; i <= 5; ++i)
  {
    tree.insert(std::make_pair(i, std::to_string(i)));
  }
  for (size_t i = 0; i <= 5; ++i)
  {
    BOOST_TEST(tree.at(i) == std::to_string(i));
  }
  try
  {
    BOOST_TEST(tree.at(6) == std::to_string(6));
  }
  catch (...)
  {
    BOOST_TEST(true);
  }
}

BOOST_AUTO_TEST_CASE(tree_at_const_test)
{
  AvlTree< size_t, std::string > tree;
  for (size_t i = 0; i <= 5; ++i)
  {
    tree.insert(std::make_pair(i, std::to_string(i)));
  }
  for (size_t i = 0; i <= 5; ++i)
  {
    BOOST_TEST((static_cast< const AvlTree< size_t, std::string > >(tree).at(i) == std::to_string(i)));
  }
}

BOOST_AUTO_TEST_CASE(tree_begin_test)
{
  AvlTree< size_t, std::string > tree;
  for (size_t i = 0; i <= 5; ++i)
  {
    tree.insert(std::make_pair(i, std::to_string(i)));
  }
  Iterator< size_t, std::string > it = tree.begin();
  BOOST_TEST((it->first == 0 && it->second == "0"));
}

BOOST_AUTO_TEST_CASE(tree_cbegin_test)
{
  AvlTree< size_t, std::string > tree;
  for (size_t i = 0; i <= 5; ++i)
  {
    tree.insert(std::make_pair(i, std::to_string(i)));
  }
  Citerator< size_t, std::string > it = tree.cbegin();
  BOOST_TEST((it->first == 0 && it->second == "0"));
}

BOOST_AUTO_TEST_CASE(tree_erase_key_test)
{
  AvlTree< size_t, std::string > tree;
  for (size_t i = 0; i <= 5; ++i)
  {
    tree.insert(std::make_pair(i, std::to_string(i)));
  }
  auto it = tree.erase(2);
  std::ostringstream out;
  print(tree, out);
  BOOST_TEST(out.str() == "0 0 1 1 3 3 4 4 5 5");
  BOOST_TEST((it->first == 3 && it->second == "3"));
  BOOST_TEST(tree.size() == 5);
}

BOOST_AUTO_TEST_CASE(tree_erase_it_test)
{
  AvlTree< size_t, std::string > tree;
  for (size_t i = 0; i <= 5; ++i)
  {
    tree.insert(std::make_pair(i, std::to_string(i)));
  }
  auto it2 = ++tree.begin();
  it2++;
  auto it = tree.erase(it2);
  std::ostringstream out;
  print(tree, out);
  BOOST_TEST(out.str() == "0 0 1 1 3 3 4 4 5 5");
  BOOST_TEST((it->first == 3 && it->second == "3"));
  BOOST_TEST(tree.size() == 5);
}

BOOST_AUTO_TEST_CASE(tree_end_test)
{
  AvlTree< size_t, std::string > tree;
  for (size_t i = 0; i <= 5; ++i)
  {
    tree.insert(std::make_pair(i, std::to_string(i)));
  }
  Iterator< size_t, std::string > it2 = tree.end();
  size_t i = 0;
  for (auto it = tree.begin(); it != it2; ++i)
  {
    auto it3 = it;
    if (++it3 == it2)
    {
      BOOST_TEST((it->first == 5 && it->second == "5"));
    }
    it++;
  }
  BOOST_TEST(i == 6);
}

BOOST_AUTO_TEST_CASE(tree_upper_lowes_bound_test)
{
  AvlTree< size_t, std::string > tree;
  for (size_t i = 1; i <= 7; i += 2)
  {
    tree.insert(std::make_pair(i, std::to_string(i)));
  }
  BOOST_TEST(tree.upperBound(5)->first == 7);
  BOOST_TEST(tree.lowerBound(5)->first == 5);
  BOOST_TEST(tree.upperBound(4)->first == 5);
  BOOST_TEST(tree.lowerBound(4)->first == 5);
}

BOOST_AUTO_TEST_CASE(tree_cend_test)
{
  AvlTree< size_t, std::string > tree;
  for (size_t i = 0; i <= 5; ++i)
  {
    tree.insert(std::make_pair(i, std::to_string(i)));
  }
  Citerator< size_t, std::string > it2 = tree.cend();
  size_t i = 0;
  for (auto it = tree.cbegin(); it != it2; ++i)
  {
    auto it3 = it;
    if (++it3 == it2)
    {
      BOOST_TEST((it->first == 5 && it->second == "5"));
    }
    it++;
  }
  BOOST_TEST(i == 6);
}
