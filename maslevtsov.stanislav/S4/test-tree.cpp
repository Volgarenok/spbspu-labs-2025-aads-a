#include <boost/test/unit_test.hpp>
#include <tree/definition.hpp>

BOOST_AUTO_TEST_SUITE(constructors_tests)
BOOST_AUTO_TEST_CASE(default_constructor_test)
{
  maslevtsov::Tree< int, int > tree;
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  maslevtsov::Tree< int, int > tree1;
  maslevtsov::Tree< int, int > tree2(tree1);
  BOOST_TEST(tree2.empty());
  tree1.insert(std::make_pair(0, 0));
  tree1.insert(std::make_pair(1, 1));
  maslevtsov::Tree< int, int > tree3(tree1);
  auto it = tree3.begin();
  BOOST_TEST(it->first == 0);
  BOOST_TEST(it->second == 0);
  ++it;
  BOOST_TEST(it->first == 1);
  BOOST_TEST(it->second == 1);
  BOOST_TEST(tree3.size() == 2);
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  maslevtsov::Tree< int, int > tree1;
  tree1.insert(std::make_pair(0, 0));
  tree1.insert(std::make_pair(1, 1));
  maslevtsov::Tree< int, int > tree2(std::move(tree1));
  auto it = tree2.begin();
  BOOST_TEST(it->first == 0);
  BOOST_TEST(it->second == 0);
  ++it;
  BOOST_TEST(it->first == 1);
  BOOST_TEST(it->second == 1);
  BOOST_TEST(tree2.size() == 2);
  BOOST_TEST(tree1.empty());
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(assignment_operators_tests)
BOOST_AUTO_TEST_CASE(copy_assignment_operator_test)
{
  maslevtsov::Tree< int, int > tree1;
  tree1.insert(std::make_pair(0, 0));
  tree1.insert(std::make_pair(1, 1));
  maslevtsov::Tree< int, int > tree2;
  tree2 = tree1;
  auto it = tree2.begin();
  BOOST_TEST(it->first == 0);
  BOOST_TEST(it->second == 0);
  ++it;
  BOOST_TEST(it->first == 1);
  BOOST_TEST(it->second == 1);
  BOOST_TEST(tree2.size() == 2);
}

BOOST_AUTO_TEST_CASE(move_assignment_operator_test)
{
  maslevtsov::Tree< int, int > tree1;
  tree1.insert(std::make_pair(0, 0));
  tree1.insert(std::make_pair(1, 1));
  maslevtsov::Tree< int, int > tree2;
  tree2 = std::move(tree1);
  auto it = tree2.begin();
  BOOST_TEST(it->first == 0);
  BOOST_TEST(it->second == 0);
  ++it;
  BOOST_TEST(it->first == 1);
  BOOST_TEST(it->second == 1);
  BOOST_TEST(tree2.size() == 2);
  BOOST_TEST(tree1.empty());
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(element_access_tests)
BOOST_AUTO_TEST_CASE(brackets_operator_test)
{
  maslevtsov::Tree< int, int > tree;
  tree.insert(std::make_pair(0, 0));
  tree[0] = 1;
  BOOST_TEST(tree.begin()->first == 0);
  BOOST_TEST(tree.begin()->second == 1);
  BOOST_TEST(tree.size() == 1);
  tree[1] = 1;
  BOOST_TEST((++tree.begin())->first == 1);
  BOOST_TEST((++tree.begin())->second == 1);
  BOOST_TEST(tree.size() == 2);
}

BOOST_AUTO_TEST_CASE(at_test)
{
  maslevtsov::Tree< int, int > tree;
  tree.insert(std::make_pair(0, 1));
  try {
    tree.at(1);
  } catch (const std::out_of_range&) {
    BOOST_TEST(true);
  }
  BOOST_TEST(tree.at(0) == 1);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(iterators_tests)
BOOST_AUTO_TEST_CASE(begin_test)
{
  maslevtsov::Tree< int, int > tree;
  tree[0] = 0;
  BOOST_TEST(tree.begin()->first == 0);
  BOOST_TEST(tree.begin()->second == 0);
  BOOST_TEST(tree.cbegin()->first == 0);
  BOOST_TEST(tree.cbegin()->second == 0);
}

BOOST_AUTO_TEST_CASE(end_test)
{
  maslevtsov::Tree< int, int > tree;
  tree[0] = 0;
  tree[1] = 1;
  BOOST_TEST((--tree.end())->first == 1);
  BOOST_TEST((--tree.end())->second == 1);
  BOOST_TEST((--tree.cend())->first == 1);
  BOOST_TEST((--tree.cend())->second == 1);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(capacity_tests)
BOOST_AUTO_TEST_CASE(empty_test)
{
  maslevtsov::Tree< int, int > tree;
  BOOST_TEST(tree.empty());
  tree[0] = 0;
  BOOST_TEST(!tree.empty());
}

BOOST_AUTO_TEST_CASE(size_test)
{
  maslevtsov::Tree< int, int > tree;
  tree[0] = 0;
  BOOST_TEST(tree.size() == 1);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(modifiers_tests)
BOOST_AUTO_TEST_CASE(clear_test)
{
  maslevtsov::Tree< int, int > tree;
  tree.clear();
  BOOST_TEST(tree.empty());
  tree[0] = 0;
  tree.clear();
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(insert_test)
{
  maslevtsov::Tree< int, int > tree;

  auto it = tree.begin();
  it = tree.insert(std::make_pair(0, 0)).first;
  BOOST_TEST(it->first == 0);
  BOOST_TEST(it->second == 0);
  BOOST_TEST(tree.size() == 1);

  it = tree.insert(std::make_pair(1, 1)).first;
  BOOST_TEST(it->first == 1);
  BOOST_TEST(it->second == 1);
  BOOST_TEST(tree.size() == 2);

  it = tree.insert(std::make_pair(2, 2)).first;
  BOOST_TEST(it->first == 2);
  BOOST_TEST(it->second == 2);
  BOOST_TEST(tree.size() == 3);

  it = tree.insert(std::make_pair(3, 3)).first;
  BOOST_TEST(it->first == 3);
  BOOST_TEST(it->second == 3);
  BOOST_TEST(tree.size() == 4);

  it = tree.insert(std::make_pair(4, 4)).first;
  BOOST_TEST(it->first == 4);
  BOOST_TEST(it->second == 4);
  BOOST_TEST(tree.size() == 5);

  it = tree.insert(std::make_pair(5, 5)).first;
  BOOST_TEST(it->first == 5);
  BOOST_TEST(it->second == 5);
  BOOST_TEST(tree.size() == 6);

  it = tree.insert(std::make_pair(6, 6)).first;
  BOOST_TEST(it->first == 6);
  BOOST_TEST(it->second == 6);
  BOOST_TEST(tree.size() == 7);
}

BOOST_AUTO_TEST_CASE(erase_test)
{
  maslevtsov::Tree< int, int > tree;

  tree[1] = 1;
  auto it = tree.erase(tree.begin());
  BOOST_TEST((it == tree.end()));
  BOOST_TEST(tree.empty());
  tree[1] = 1;
  tree[2] = 2;
  it = tree.erase(--tree.end());
  BOOST_TEST((it == tree.end()));
  BOOST_TEST(tree.size() == 1);
  tree[2] = 2;
  it = tree.erase(tree.begin());
  BOOST_TEST(it->first == 2);
  BOOST_TEST(tree.size() == 1);

  tree[1] = 1;
  tree[3] = 3;
  it = tree.erase(tree.begin());
  BOOST_TEST(it->first == 2);
  BOOST_TEST(tree.size() == 2);
  tree[1] = 1;
  it = tree.erase(++tree.begin());
  BOOST_TEST(it->first == 3);
  BOOST_TEST(tree.size() == 2);
  tree[2] = 2;
  it = tree.erase(--tree.end());
  BOOST_TEST((it == tree.end()));
  BOOST_TEST(tree.size() == 2);

  tree[3] = 3;
  tree[0] = 0;
  it = tree.erase(++(++tree.begin()));
  BOOST_TEST(it->first == 3);
  BOOST_TEST(tree.size() == 3);

  tree[2] = 2;
  tree[4] = 4;
  it = tree.erase(tree.begin());
  BOOST_TEST(it->first == 1);
  BOOST_TEST(tree.size() == 4);
  tree[0] = 0;
  it = tree.erase(++(++tree.begin()));
  BOOST_TEST(it->first == 3);
  BOOST_TEST(tree.size() == 4);
  tree[2] = 2;
  it = tree.erase(--tree.end());
  BOOST_TEST((it == tree.end()));
  BOOST_TEST(tree.size() == 4);
  tree[4] = 4;
  it = tree.erase(++tree.begin());
  BOOST_TEST(it->first == 2);
  BOOST_TEST(tree.size() == 4);
  tree[1] = 1;
  it = tree.erase(--(--tree.end()));
  BOOST_TEST(it->first == 4);
  BOOST_TEST(tree.size() == 4);

  tree[3] = 3;
  tree[5] = 5;
  it = tree.erase(tree.find(2));
  BOOST_TEST(it->first == 3);
  BOOST_TEST(tree.size() == 5);
  tree[2] = 2;
  it = tree.erase(tree.begin());
  BOOST_TEST(it->first == 1);
  BOOST_TEST(tree.size() == 5);
  tree[0] = 0;
  it = tree.erase(tree.find(3));
  BOOST_TEST(it->first == 4);
  BOOST_TEST(tree.size() == 5);
  tree[3] = 3;
  it = tree.erase(--tree.end());
  BOOST_TEST((it == tree.end()));
  BOOST_TEST(tree.size() == 5);

  tree.clear();
  for (int i = 0; i != 7; ++i) {
    tree[i] = i;
  }
  it = tree.erase(tree.begin());
  BOOST_TEST(it->first == 1);
  BOOST_TEST(tree.size() == 6);
  tree.clear();
  for (int i = 0; i != 7; ++i) {
    tree[i] = i;
  }
  it = tree.erase(tree.find(6));
  BOOST_TEST((it == tree.end()));
  BOOST_TEST(tree.size() == 6);

  tree.clear();
  for (int i = 0; i != 9; ++i) {
    tree[i] = i;
  }
  it = tree.erase(tree.find(2));
  BOOST_TEST(it->first == 3);
  BOOST_TEST(tree.size() == 8);
  tree.clear();
  for (int i = 8; i >= 0; --i) {
    tree[i] = i;
  }
  it = tree.erase(tree.find(6));
  BOOST_TEST(it->first == 7);
  BOOST_TEST(tree.size() == 8);

  tree.clear();
  for (int i = 0; i != 7; ++i) {
    tree[i] = i;
  }
  for (int i = 9; i != 13; ++i) {
    tree[i] = i;
  }
  for (int i = 7; i != 9; ++i) {
    tree[i] = i;
  }
  it = tree.erase(tree.find(2));
  BOOST_TEST(it->first == 3);
  BOOST_TEST(tree.size() == 12);
  tree.clear();
  for (int i = 0; i != 7; ++i) {
    tree[i] = i;
  }
  for (int i = 9; i != 13; ++i) {
    tree[i] = i;
  }
  for (int i = 7; i != 9; ++i) {
    tree[i] = i;
  }
  it = tree.erase(tree.find(12));
  BOOST_TEST((it == tree.end()));
  BOOST_TEST(tree.size() == 12);
  tree.clear();
  for (int i = 0; i != 3; ++i) {
    tree[i] = i;
  }
  for (int i = 5; i != 13; ++i) {
    tree[i] = i;
  }
  tree[3] = 3;
  tree[4] = 4;
  it = tree.erase(tree.find(7));
  BOOST_TEST(it->first == 8);
  BOOST_TEST(tree.size() == 12);
  tree.clear();
  for (int i = 0; i != 13; ++i) {
    tree[i] = i;
  }
  it = tree.erase(tree.find(4));
  BOOST_TEST(it->first == 5);
  BOOST_TEST(tree.size() == 12);

  tree.clear();
  for (int i = 0; i != 11; ++i) {
    tree[i] = i;
  }
  it = tree.erase(tree.find(1));
  BOOST_TEST(it->first == 2);
  BOOST_TEST(tree.size() == 10);
  tree.clear();
  for (int i = 0; i != 11; ++i) {
    tree[i] = i;
  }
  it = tree.erase(tree.find(4));
  BOOST_TEST(it->first == 5);
  BOOST_TEST(tree.size() == 10);
  tree.clear();
  for (int i = 0; i != 11; ++i) {
    tree[i] = i;
  }
  it = tree.erase(tree.find(10));
  BOOST_TEST((it == tree.end()));
  BOOST_TEST(tree.size() == 10);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  maslevtsov::Tree< int, int > tree1;
  maslevtsov::Tree< int, int > tree2;
  tree1[0] = 0;
  tree1[1] = 1;
  tree2[2] = 2;
  tree1.swap(tree2);
  BOOST_TEST(tree1.begin()->first == 2);
  BOOST_TEST(tree1.begin()->second == 2);
  BOOST_TEST(tree1.size() == 1);
  BOOST_TEST(tree2.begin()->first == 0);
  BOOST_TEST(tree2.begin()->second == 0);
  BOOST_TEST((++tree2.begin())->first == 1);
  BOOST_TEST((++tree2.begin())->second == 1);
  BOOST_TEST(tree2.size() == 2);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(operations_tests)
BOOST_AUTO_TEST_CASE(count_test)
{
  maslevtsov::Tree< int, int > tree;
  tree[0] = 0;
  BOOST_TEST(tree.count(0) == 1);
  BOOST_TEST(tree.count(1) == 0);
}

BOOST_AUTO_TEST_CASE(find_test)
{
  maslevtsov::Tree< int, int > tree;
  tree[0] = 0;
  tree[1] = 1;
  BOOST_TEST(tree.find(1)->first == 1);
  BOOST_TEST(tree.find(1)->second == 1);
}

BOOST_AUTO_TEST_CASE(equal_range_test)
{
  maslevtsov::Tree< int, int > tree;
  tree[0] = 0;
  tree[1] = 1;
  tree[2] = 2;
  BOOST_TEST(tree.equal_range(1).first->first == 1);
  BOOST_TEST(tree.equal_range(1).first->second == 1);
  BOOST_TEST(tree.equal_range(1).second->first == 2);
  BOOST_TEST(tree.equal_range(1).second->second == 2);
}
BOOST_AUTO_TEST_SUITE_END()
