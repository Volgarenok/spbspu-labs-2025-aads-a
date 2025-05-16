#include <boost/test/unit_test.hpp>
#include "io-utils.hpp"
#include <string>
#include <sstream>
#include <fstream>
#include "queue.hpp"
#include "stack.hpp"

using namespace kushekbaev;

void createFile(const std::string& filename, const std::string& content)
{
  std::ofstream out(filename);
  out << content;
  out.close();
}

BOOST_AUTO_TEST_SUITE(io_utils)

BOOST_AUTO_TEST_CASE(splitExprTest)
{
  std::string str = "12 + 34 - 56";
  Queue< std::string > arithmeticQ;
  splitExpr(str, arithmeticQ);
  BOOST_TEST(arithmeticQ.front() == "12");
  arithmeticQ.pop();
  BOOST_TEST(arithmeticQ.front() == "+");
  arithmeticQ.pop();
  BOOST_TEST(arithmeticQ.front() == "34");
  arithmeticQ.pop();
  BOOST_TEST(arithmeticQ.front() == "-");
  arithmeticQ.pop();
  BOOST_TEST(arithmeticQ.front() == "56");
}

BOOST_AUTO_TEST_CASE(inputExprTest)
{
  std::string str = "31 * 5\n 5 / 4\n 3 % 2\n 20 - 10\n" "\n" "44 + 13\n";
  std::istringstream input(str);
  Queue< Queue < std::string > > Q;
  inputExpr(input, Q);
  BOOST_TEST(Q.size() == 5);
  auto firstExpr = Q.front();
  Q.pop();
  BOOST_TEST(firstExpr.front() == "31");
  firstExpr.pop();
}

BOOST_AUTO_TEST_CASE(openFileTest)
{
  const std::string filename1 = "filename1.txt";
  const std::string content1 = "1 + 3\n" "6 * 5\n";
  createFile(filename1, content1);
  Queue< Queue < std::string > > res1;
  try
  {
    openFile(filename1, res1);
  }
  catch (...)
  {
    BOOST_TEST(1 != 1);
  }
  BOOST_TEST(res1.size() == 2);
  auto firstExpr1 = res1.front();
  res1.pop();
  auto lastExpr1 = res1.front();
  BOOST_TEST(firstExpr1.size() == 3);
  BOOST_TEST(lastExpr1.size() == 3);
  BOOST_TEST(firstExpr1.front() == "1");
  BOOST_TEST(lastExpr1.front() == "6");
}

BOOST_AUTO_TEST_CASE(outputTest)
{
  Stack< long long int > results;
  results.push(1);
  results.push(20);
  results.push(300);
  BOOST_TEST(!results.empty());
  std::ostringstream oss;
  output(oss, results);
  BOOST_TEST(oss.str() == "300 20 1");
  BOOST_TEST(results.empty());
}

BOOST_AUTO_TEST_SUITE_END()
