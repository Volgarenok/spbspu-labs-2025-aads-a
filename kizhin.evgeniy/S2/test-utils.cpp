#include "test-utils.hpp"

void kizhin::testBufferInvariants(const BufferT& list)
{
  BOOST_TEST(list.empty() == (list.size() == 0));
  BOOST_TEST(list.empty() == (list.begin() == list.end()));
  BOOST_TEST(std::distance(list.begin(), list.end()) == list.size());
  if (list.empty()) {
    return;
  }
  if (list.size() == 1) {
    BOOST_TEST(list.front() == list.back());
  }
  const BufferT::const_iterator beforeEnd = std::next(list.begin(), list.size() - 1);
  BOOST_TEST(std::addressof(list.front()) == std::addressof(*list.begin()));
  BOOST_TEST(std::addressof(list.back()) == std::addressof(*beforeEnd));
}

void kizhin::testBufferComparison(const BufferT& lhs, const BufferT& rhs)
{
  BOOST_TEST((lhs == rhs) == !(lhs != rhs));
  BOOST_TEST((lhs < rhs) == (rhs > lhs));
  BOOST_TEST((lhs < rhs) == !(lhs >= rhs));
  BOOST_TEST((lhs > rhs) == !(lhs <= rhs));
  BOOST_TEST((lhs == rhs) == (lhs <= rhs && lhs >= rhs));
}

