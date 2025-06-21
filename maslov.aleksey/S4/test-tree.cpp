#include <boost/test/unit_test.hpp>
#include <string>
#include <sstream>
#include <tree/tree.hpp>

namespace
{
 template< typename Key, typename Value, typename Cmp >
  void printTree(std::ostream & out, const maslov::BiTree< Key, Value, Cmp > & biTree)
  {
    out << biTree.cbegin()->first << ' ' << biTree.cbegin()->second;
    for (auto it = ++biTree.cbegin(); it != biTree.cend(); ++it)
    {
      out << ' ' << it->first << ' ' << it->second;
    }
  }
}

BOOST_AUTO_TEST_SUITE(constructors)

BOOST_AUTO_TEST_CASE(defaultConstructor)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  BOOST_TEST(tree.empty());
  bool check = tree.cbegin() == tree.cend();
  BOOST_TEST(check);
}

BOOST_AUTO_TEST_CASE(copyConstructor)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  tree.push(1, "first");
  tree.push(2, "second");
  tree.push(3, "third");
  maslov::BiTree< int, std::string, std::less< int > > copy(tree);
  std::ostringstream out;
  printTree(out, copy);
  BOOST_TEST(out.str() == "1 first 2 second 3 third");
  tree.pop(2);
  BOOST_TEST(copy.size() == 3);
  BOOST_TEST(tree.size() == 2);
}

BOOST_AUTO_TEST_CASE(moveConstructor)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  tree.push(1, "first");
  tree.push(2, "second");
  tree.push(3, "third");
  maslov::BiTree< int, std::string, std::less< int > > moved(std::move(tree));
  std::ostringstream out;
  printTree(out, moved);
  BOOST_TEST(out.str() == "1 first 2 second 3 third");
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(rangeConstructor)
{
  maslov::BiTree< int, std::string, std::less< int > > tree1;
  tree1.push(1, "first");
  tree1.push(2, "second");
  tree1.push(3, "third");
  auto first = tree1.find(1);
  auto last = tree1.end();
  maslov::BiTree< int, std::string, std::less< int > > tree2(first, last);
  std::ostringstream out1;
  printTree(out1, tree1);
  std::ostringstream out2;
  printTree(out2, tree2);
  BOOST_TEST(out1.str() == out2.str());
}

BOOST_AUTO_TEST_CASE(initializerListconstructor)
{
  std::initializer_list< std::pair< int, std::string > > il{{1, "first"}, {2, "second"}, {3, "third"}};
  maslov::BiTree< int, std::string, std::less< int > > tree1(il);
  maslov::BiTree< int, std::string, std::less< int > > tree2;
  tree2.push(1, "first");
  tree2.push(2, "second");
  tree2.push(3, "third");
  std::ostringstream out1;
  printTree(out1, tree1);
  std::ostringstream out2;
  printTree(out2, tree2);
  BOOST_TEST(out1.str() == out2.str());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(operators)

BOOST_AUTO_TEST_CASE(copyOperator)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  tree.push(1, "first");
  tree.push(2, "second");
  tree.push(3, "third");
  maslov::BiTree< int, std::string, std::less< int > > copy;
  copy = tree;
  std::ostringstream out1;
  printTree(out1, tree);
  std::ostringstream out2;
  printTree(out2, copy);
  BOOST_TEST(out1.str() == out2.str());
}

BOOST_AUTO_TEST_CASE(moveOperator)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  tree.push(1, "first");
  tree.push(2, "second");
  tree.push(3, "third");
  maslov::BiTree< int, std::string, std::less< int > > moved;
  moved = std::move(tree);
  std::ostringstream out;
  printTree(out, moved);
  BOOST_TEST(out.str() == "1 first 2 second 3 third");
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(elementAccess)

BOOST_AUTO_TEST_CASE(operatorSquareBrackets)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  tree[2] = "second";
  std::ostringstream out;
  printTree(out, tree);
  BOOST_TEST(out.str() == "2 second");
}

BOOST_AUTO_TEST_CASE(at)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  tree.push(1, "first");
  tree.push(2, "second");
  tree.push(3, "third");
  BOOST_TEST(tree.at(1) == "first");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(modifiers)

BOOST_AUTO_TEST_CASE(pushSingleElement)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  tree.push(1, "first");
  std::ostringstream out;
  printTree(out, tree);
  BOOST_TEST(out.str() == "1 first");
}

BOOST_AUTO_TEST_CASE(pushMoreElements)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  tree.push(2, "second");
  tree.push(1, "first");
  tree.push(3, "third");
  tree.push(4, "fourth");
  tree.push(5, "fifth");
  tree.push(6, "sixth");
  std::ostringstream out;
  printTree(out, tree);
  BOOST_TEST(out.str() == "1 first 2 second 3 third 4 fourth 5 fifth 6 sixth");
}

