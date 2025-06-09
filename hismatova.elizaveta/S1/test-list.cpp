#include <boost/test/unit_test.hpp>
#include "main.cpp"
using namespace hismatova;

BOOST_AUTO_TEST_CASE(constructorAndPushBack)
{
  List< int > lst;
  BOOST_TEST(lst.empty());
  BOOST_TEST(lst.size() == 0);

  lst.push_back(10);
  lst.push_back(20);
  lst.push_back(30);

  BOOST_TEST(!lst.empty());
  BOOST_TEST(lst.size() == 3);
  BOOST_TEST(lst.front() == 10);
}

BOOST_AUTO_TEST_CASE(popFront)
{
  List< int > lst;
  lst.push_back(10);
  lst.push_back(20);

  lst.pop_front();

  BOOST_TEST(lst.size() == 1);
  BOOST_TEST(lst.front() == 20);
}

BOOST_AUTO_TEST_CASE(initializerListConstructor)
{
  List< int > lst = { 1, 2, 3, 4 };
  BOOST_TEST(lst.size() == 4);
  BOOST_TEST(lst.front() == 1);
}

BOOST_AUTO_TEST_CASE(copyConstructor)
{
  List< int > lst1 = { 1, 2, 3 };
  List< int > lst2 = lst1;

  BOOST_TEST(lst1.size() == lst2.size());
  BOOST_TEST(lst1 == lst2);
}

BOOST_AUTO_TEST_CASE(assignmentOperator)
{
  List< int > lst1 = { 1, 2, 3 };
  List< int > lst2;
  lst2 = lst1;

  BOOST_TEST(lst1.size() == lst2.size());
  BOOST_TEST(lst1 == lst2);
}

BOOST_AUTO_TEST_CASE(moveConstructor)
{
  List< int > lst1 = { 1, 2, 3 };
  List< int > lst2 = std::move(lst1);

  BOOST_TEST(lst2.size() == 3);
  BOOST_TEST(lst1.size() == 0);
}

BOOST_AUTO_TEST_CASE(remove)
{
  List< int > lst = { 1, 2, 3, 4, 5 };
  lst.remove(3);

  BOOST_TEST(lst.size() == 4);
  BOOST_TEST(!(lst == List< int >({ 1, 2, 3, 4, 5 })));
}

BOOST_AUTO_TEST_CASE(reverse)
{
  List< int > lst = { 1, 2, 3, 4, 5 };
  lst.reverse();

  BOOST_TEST(lst.front() == 5);
  BOOST_TEST(lst.size() == 5);
}

BOOST_AUTO_TEST_CASE(iterators)
{
  List< int > lst = { 1, 2, 3 };

  auto it = lst.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 3);
}

BOOST_AUTO_TEST_CASE(swap)
{
  List< int > lst1 = { 1, 2 };
  List< int > lst2 = { 3, 4, 5 };

  lst1.swap(lst2);

  BOOST_TEST(lst1.size() == 3);
  BOOST_TEST(lst2.size() == 2);
  BOOST_TEST(lst1.front() == 3);
  BOOST_TEST(lst2.front() == 1);
}

BOOST_AUTO_TEST_CASE(insert)
{
  List< int > lst = { 1, 2, 4, 5 };

  auto it = lst.begin();
  ++it;

  lst.insert(it, 3);

  BOOST_TEST(lst.size() == 5);

  it = lst.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 3);
  ++it;
  BOOST_TEST(*it == 4);
  ++it;
  BOOST_TEST(*it == 5);
}

BOOST_AUTO_TEST_CASE(equalityOperator)
{
  List< int > lst1 = { 1, 2, 3 };
  List< int > lst2 = { 1, 2, 3 };
  List< int > lst3 = { 1, 2 };

  BOOST_TEST(lst1 == lst2);
  BOOST_TEST(!(lst1 == lst3));
}

BOOST_AUTO_TEST_CASE(erase)
{
  List< int > lst = { 1, 2, 3, 4 };
  auto it = lst.begin();
  ++it;
  lst.erase(it);

  BOOST_TEST(lst.size() == 3);
  BOOST_TEST(lst.front() == 1);
  it = lst.begin();
  ++it;
  BOOST_TEST(*it == 3);
}

BOOST_AUTO_TEST_CASE(empty)
{
  List< int > lst;
  BOOST_TEST(lst.empty());

  lst.push_back(10);
  BOOST_TEST(!lst.empty());
}

BOOST_AUTO_TEST_CASE(verticalSums)
{
  std::vector<std::pair<std::string, List< int >>> sequences;
  sequences.emplace_back("Seq1", List< int >{1, 2, 3});
  sequences.emplace_back("Seq2", List< int >{4, 5});
  sequences.emplace_back("Seq3", List< int >{6, 7, 8, 9});

  std::vector< int > sums;
  std::vector<std::vector< int >> vertical_sequences = { {1, 4, 6}, {2, 5, 7}, {3, 8}, {9} };

  for (const auto& v_seq : vertical_sequences) {
    if (!v_seq.empty()) {
      int sum = std::accumulate(v_seq.begin(), v_seq.end(), 0);
      sums.push_back(sum);
    }
  }

  BOOST_TEST(sums.size() == vertical_sequences.size());
  BOOST_TEST(sums[0] == 1 + 4 + 6);
  BOOST_TEST(sums[1] == 2 + 5 + 7);
  BOOST_TEST(sums[2] == 3 + 8);
  BOOST_TEST(sums[3] == 9);
}

BOOST_AUTO_TEST_CASE(emptyListOperations)
{
  List< int > lst;

  lst.push_back(10);
  BOOST_TEST(lst.front() == 10);

  lst.pop_front();
  BOOST_TEST(lst.empty());

  lst.remove(10);
  BOOST_TEST(lst.empty());
}

BOOST_AUTO_TEST_CASE(assign)
{
  List< int > lst = { 1, 2, 3 };
  List< int > lst2;
  lst2.assign({ 4, 5, 6 });

  BOOST_TEST(lst2.size() == 3);
  BOOST_TEST(lst2.front() == 4);

  lst2.assign(lst.begin(), lst.end());

  BOOST_TEST(lst2.size() == 3);
  BOOST_TEST(lst2.front() == 1);
}

BOOST_AUTO_TEST_CASE(reverseEmptyList)
{
  List< int > lst;
  lst.reverse();

  BOOST_TEST(lst.empty());
}

BOOST_AUTO_TEST_CASE(circularList)
{
  List< int > lst = { 1, 2, 3 };

  auto it = lst.begin();
  ++it;
  lst.erase(it);

  BOOST_TEST(lst.size() == 2);
  BOOST_TEST(lst.front() == 1);
  it = lst.begin();
  ++it;
  BOOST_TEST(*it == 3);
}

BOOST_AUTO_TEST_SUITE_END()
