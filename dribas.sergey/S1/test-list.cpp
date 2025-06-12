#include <boost/test/unit_test.hpp>
#include <algorithm>
#include <functional>

#include "list.hpp"

using namespace dribas;

BOOST_AUTO_TEST_SUITE(Constructor)
BOOST_AUTO_TEST_CASE(DefaultConstructorTest)
{
  List< int > list;

  BOOST_CHECK(list.empty());
}
BOOST_AUTO_TEST_CASE(CopyConstructorTest)
{
  List< int > original;
  original.push_back(1);
  List< int > copy(original);

  BOOST_CHECK_EQUAL(copy.size(), original.size());
  BOOST_CHECK(copy.begin() != original.begin());
  BOOST_CHECK_EQUAL(copy.front(), original.front());
}
BOOST_AUTO_TEST_CASE(MoveConstructorTest)
{
  List< int > original;
  original.push_back(1);
  List< int > moved(std::move(original));

  BOOST_CHECK(moved.size() == 1);
  BOOST_CHECK(moved.front() == 1);
  BOOST_CHECK(original.size() == 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(assignmentOperator)
BOOST_AUTO_TEST_CASE(CopyOperator)
{
  List< int > original;
  List< int > copy;
  original.push_back(1);
  copy = original;

  BOOST_CHECK_EQUAL(copy.size(), original.size());
  BOOST_CHECK(copy.begin() != original.begin());
  BOOST_CHECK(copy.front() == original.front());
}
BOOST_AUTO_TEST_CASE(CopyOperatorThis)
{
  List< int > original;
  original.push_back(1);
  original = original;

  BOOST_CHECK(original.size() == 1);
  BOOST_CHECK(original.front() == 1);
}
BOOST_AUTO_TEST_CASE(MoveOperator)
{

  List< int > original;
  original.push_back(1);
  List< int > moved = std::move(original);

  BOOST_CHECK(moved.size() == 1);
  BOOST_CHECK(moved.front() == 1);
  BOOST_CHECK(original.size() == 0);
}
BOOST_AUTO_TEST_CASE(MoveOperatorThis)
{
  List< int > original;
  original.push_back(1);

  BOOST_CHECK(original.size() == 1);
  BOOST_CHECK(original.front()== 1);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(noAssignmentOperator)
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MethodList)

BOOST_AUTO_TEST_CASE(removeIf)
{
  List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.remove_if(std::bind(std::equal_to< int >(), std::placeholders::_1, 1)  );
  BOOST_CHECK(list.front() == 2);
  list.clear();
  list.remove_if(std::bind(std::equal_to< int >(), std::placeholders::_1, 1));
  BOOST_CHECK(list.empty());
}
BOOST_AUTO_TEST_CASE(remove )
{
  List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.remove(1);
  BOOST_CHECK(list.front() == 2);
  list.clear();
  list.remove(1);
  BOOST_CHECK(list.empty());
}
BOOST_AUTO_TEST_CASE(empty)
{
  List< int > list;
  BOOST_CHECK(list.empty());
  list.push_back(1);
  BOOST_CHECK(!list.empty());
}
BOOST_AUTO_TEST_CASE(size)
{
  List< int > list;
  BOOST_CHECK(list.size() == 0);
  list.push_back(1);
  BOOST_CHECK(list.size() == 1);
}
BOOST_AUTO_TEST_CASE(PushFrontToNonEmptyList)
{
  dribas::List< int > list;
  list.push_front(2);
  list.push_front(1);

  BOOST_CHECK(list.size() == 2);
  BOOST_CHECK(list.front() == 1);
  BOOST_CHECK(list.back() == 2);
}
BOOST_AUTO_TEST_CASE(PushFrontToEmptyList)
{
  dribas::List< int > list;
  list.push_front(78);

  BOOST_CHECK(!list.empty());
  BOOST_CHECK(list.size() == 1);
  BOOST_CHECK(list.front() == list.back());
  BOOST_CHECK(list.front() == 78);
}
BOOST_AUTO_TEST_CASE(PushFrontRvalue)
{
  dribas::List< std::string > list;
  list.push_front("second");
  list.push_front("first");

  BOOST_CHECK(list.size() == 2);
  BOOST_CHECK(list.front() == "first");
  BOOST_CHECK(list.back() == "second");
}
BOOST_AUTO_TEST_CASE(PushFrontRvalues)
{
  dribas::List< std::string > list;
  list.push_front("3");
  list.push_front("2");
  list.push_front("1");

  BOOST_CHECK(list.size() == 3);
  BOOST_CHECK(list.front() == "1");
  BOOST_CHECK(*(++list.begin()) == "2");
}
BOOST_AUTO_TEST_CASE(PushFrontMove)
{
  dribas::List< std::string > list;
  std::string value = "value";
  list.push_front(std::move(value));

  BOOST_CHECK(list.front() == "value");
  BOOST_CHECK(list.size() == 1);
}
BOOST_AUTO_TEST_CASE(FrontBackSingleElement)
{
  List< int > list;
  list.push_front(42);
  BOOST_CHECK(list.front() == 42);
  BOOST_CHECK(list.back() == 42);
  const List< int >& constList = list;
  BOOST_CHECK(constList.front() == 42);
  BOOST_CHECK(constList.back() == 42);
}
BOOST_AUTO_TEST_CASE(FrontBackModifyElements)
{
  dribas::List< int > list;
  list.push_front(2);
  list.push_front(1);
  list.front() = 10;
  BOOST_CHECK(list.front() == 10);
  list.back() = 20;
  BOOST_CHECK(list.back() == 20);
  BOOST_CHECK(list.front() == 10);
  BOOST_CHECK(list.back() == 20);
}
BOOST_AUTO_TEST_CASE(PopFrontEmptyList)
{
  dribas::List< int > list;
  list.pop_front();
  BOOST_CHECK(list.empty());
}
BOOST_AUTO_TEST_CASE(PopFrontSingleElement)
{
  dribas::List< int > list;
  list.push_front(1);
  list.pop_front();
  BOOST_CHECK(list.empty());
}
BOOST_AUTO_TEST_CASE(PopFrontMultipleElements)
{
  dribas::List< int > list;
  list.push_front(3);
  list.push_front(2);
  list.push_front(1);
  list.pop_front();
  BOOST_CHECK(list.size() == 2);
  BOOST_CHECK(list.front() == 2);
  BOOST_CHECK(list.back() == 3);
}
BOOST_AUTO_TEST_CASE(PopFrontAllElements)
{
  dribas::List< int > list;
  list.push_front(2);
  list.push_front(1);
  list.pop_front();

  BOOST_CHECK(list.size() == 1);
  BOOST_CHECK(list.front() == 2);
  BOOST_CHECK(list.back() == 2);
  list.pop_front();
  BOOST_CHECK(list.empty());
  BOOST_CHECK(list.size() == 0);
}
BOOST_AUTO_TEST_CASE(PopBackEmptyList)
{
  dribas::List< int > list;
  list.pop_back();
  BOOST_CHECK(list.empty());
  BOOST_CHECK(list.size() == 0);
}
BOOST_AUTO_TEST_CASE(PopBackSingleElement)
{
  dribas::List< int > list;
  list.push_back(32);
  list.pop_back();
  BOOST_CHECK(list.empty());
  BOOST_CHECK(list.size() == 0);
}
BOOST_AUTO_TEST_CASE(ClearAlreadyEmptyList)
{
  dribas::List< int > list;
  list.clear();
  BOOST_CHECK(list.empty());
  list.clear();
  BOOST_CHECK(list.empty());
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Itarator)
BOOST_AUTO_TEST_CASE(iteratorBeginEnd)
{
  List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  auto it = list.begin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 3);
}
BOOST_AUTO_TEST_CASE(iteratorEmptyList)
{
  List< int > emptyList;
  BOOST_CHECK(emptyList.begin() == emptyList.end());
  const List< int > constEmptyList;
  BOOST_CHECK(constEmptyList.begin() == constEmptyList.end());
  BOOST_CHECK(constEmptyList.cbegin() == constEmptyList.cend());
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(AssignRangeTests)
BOOST_AUTO_TEST_CASE(assignRangeFromVector)
{
  dribas::List< int > list;
  list.push_back(100);
  std::vector< int > vec = {1, 2, 3, 4, 5};
  list.assign_range(vec);
  BOOST_CHECK(list.size() == 5);
  BOOST_CHECK(list.front() == 1);
  BOOST_CHECK(list.back() == 5);
}
BOOST_AUTO_TEST_CASE(assignRangeFromArray)
{
  dribas::List< int > list;
  double arr[] = {1, 2, 3};
  list.assign_range(arr);
  BOOST_CHECK(list.size() == 3);
  BOOST_CHECK(list.front() == 1);
  BOOST_CHECK(list.back() == 3);
}
BOOST_AUTO_TEST_CASE(assignRangeFromEmpty)
{
  dribas::List< int > list;
  list.push_back(1);
  list.push_back(2);
  std::vector< int > empty_vec;
  list.assign_range(empty_vec);
  BOOST_CHECK(list.empty());
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(ConstructWithZeroSize)
{
  const size_t n = 0;
  const int val = 42;
  dribas::List< int > lst(n, val);
  BOOST_CHECK(lst.empty());
  BOOST_CHECK_EQUAL(lst.size(), n);
}

BOOST_AUTO_TEST_CASE(ConstructWithPositiveSize)
{
  const size_t n = 5;
  const std::string val = "test";
  dribas::List< std::string > lst(n, val);
  BOOST_CHECK(!lst.empty());
  BOOST_CHECK_EQUAL(lst.size(), n);
  for (const auto& item : lst) {
    BOOST_CHECK_EQUAL(item, val);
  }
}


BOOST_AUTO_TEST_CASE(ConstructFromVector)
{
  dribas::List< std::string > v;
  v.push_back("1");
  v.push_back("2");

  dribas::List< std::string > lst(v.begin(), v.end());

  BOOST_CHECK_EQUAL(lst.front(), "1");
  BOOST_CHECK_EQUAL(lst.back(), "2");
}

BOOST_AUTO_TEST_CASE(InitializerListConstructor)
{
  dribas::List< int > lst = { 10, 20, 30 };
  BOOST_CHECK_EQUAL(lst.size(), 3);
  BOOST_CHECK_EQUAL(lst.front(), 10);
  BOOST_CHECK_EQUAL(lst.back(), 30);
}

BOOST_AUTO_TEST_CASE(Insert)
{
  dribas::List< int > mylist;
  for (int i = 1; i <= 5; ++i) {
    mylist.push_back(i);
  }

  auto it = mylist.begin();
  it++;

  mylist.insert(it, 10);
  BOOST_CHECK(*(--it) == 10);

}

BOOST_AUTO_TEST_CASE(Insert2)
{

  dribas::List< int > lst;
  lst.insert(lst.begin(),{ 10, 20, 30 });
  BOOST_CHECK_EQUAL(lst.size(), 3);
  BOOST_CHECK_EQUAL(lst.front(), 10);
  BOOST_CHECK_EQUAL(lst.back(), 30);
}

