#include <iostream>
#include <list>
#include <iomanip>
#include "read_expressions.hpp"
#include "conversion.hpp"

int main(int argc, char * argv[])
{
  std::string filename;
  if (argc > 1)
  {
    filename = argv[1];
  }

  std::list< std::string > expressions = smirnov::readExpressions(filename);
  std::list< long long > results;

  std::list< std::string >::iterator it;
  for (it = expressions.begin(); it != expressions.end(); ++it)
  {
    std::string expr = *it;
    try
    {
      smirnov::Queue< std::string > postfix = smirnov::infixToPostfix(expr);
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
    std::list< long long >::iterator rit = results.begin();
    std::cout << *rit;
    ++rit;
    for (; rit != results.end(); ++rit)
    {
      std::cout << " " << *rit;
    }
  }
  std::cout << "\n";
}
