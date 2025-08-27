#include <iostream>
#include <fstream>
#include "stack.hpp"
#include "expression-utils.hpp"

namespace
{
  using StackOfResults = aleksandrov::Stack< long long int >;
  using aleksandrov::Expressions;

  void getPostfixForms(Expressions& postfixExprs, Expressions& exprs)
  {
    while (!exprs.empty())
    {
      postfixExprs.push(getPostfixForm(exprs.front()));
      exprs.pop();
    }
  }

  void evalPostfixExpressions(Expressions& postfixExprs, StackOfResults& results)
  {
    while (!postfixExprs.empty())
    {
      results.push(evalPostfixExpression(postfixExprs.front()));
      postfixExprs.pop();
    }
  }

  void printStackOfResults(StackOfResults& results, std::ostream& out)
  {
    if (!results.empty())
    {
      out << results.top();
      results.pop();
    }
    while (!results.empty())
    {
      out << ' ' << results.top();
      results.pop();
    }
  }
}

int main(int argc, char* argv[])
{
  using namespace aleksandrov;

  if (argc > 2)
  {
    std::cerr << "ERROR: Incorrect arguments!\n";
    return 1;
  }

  std::ifstream file;
  if (argc == 2)
  {
    file.open(argv[1]);
    if (!file)
    {
      std::cerr << "ERROR: Incorrect file!\n";
      return 1;
    }
  }
  std::istream& in = file.is_open() ? file : std::cin;

  Stack< long long int > results;
  try
  {
    Queue< Queue< ExpressionPart > > exprs;
    getExpressions(exprs, in);

    Queue< Queue< ExpressionPart > > postfixExprs;
    getPostfixForms(postfixExprs, exprs);

    evalPostfixExpressions(postfixExprs, results);
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "ERROR: Out of memory!\n";
    return 2;
  }
  catch (const std::exception& e)
  {
    std::cerr << "ERROR: " << e.what() << '\n';
    return 2;
  }

  printStackOfResults(results, std::cout);
  std::cout << '\n';
}