BOOST_AUTO_TEST_CASE(get)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  tree.push(1, "first");
  tree.push(2, "second");
  tree.push(3, "third");
  BOOST_TEST(tree.get(3) == "third");
}

BOOST_AUTO_TEST_CASE(pop)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  tree.push(1, "first");
  tree.push(2, "second");
  tree.push(3, "third");
  BOOST_TEST(tree.pop(2) == "second");
  std::ostringstream out;
  printTree(out, tree);
  BOOST_TEST(out.str() == "1 first 3 third");
}

BOOST_AUTO_TEST_CASE(insert)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  auto pair1 = tree.insert({1, "first"});
  BOOST_TEST(pair1.second == true);
  std::ostringstream out1;
  printTree(out1, tree);
  BOOST_TEST(out1.str() == "1 first");

  auto pair2 = tree.insert({1, "second"});
  BOOST_TEST(pair2.second == false);
  std::ostringstream out2;
  printTree(out2, tree);
  BOOST_TEST(out2.str() == out1.str());
}

BOOST_AUTO_TEST_CASE(insertRange)
{
  maslov::BiTree< int, std::string, std::less< int > > tree1;
  maslov::BiTree< int, std::string, std::less< int > > tree2;
  tree1.push(1, "first");
  tree1.push(2, "second");
  tree1.push(3, "third");
  tree1.push(4, "fourth");
  tree1.push(5, "fifth");
  auto first = tree1.find(2);
  auto last = tree1.find(4);
  tree2.insert(first, last);
  std::ostringstream out;
  printTree(out, tree2);
  BOOST_TEST(out.str() == "2 second 3 third");
}

BOOST_AUTO_TEST_CASE(eraseKey)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  tree.push(1, "first");
  tree.push(2, "second");
  tree.push(3, "third");
  size_t erased = tree.erase(2);
  BOOST_TEST(erased == 1);
  std::ostringstream out;
  printTree(out, tree);
  BOOST_TEST(out.str() == "1 first 3 third");
  erased = tree.erase(999);
  BOOST_TEST(erased == 0);
}

BOOST_AUTO_TEST_CASE(eraseIterator)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  tree.push(1, "first");
  tree.push(2, "second");
  tree.push(3, "third");
  auto it = tree.find(2);
  auto next = tree.erase(it);
  BOOST_TEST(next->first == 3);
  std::ostringstream out;
  printTree(out, tree);
  BOOST_TEST(out.str() == "1 first 3 third");
}

BOOST_AUTO_TEST_CASE(eraseRange)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  tree.push(1, "first");
  tree.push(2, "second");
  tree.push(3, "third");
  tree.push(4, "fourth");
  tree.push(5, "fifth");
  auto first = tree.find(2);
  auto last = tree.find(4);
  auto it = tree.erase(first, last);
  BOOST_TEST(it->first == 4);
  std::ostringstream out;
  printTree(out, tree);
  BOOST_TEST(out.str() == "1 first 4 fourth 5 fifth");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(lookup)

BOOST_AUTO_TEST_CASE(find)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  tree.push(1, "first");
  tree.push(2, "second");
  tree.push(3, "third");
  auto it1 = tree.find(1);
  BOOST_TEST(it1->second == "first");
  auto it2 = tree.find(4);
  bool check = (it2 == tree.end());
  BOOST_TEST(check);
}

BOOST_AUTO_TEST_CASE(lowerBound)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  tree.push(1, "first");
  tree.push(2, "second");
  tree.push(5, "fifth");
  auto lb1 = tree.lowerBound(2);
  BOOST_TEST(lb1->first == 2);

  auto lb2 = tree.lowerBound(3);
  BOOST_TEST(lb2->first == 5);

  auto lb3 = tree.lowerBound(6);
  bool check = (lb3 == tree.end());
  BOOST_TEST(check);
}

BOOST_AUTO_TEST_CASE(upperBound)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  tree.push(1, "first");
  tree.push(2, "second");
  tree.push(5, "fifth");
  auto ub1 = tree.upperBound(2);
  BOOST_TEST(ub1->first == 5);

  auto ub2 = tree.upperBound(3);
  BOOST_TEST(ub2->first == 5);

  auto ub3 = tree.upperBound(6);
  bool check = (ub3 == tree.end());
  BOOST_TEST(check);
}

BOOST_AUTO_TEST_CASE(equalRange)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  tree.push(1, "first");
  tree.push(2, "second");
  tree.push(5, "fifth");
  auto range = tree.equalRange(2);
  BOOST_TEST(range.first->first == 2);
  BOOST_TEST(range.second->first == 5);
}

BOOST_AUTO_TEST_CASE(count)
{
  maslov::BiTree< int, std::string, std::less< int > > tree;
  tree.push(1, "first");
  tree.push(2, "second");
  tree.push(5, "fifth");
  BOOST_TEST(tree.count(1) == 1);
  BOOST_TEST(tree.count(3) == 0);
}

BOOST_AUTO_TEST_SUITE_END()
