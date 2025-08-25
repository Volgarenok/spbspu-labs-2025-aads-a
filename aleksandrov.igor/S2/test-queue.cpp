#include <boost/test/unit_test.hpp>
#include "queue.hpp"

using aleksandrov::Queue;

namespace
{
  struct Point
  {
    int x;
    int y;

    Point() = delete;
    Point(int x, int y):
      x(x),
      y(y)
    {}

    bool operator==(const Point& rhs) const
    {
      return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const Point& rhs) const
    {
      return !operator==(rhs);
    }
  };

  std::ostream& operator<<(std::ostream& out, const Point& rhs)
  {
    return out << '(' << rhs.x << ';' << rhs.y << ')';
  }
}

BOOST_AUTO_TEST_CASE(queue_default_construction)
{
  using aleksandrov::minDequeCapacity;

  Queue< Point > q;
  BOOST_TEST(q.empty());
  BOOST_TEST(q.size() == 0);
  BOOST_TEST(q.maxSize() >= 0);
  BOOST_TEST(q.capacity() == 0);
}

BOOST_AUTO_TEST_CASE(queue_copy_construction)
{
  Queue< Point > q1;
  Queue< Point > q1Copy(q1);
  BOOST_TEST(q1Copy.empty());
  BOOST_TEST(q1Copy.size() == 0);
  BOOST_TEST(q1Copy.capacity() == 0);

  Queue< Point > q2;
  q2.push(Point(1, 1));
  q2.push(Point(2, 2));
  Queue< Point > q2Copy(q2);
  BOOST_TEST(q2Copy.size() == 2);
  BOOST_TEST(q2Copy.capacity() == q2.capacity());
  BOOST_TEST(q2Copy.front() == Point(1, 1));
}

BOOST_AUTO_TEST_CASE(queue_move_construction)
{
  Queue< Point > q1;
  Queue< Point > q1Copy(std::move(q1));
  BOOST_TEST(q1Copy.empty());
  BOOST_TEST(q1Copy.size() == 0);
  BOOST_TEST(q1Copy.capacity() == 0);

  Queue< Point > q2;
  q2.push(Point(1, 1));
  q2.push(Point(2, 2));
  Queue< Point > q2Copy(std::move(q2));
  BOOST_TEST(q2Copy.size() == 2);
  BOOST_TEST(q2Copy.front() == Point(1, 1));
  BOOST_TEST(q2.empty());
  BOOST_TEST(q2.capacity() == 0);
}

BOOST_AUTO_TEST_CASE(queue_copy_assignment)
{
  Queue< Point > q1;
  Queue< Point > q2;
  q2 = q1;
  BOOST_TEST(q2.empty());
  BOOST_TEST(q2.capacity() == 0);

  q1.push(Point(1, 1));
  q1.push(Point(2, 2));
  q2 = q1;
  BOOST_TEST(q2.size() == 2);
  BOOST_TEST(q2.capacity() == q1.capacity());
  BOOST_TEST(q2.front() == Point(1, 1));

  q1.clear();
  q2 = q1;
  BOOST_TEST(q2.empty());
  BOOST_TEST(q2.capacity() == q1.capacity());

  q1.push(Point(1, 1));
  q1.push(Point(2, 2));
  q2.push(Point(3, 3));
  q2.push(Point(4, 4));
  q2 = q1;
  BOOST_CHECK(q2 == q1);
}

BOOST_AUTO_TEST_CASE(queue_move_assignment)
{
  Queue< Point > q1;
  Queue< Point > q2;
  q2 = std::move(q1);
  BOOST_TEST(q2.empty());
  BOOST_TEST(q2.capacity() == 0);

  q1.push(Point(1, 1));
  q1.push(Point(2, 2));
  q2 = std::move(q1);
  BOOST_TEST(q2.size() == 2);
  BOOST_TEST(q2.front() == Point(1, 1));
  BOOST_TEST(q1.empty());
  BOOST_TEST(q1.capacity() == 0);

  q2 = std::move(q1);
  BOOST_TEST(q2.empty());
  BOOST_TEST(q2.capacity() == 0);

  q2 = std::move(q1);
  BOOST_CHECK(q2 == q1);
}

BOOST_AUTO_TEST_CASE(queue_front)
{
  Queue< double > q;
  q.push(1.0);
  BOOST_TEST(q.front() == 1.0);

  q.push(2.0);
  BOOST_TEST(q.front() == 1.0);

  q.pop();
  BOOST_TEST(q.front() == 2.0);

  q.front() = 2;
  BOOST_TEST(q.front() == 2.0);
}

