#include <boost/test/unit_test.hpp>
#include "robinHashTable.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <utility>
#include <boost/test/tools/output_test_stream.hpp>

using namespace dribas;

namespace dribas
{
  template < class Key, class T, class Hash >
  std::ostream& operator<<(std::ostream& os, const dribas::ConstIterator< Key, T, Hash >& it)
  {
    if (it == dribas::ConstIterator< Key, T, Hash >()) {
      return os << "end";
    }
    return os << it->first << it->second;
  }

  template < class Key, class T, class Hash >
  std::ostream& operator<<(std::ostream& os, const dribas::Iterator< Key, T, Hash >& it)
  {
    return os << static_cast< const dribas::ConstIterator< Key, T, Hash >& >(it);
  }
}

namespace boost::test_tools::tt_detail
{
template < class Key, class T, class Hash >
  struct print_log_value<dribas::ConstIterator< Key, T, Hash > >{
    void operator()(std::ostream& os, const dribas::ConstIterator< Key, T, Hash >& it)
    {
      if (it == dribas::ConstIterator< Key, T, Hash >()) {
        os << "end";
      } else {
        os << it->first << it->second;
      }
    }
  };

  template < class Key, class T, class Hash >
  struct print_log_value<dribas::Iterator< Key, T, Hash >>{
    void operator()(std::ostream& os, const dribas::Iterator< Key, T, Hash >& it)
    {
      os << static_cast<const dribas::ConstIterator< Key, T, Hash >&>(it);
    }
  };
}

BOOST_AUTO_TEST_SUITE(ConstructionAndAssignment)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  RobinHoodHashTable< int, std::string > table;
  BOOST_TEST(table.empty());
  BOOST_TEST(table.size() == 0);
}

BOOST_AUTO_TEST_CASE(InitializerListConstructor)
{
  RobinHoodHashTable< int, std::string > table = {{ 1, "one" }, { 2, "two" }};
  BOOST_TEST(table.size() == 2);
  BOOST_TEST(table.at(1) == "one");
  BOOST_TEST(table.at(2) == "two");
}

BOOST_AUTO_TEST_CASE(IteratorRangeConstructor)
{
  std::vector< std::pair< int, std::string >> vec = {{ 1, "one" }, { 2, "two" }};
  RobinHoodHashTable< int, std::string > table(vec.begin(), vec.end());
  BOOST_TEST(table.size() == 2);
  BOOST_TEST(table.find(1) != table.end());
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  RobinHoodHashTable< int, std::string > original = {{ 1, "one" }};
  auto copy(original);
  BOOST_TEST(copy.size() == 1);
  BOOST_TEST(copy.at(1) == "one");
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  RobinHoodHashTable< int, std::string > original = {{ 1, "one" }};
  auto moved(std::move(original));
  BOOST_TEST(moved.size() == 1);
  BOOST_TEST(original.empty());
}

BOOST_AUTO_TEST_CASE(CopyAssignment)
{
  RobinHoodHashTable< int, std::string > original = {{ 1, "one" }};
  RobinHoodHashTable< int, std::string > copy;
  copy = original;
  BOOST_TEST(copy.size() == 1);
  BOOST_TEST(copy.at(1) == "one");
}

BOOST_AUTO_TEST_CASE(MoveAssignment)
{
  RobinHoodHashTable< int, std::string > original = {{ 1, "one" }};
  RobinHoodHashTable< int, std::string > moved;
  moved = std::move(original);
  BOOST_TEST(moved.size() == 1);
  BOOST_TEST(original.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Iterators)

BOOST_AUTO_TEST_CASE(BeginEnd)
{
  RobinHoodHashTable< int, std::string > table;
  BOOST_TEST(table.begin() == table.end());
  table.insert(1, "one");
  BOOST_TEST(table.begin() != table.end());
  BOOST_TEST(table.begin()->first == 1);
}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Modifiers)

