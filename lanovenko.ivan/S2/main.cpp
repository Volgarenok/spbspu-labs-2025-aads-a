#include <iostream>
#include <fstream>
#include <string>
#include "stack.hpp"
#include "queue.hpp"
#include "input_output_expressions.hpp"
#include "conversion_to_postfix.hpp"
#include "expression_calculations.hpp"

int main(int argc, char** argv)
{
  using namespace lanovenko;
  if (argc > 2)
  {
    std::cerr << "to much parametrs!\n";
    return 1;
  }
  Queue < std::string > infixExpressions{};
  if (argc == 1)
  {
    inputExpression(std::cin, infixExpressions);
  }
  if (argc == 2)
  {
    std::ifstream input(argv[1]);
    inputExpression(input, infixExpressions);
  }
  Queue< Queue < std::string > > postfixExpressions{};
  while (!infixExpressions.empty())
  {
    std::string expression = infixExpressions.front();
    try
    {
      postfixExpressions.push(toPostfix(expression));
    }
    catch (const std::invalid_argument& e)
    {
      std::cerr << e.what();
      return 1;
    }
    infixExpressions.pop();
  }
  Stack < long long > results{};
  while (!postfixExpressions.empty())
  {
    long long res = 0;
    try
    {
      res = calcualtePostfix(postfixExpressions.front());
    }
    catch (const std::invalid_argument& e)
    {
      std::cerr << e.what();
      return 1;
    }
    catch (const std::logic_error& e)
    {
      std::cerr << e.what();
      return 1;
    }
    results.push(res);
    postfixExpressions.pop();
  }
  resultsOutput(results, std::cout);
}
