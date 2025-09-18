#include "input_output_expressions.hpp"

void lanovenko::inputExpression(std::istream& in, Queue< std::string >& infixExpressions)
{
  while (!in.eof())
  {
    std::string expression = "";
    std::getline(in, expression, '\n');
    if (!expression.empty())
    {
      infixExpressions.push(expression);
    }
  }
}

void lanovenko::resultsOutput(Stack< long long >& results, std::ostream& out)
{
  if (!results.empty())
  {
    out << results.top();
    results.pop();
  }
  while (!results.empty())
  {
    out << " " << results.top();
    results.pop();
  }
  out << '\n';
}
