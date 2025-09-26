#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>
#include <stdexcept>
#include "avl_tree.hpp"

using namespace karnauhova;

BOOST_AUTO_TEST_SUITE(avl_tree)

BOOST_AUTO_TEST_CASE(constructors_and_assignment)
{
  AvlTree< int, std::string > tree1;
  BOOST_TEST(tree1.empty());

  tree1.insert({1, "one"});
  tree1.insert({2, "two"});

  AvlTree< int, std::string > tree2(tree1);
  BOOST_TEST(tree2.size() == 2);
  BOOST_TEST(tree2.count(1) == 1);
  BOOST_TEST(tree2.count(2) == 1);

  AvlTree< int, std::string > tree3(std::move(tree1));
  BOOST_TEST(tree3.size() == 2);
  BOOST_TEST(tree1.empty());

  tree1 = tree2;
  BOOST_TEST(tree1.size() == 2);
  BOOST_TEST(tree1.count(1) == 1);
}

BOOST_AUTO_TEST_CASE(element_access_operations)
{
  AvlTree< int, std::string > tree;
  tree.insert({1, "one"});
  tree.insert({2, "two"});

  BOOST_TEST(tree[1] == "one");
  BOOST_TEST(tree[2] == "two");

  tree[1] = "modified";
  BOOST_TEST(tree[1] == "modified");

  BOOST_TEST(tree.at(2) == "two");
  BOOST_CHECK_THROW(tree.at(3), std::out_of_range);

  const auto& const_tree = tree;
  BOOST_TEST(const_tree.at(1) == "modified");
}

BOOST_AUTO_TEST_CASE(insert_operations)
{
  AvlTree< int, std::string > tree;

  auto result1 = tree.insert({1, "one"});
  BOOST_TEST(result1.second == true);
  BOOST_TEST(result1.first->first == 1);

  auto result2 = tree.insert({1, "duplicate"});
  BOOST_TEST(result2.second == false);

  tree.insert({{2, "two"}, {3, "three"}});
  BOOST_TEST(tree.size() == 3);
  BOOST_TEST(tree.count(2) == 1);
  BOOST_TEST(tree.count(3) == 1);

  std::vector< std::pair< int, std::string > > vec = {{4, "four"}, {5, "five"}};
  tree.insert(vec.begin(), vec.end());
  BOOST_TEST(tree.size() == 5);
  BOOST_TEST(tree.count(4) == 1);
  BOOST_TEST(tree.count(5) == 1);
}

BOOST_AUTO_TEST_CASE(erase_operations)
{
  AvlTree< int, std::string > tree;
  for (int i = 1; i <= 5; ++i) tree.insert({i, "value"});

  auto it = tree.find(2);
  tree.erase(it);
  BOOST_TEST(tree.size() == 4);
  BOOST_TEST(tree.count(2) == 0);

  size_t removed = tree.erase(3);
  BOOST_TEST(removed == 1);
  BOOST_TEST(tree.size() == 3);
  BOOST_TEST(tree.count(3) == 0);

  auto first = tree.find(1);
  auto last = tree.find(4);
  tree.erase(first, last);
  BOOST_TEST(tree.count(1) == 0);
  BOOST_TEST(tree.count(2) == 0);
  BOOST_TEST(tree.count(3) == 0);
  BOOST_TEST(tree.count(4) == 1);
}

BOOST_AUTO_TEST_CASE(iteration_and_traversal)
{
  AvlTree< int, std::string > tree;
  tree.insert({2, "two"});
  tree.insert({1, "one"});
  tree.insert({3, "three"});

  int const_count = 0;
  for (auto it = tree.cbegin(); it != tree.cend(); ++it) {
    const_count++;
  }
  BOOST_TEST(const_count == 3);
}

BOOST_AUTO_TEST_CASE(lookup_operations)
{
  AvlTree< int, std::string > tree;
  tree.insert({1, "one"});
  tree.insert({2, "two"});

  BOOST_TEST(tree.count(1) == 1);
  BOOST_TEST(tree.count(2) == 1);
  BOOST_TEST(tree.count(3) == 0);

  auto found = tree.find(2);
  BOOST_TEST(found->first == 2);
  BOOST_TEST(found->second == "two");

  BOOST_TEST(tree.count(5) == 0);
}

BOOST_AUTO_TEST_CASE(tree_modifications)
{
  AvlTree< int, std::string > tree;
  tree.insert({1, "one"});
  tree.insert({2, "two"});

  tree.clear();
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);

  tree.insert({3, "three"});
  AvlTree< int, std::string > other;
  other.insert({4, "four"});

  tree.swap(other);
  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.count(4) == 1);
  BOOST_TEST(other.size() == 1);
  BOOST_TEST(other.count(3) == 1);
}

BOOST_AUTO_TEST_CASE(bound_operations)
{
  AvlTree< int, std::string > tree;
  tree.insert({1, "one"});
  tree.insert({3, "three"});
  tree.insert({5, "five"});

  auto lb = tree.lower_bound(2);
  BOOST_TEST(lb->first == 3);

  auto ub = tree.upper_bound(3);
  BOOST_TEST(ub->first == 5);

  auto range = tree.equal_range(3);
  BOOST_TEST(range.first->first == 3);
  BOOST_TEST(range.second->first == 5);

  auto rrange = tree.equal_range(3);
  BOOST_TEST(rrange.first->first == 3);
  BOOST_TEST(rrange.first->second == "three");
  BOOST_TEST(rrange.second->first == 5);
  BOOST_TEST(rrange.second->second == "five");
}

BOOST_AUTO_TEST_SUITE_END()
