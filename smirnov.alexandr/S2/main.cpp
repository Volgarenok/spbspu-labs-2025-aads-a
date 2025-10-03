#include <iostream>
#include <iomanip>
#include <list/list.hpp>
#include "read_expressions.hpp"
#include "conversion.hpp"

int main(int argc, char * argv[])
{
  using namespace smirnov;
  std::string filename;
  if (argc > 1)
  {
    filename = argv[1];
  }
  List< std::string > expressions = readExpressions(filename);
  List< long long > results;
  for (auto it = expressions.begin(); it != expressions.end(); ++it)
  {
    std::string expr = *it;
    try
    {
      Queue< std::string > postfix = infixToPostfix(expr);
      long long result = evaluatePostfix(postfix);
      results.push_front(result);
    }
    catch (const std::exception & e)
    {
      std::cerr << e.what() << "\n";
      return 1;
    }
  }
  if (!results.empty())
  {
    auto rit = results.begin();
    std::cout << *rit;
    ++rit;
    for (; rit != results.end(); ++rit)
    {
      std::cout << " " << *rit;
    }
  }
  std::cout << "\n";
}
