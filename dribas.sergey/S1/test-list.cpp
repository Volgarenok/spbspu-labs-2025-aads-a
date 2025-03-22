#include <boost/test/unit_test.hpp>
#include "list.hpp"

using namespace dribas;

BOOST_AUTO_TEST_SUITE(Constructor)
BOOST_AUTO_TEST_CASE(DefaultConstructorTest) {
  List< int > list;

  BOOST_CHECK(list.empty());
}
BOOST_AUTO_TEST_CASE(CopyConstructorTest) {
  List< int > original;
  original.push_back(1);
  List< int > copy(original);

  BOOST_CHECK_EQUAL(copy.size(), original.size());
  BOOST_CHECK_NE(copy.at(0), original.at(0));
  BOOST_CHECK_EQUAL(copy.at(0)->data_, original.at(0)->data_);
}
BOOST_AUTO_TEST_CASE(MoveConstructorTest) {
  List< int > original;
  original.push_back(1);
  List< int > moved(std::move(original));

  BOOST_CHECK(moved.size() == 1);
  BOOST_CHECK(moved.at(0)->data_ == 1);
  BOOST_CHECK(original.size() == 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(assignmentOperator)
BOOST_AUTO_TEST_CASE(CopyOperator) {
  List< int > original;
  List< int > copy;
  original.push_back(1);
  copy = original;

  BOOST_CHECK_EQUAL(copy.size(), original.size());
  BOOST_CHECK_NE(copy.at(0), original.at(0));
  BOOST_CHECK_EQUAL(copy.at(0)->data_, original.at(0)->data_);
}
BOOST_AUTO_TEST_CASE(CopyOperatorThis) {
  List< int > original;
  original.push_back(1);
  original = original;

  BOOST_CHECK(original.size() == 1);
  BOOST_CHECK(original.at(0)->data_ == 1);
}
BOOST_AUTO_TEST_CASE(MoveOperator) {

  List< int > original;
  original.push_back(1);
  List< int > moved = std::move(original);

  BOOST_CHECK(moved.size() == 1);
  BOOST_CHECK(moved.at(0)->data_ == 1);
  BOOST_CHECK(original.size() == 0);
}
BOOST_AUTO_TEST_CASE(MoveOperatorThis) {
  List< int > original;
  original.push_back(1);
  original = std::move(original);

  BOOST_CHECK(original.size() == 1);
  BOOST_CHECK(original.at(0)->data_ == 1);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(noAssignmentOperator)
BOOST_AUTO_TEST_CASE(operatorScob) {
  List< int > list;
  list.push_back(1);
  BOOST_CHECK(list[0]->data_ == 1);
}
BOOST_AUTO_TEST_CASE(operatorAt) {
  List< int > list;
  list.push_back(1);
  BOOST_CHECK(list.at(0)->data_ == 1);
  BOOST_CHECK_THROW(list.at(1)->data_, std::out_of_range);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MethodList)
BOOST_AUTO_TEST_CASE(empty) {
  List< int > list;
  BOOST_CHECK(list.empty());
  list.push_back(1);
  BOOST_CHECK(!list.empty());
}
BOOST_AUTO_TEST_CASE(size) {
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
BOOST_AUTO_TEST_CASE(PushFrontToEmptyList) {
  dribas::List< int > list;
  list.push_front(78);

  BOOST_CHECK(!list.empty());
  BOOST_CHECK(list.size() == 1);
  BOOST_CHECK(list.front() == list.back());
  BOOST_CHECK(list.front() == 78);
}
BOOST_AUTO_TEST_CASE(PushFrontRvalueToEmptyList) {
  dribas::List< std::string > list;
  list.push_front("temp_value");

  BOOST_CHECK(!list.empty());
  BOOST_CHECK(list.size() == 1);
  BOOST_CHECK(list.at(0)->data_ == "temp_value");
  BOOST_CHECK(list.at(0)->data_ == list.front());
  BOOST_CHECK(list.at(0)->data_ == list.back());
}
BOOST_AUTO_TEST_CASE(PushFrontRvalue) {
  dribas::List< std::string > list;
  list.push_front("second");
  list.push_front("first");

  BOOST_CHECK(list.size() == 2);
  BOOST_CHECK(list.front() == "first");
  BOOST_CHECK(list.back() == "second");
}
BOOST_AUTO_TEST_CASE(PushFrontRvalues) {
  dribas::List< std::string > list;
  list.push_front("3");
  list.push_front("2");
  list.push_front("1");

  BOOST_CHECK(list.size() == 3);
  BOOST_CHECK(list.at(0)->data_ == "1");
  BOOST_CHECK(list.at(1)->data_ == "2");
  BOOST_CHECK(list.at(2)->data_ == "3");
}
BOOST_AUTO_TEST_CASE(PushFrontMove) {
  dribas::List< std::string > list;
  std::string value = "value";
  list.push_front(std::move(value));

  BOOST_CHECK(value.empty());
  BOOST_CHECK(list.at(0)->data_ == "value");
  BOOST_CHECK(list.size() == 1);
}
BOOST_AUTO_TEST_CASE(FrontBackSingleElement) {
  List< int > list;
  list.push_front(42);
  BOOST_CHECK(list.front() == 42);
  BOOST_CHECK(list.back() == 42);
  const List< int >& constList = list;
  BOOST_CHECK(constList.front() == 42);
  BOOST_CHECK(constList.back() == 42);
}
BOOST_AUTO_TEST_CASE(FrontBackModifyElements) {
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
BOOST_AUTO_TEST_CASE(PopFrontEmptyList) {
  dribas::List< int > list;
  list.pop_front();
  BOOST_CHECK(list.empty());
}
BOOST_AUTO_TEST_CASE(PopFrontSingleElement)
{
  dribas::List<int> list;
  list.push_front(1);
  list.pop_front();
  BOOST_CHECK(list.empty());
}
BOOST_AUTO_TEST_CASE(PopFrontMultipleElements) {
  dribas::List<int> list;
  list.push_front(3);
  list.push_front(2);
  list.push_front(1);
  list.pop_front();
  BOOST_CHECK(list.size() == 2);
  BOOST_CHECK(list.front() == 2);
  BOOST_CHECK(list.back() == 3);
}
BOOST_AUTO_TEST_CASE(PopFrontAllElements) {
  dribas::List<int> list;
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
BOOST_AUTO_TEST_CASE(PopBackEmptyList) {
  dribas::List< int > list;
  list.pop_back();
  BOOST_CHECK(list.empty());
  BOOST_CHECK(list.size() == 0);
}
BOOST_AUTO_TEST_CASE(PopBackSingleElement) {
  dribas::List<int> list;
  list.push_back(32);
  list.pop_back();
  BOOST_CHECK(list.empty());
  BOOST_CHECK(list.size() == 0);
}
BOOST_AUTO_TEST_CASE(ClearAlreadyEmptyList) {
  dribas::List< int > list;
  list.clear();
  BOOST_CHECK(list.empty());
  list.clear();
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_SUITE_END()