BOOST_AUTO_TEST_CASE(InsertKeyValue)
{
  RobinHoodHashTable< int, std::string > table;
  table.insert(1, "one");
  BOOST_TEST(table.size() == 1);
  BOOST_TEST(table.at(1) == "one");
}

BOOST_AUTO_TEST_CASE(InsertRange)
{
  std::vector<std::pair< int, std::string >> vec = {{ 1, "one" }, { 2, "two" }};
  RobinHoodHashTable< int, std::string > table;
  table.insert(vec.begin(), vec.end());
  BOOST_TEST(table.size() == 2);
}

BOOST_AUTO_TEST_CASE(EraseByKey)
{
  RobinHoodHashTable< int, std::string > table = {{ 1, "one" }};
  BOOST_TEST(table.erase(1));
  BOOST_TEST(table.empty());
  BOOST_TEST(!table.erase(999));
}

BOOST_AUTO_TEST_CASE(EraseByIterator)
{
  RobinHoodHashTable< int, std::string > table = {{ 1, "one" }};
  auto it = table.find(1);
  it = table.erase(it);
  BOOST_TEST(table.empty());
  BOOST_TEST(it == table.end());
}

BOOST_AUTO_TEST_CASE(Clear)
{
  RobinHoodHashTable< int, std::string > table = {{ 1, "one" }};
  table.clear();
  BOOST_TEST(table.empty());
}

BOOST_AUTO_TEST_CASE(Swap)
{
  RobinHoodHashTable< int, std::string > table1 = {{ 1, "one" }};
  RobinHoodHashTable< int, std::string > table2;
  table1.swap(table2);
  BOOST_TEST(table1.empty());
  BOOST_TEST(table2.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(ElementAccess)

BOOST_AUTO_TEST_CASE(SubscriptOperator)
{
  RobinHoodHashTable< int, std::string > table;
  table[1] = "one";
  BOOST_TEST(table[1] == "one");
  table[1] = "new";
  BOOST_TEST(table[1] == "new");
}

BOOST_AUTO_TEST_CASE(AtValid)
{
  RobinHoodHashTable< int, std::string > table = {{ 1, "one" }};
  BOOST_TEST(table.at(1) == "one");
  BOOST_CHECK_NO_THROW(table.at(1));
}

BOOST_AUTO_TEST_CASE(AtInvalid) {
  RobinHoodHashTable< int, std::string > table;
  BOOST_CHECK_THROW(table.at(9), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(Find)
{
  RobinHoodHashTable< int, std::string > table = {{ 1, "one" }};
  BOOST_TEST(table.find(1) != table.end());
  BOOST_TEST(table.find(999) == table.end());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MemoryManagement)

BOOST_AUTO_TEST_CASE(LoadFactor)
{
  RobinHoodHashTable< int, std::string > table;
  table.insert(1, "one");
  BOOST_TEST(table.load_factor() > 0.0f);
  BOOST_TEST(table.max_load_factor() == 0.75f);
}

BOOST_AUTO_TEST_CASE(SetMaxLoadFactor)
{
  RobinHoodHashTable< int, std::string > table;
  table.max_load_factor(0.5f);
  BOOST_TEST(table.max_load_factor() == 0.5f);
}

BOOST_AUTO_TEST_CASE(Rehash)
{
  RobinHoodHashTable< int, std::string > table;
  table.rehash(128);
  BOOST_TEST(table.load_factor() == 0.0f);
  table.insert(1, "one");
  BOOST_TEST(table.at(1) == "one");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Capacity)

BOOST_AUTO_TEST_CASE(Size)
{
  RobinHoodHashTable< int, std::string > table;
  BOOST_TEST(table.size() == 0);
  table.insert(1, "one");
  BOOST_TEST(table.size() == 1);
}

BOOST_AUTO_TEST_CASE(Empty)
{
  RobinHoodHashTable< int, std::string > table;
  BOOST_TEST(table.empty());
  table.insert(1, "one");
  BOOST_TEST(!table.empty());
}

BOOST_AUTO_TEST_SUITE_END()
