#include "postfix-utils.h"
#include "stack.hpp"
#include "postfix-expression.h"

void savintsev::evaluate_postfix_stream(std::istream & in)
{
  using namespace std;

  string text;
  Stack< long long > results;

  while (in)
  {
    getline(in, text);
    if (text.empty())
    {
      continue;
    }

    PostfixExpr expr = convert(text);
    results.push(expr());
  }

  if (results.empty())
  {
    cout << '\n';
    return;
  }

  cout << results.top();
  results.pop();

  while (!results.empty())
  {
    cout << " " << results.top();
    results.pop();
  }

  cout << '\n';
}