BOOST_AUTO_TEST_CASE(queue_empty_size_capacity)
{
  Queue< float > q;
  BOOST_TEST(q.empty());
  BOOST_TEST(q.size() == 0);
  BOOST_TEST(q.capacity() == 0);

  q.push(2.5f);
  BOOST_TEST(!q.empty());
  BOOST_TEST(q.size() == 1);
  BOOST_TEST(q.capacity() > 0);

  q.push(2.6f);
  BOOST_TEST(q.size() == 2);

  q.clear();
  BOOST_TEST(q.empty());
  BOOST_TEST(q.size() == 0);
  BOOST_TEST(q.capacity() > 0);
}

BOOST_AUTO_TEST_CASE(queue_shrink_to_fit)
{
  Queue< Point > q;
  q.shrinkToFit();
  BOOST_TEST(q.capacity() == 0);

  q.push(Point(0, 0));
  q.shrinkToFit();
  BOOST_TEST(q.capacity() == q.size());

  q.push(Point(0, 0));
  q.pop();
  q.push(Point(0, 0));
  q.shrinkToFit();
  q.shrinkToFit();
  BOOST_TEST(q.capacity() == q.size());
}

BOOST_AUTO_TEST_CASE(queue_clear)
{
  Queue< Point > q;
  q.clear();
  BOOST_TEST(q.empty());
  BOOST_TEST(q.capacity() == 0);

  q.push(Point(0, 0));
  q.clear();
  BOOST_TEST(q.empty());
  BOOST_TEST(q.capacity() > 0);

  q.push(Point(0, 0));
  q.push(Point(0, 0));
  q.clear();
  q.clear();
  BOOST_TEST(q.empty());
}

BOOST_AUTO_TEST_CASE(queue_push)
{
  Queue< Point > polygon;
  Point point(1, 2);

  polygon.push(point);
  BOOST_TEST(polygon.front() == point);

  polygon.push(Point(2, 0));
  BOOST_TEST(polygon.front() == point);
}

BOOST_AUTO_TEST_CASE(queue_emplace)
{
  Queue< Point > polygon;
  Point point(-1, -2);

  polygon.emplace(point);
  BOOST_TEST(polygon.front() == point);

  polygon.emplace(Point(-3, -4));
  BOOST_TEST(polygon.front() == point);

  polygon.emplace(0, 6);
  BOOST_TEST(polygon.front() == point);
}

BOOST_AUTO_TEST_CASE(queue_capacity_full)
{
  using aleksandrov::minDequeCapacity;

  Queue< int > q;
  for (size_t i = 0; i < minDequeCapacity; ++i)
  {
    q.push(0);
  }
  BOOST_TEST(q.size() == q.capacity());

  q.push(1);
  BOOST_TEST(q.size() == minDequeCapacity + 1);
  BOOST_TEST(q.capacity() >= q.size());

  q.pop();
  BOOST_TEST(q.size() != q.capacity());
  BOOST_TEST(q.capacity() > minDequeCapacity);

  q.clear();
  BOOST_TEST(q.empty());
  BOOST_TEST(q.capacity() > minDequeCapacity);

  Queue< char > other;
  BOOST_TEST(other.capacity() < q.capacity());
}

BOOST_AUTO_TEST_CASE(queue_pop)
{
  Queue< Point > q;
  q.push(Point(0, 0));
  q.pop();
  BOOST_TEST(q.empty());
  BOOST_TEST(q.capacity() > 0);

  q.push(Point(1, 1));
  q.push(Point(2, 2));
  q.pop();
  BOOST_TEST(q.size() == 1);
  BOOST_TEST(q.front() == Point(2, 2));

  q.pop();
  BOOST_TEST(q.empty());
}

BOOST_AUTO_TEST_CASE(queue_swap)
{
  Queue< int > q1;
  Queue< int > q2;
  q1.swap(q2);
  BOOST_CHECK(q1 == q2);

  q1.push(1);
  q2.push(2);
  q2.push(3);
  q2.swap(q1);
  BOOST_TEST(q1.front() == 2);

  q1.swap(q1);
  BOOST_TEST(q1.front() == 2);
}

BOOST_AUTO_TEST_CASE(queue_equal_operators)
{
  Queue< Point > q1;
  Queue< Point > q2;
  BOOST_CHECK(q1 == q2);

  q1.push(Point(1, 1));
  q1.pop();
  BOOST_CHECK(q1 == q2);

  q1.push(Point(1, 1));
  BOOST_CHECK(q1 != q2);

  q2.push(Point(1, 1));
  BOOST_CHECK(q2 == q1);
}

