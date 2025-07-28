#include <boost/test/unit_test.hpp>
#include "queue.hpp"

using aleksandrov::Queue;

namespace
{
  struct Point
  {
    Point(int x, int y):
      x(x),
      y(y)
    {}
    bool operator==(const Point& rhs) const
    {
      return x == rhs.x && y == rhs.y;
    }
    int x;
    int y;
  };

  std::ostream& operator<<(std::ostream& out, const Point& rhs)
  {
    return out << '(' << rhs.x << ';' << rhs.y << ')';
  }
}

BOOST_AUTO_TEST_CASE(queue_construction)
{
  Queue< Point > q1;
  q1.push(Point(1, 2));
  q1.push(Point(3, 4));
  BOOST_TEST(q1.size() == 2);

  Queue< Point > q2(q1);
  BOOST_TEST(q2.size() == 2);
  BOOST_TEST(q2.front() == Point(1, 2));

  Queue< Point > q3(std::move(q1));
  BOOST_TEST(q3.size() == 2);
  BOOST_TEST(q3.front() == Point(1, 2));
}

BOOST_AUTO_TEST_CASE(queue_copy_move)
{
  Queue< Point > q1;
  Queue< Point > q2;
  Queue< Point > q3;
  q1.push(Point(2, 4));
  q1.push(Point(6, 8));

  q2 = q1;
  BOOST_TEST(q2.size() == 2);
  BOOST_TEST(q2.front() == Point(2, 4));

  q3 = std::move(q2);
  BOOST_TEST(q3.size() == 2);
  BOOST_TEST(q3.front() == Point(2, 4));
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
}

BOOST_AUTO_TEST_CASE(queue_empty_size)
{
  Queue< float > q;
  BOOST_TEST(q.empty());
  BOOST_TEST(q.size() == 0);

  q.push(2.5f);
  BOOST_TEST(!q.empty());
  BOOST_TEST(q.size() == 1);

  q.pop();
  BOOST_TEST(q.empty());
  BOOST_TEST(q.size() == 0);
}

BOOST_AUTO_TEST_CASE(queue_clear)
{
  Queue< Point > q;
  q.push(Point(0, 0));

  q.clear();
  BOOST_TEST(q.empty());

  q.push(Point(0, 0));
  q.push(Point(0, 0));
  q.clear();
  BOOST_TEST(q.empty());
}

BOOST_AUTO_TEST_CASE(queue_push_emplace)
{
  Queue< Point > polygon;
  Point point(-1.5, -2.0);

  polygon.push(point);
  BOOST_TEST(polygon.front() == point);

  polygon.pop();
  polygon.emplace(-1.5, -2.0);
  BOOST_TEST(polygon.front() == point);

  polygon.push(Point(2.5, 0.0));
  BOOST_TEST(polygon.front() == point);

  polygon.emplace(-0.5, 4.5);
  BOOST_TEST(polygon.front() == point);
}

BOOST_AUTO_TEST_CASE(queue_capacity_full)
{
  Queue< char > q;
  for (size_t i = 0; i < 65; ++i)
  {
    q.push(1);
  }
  BOOST_TEST(q.size() == 65);
  BOOST_TEST(q.capacity() >= 65);

  q.clear();
  BOOST_TEST(q.empty());
  BOOST_TEST(q.capacity() >= 65);

  Queue< char > g;
  BOOST_TEST(g.capacity() < q.capacity());
}

BOOST_AUTO_TEST_CASE(queue_pop)
{
  Queue< Point > q;
  q.push(Point(0, 0));
  q.push(Point(0, 0));

  q.pop();
  BOOST_TEST(q.size() == 1);
  BOOST_TEST(q.front() == Point(0, 0));

  q.pop();
  BOOST_TEST(q.empty());
}

BOOST_AUTO_TEST_CASE(queue_swap)
{
  Queue< float > q1;
  q1.push(1.0f);
  Queue< float > q2;
  q2.push(2.0f);
  q2.push(3.0f);

  q1.swap(q2);
  BOOST_TEST(q1.front() == 2.0f);

  q1.swap(q1);
  BOOST_TEST(q1.front() == 2.0f);
}

