#include "io-infix.hpp"

sveshnikov::Queue< sveshnikov::Postfix > sveshnikov::inputInfix(std::istream &in)
{
  sveshnikov::Queue< sveshnikov::Postfix > infix_exprs;
  while (!in.eof())
  {
    std::string expr;
    std::getline(in, expr);
    if (expr.empty())
    {
      continue;
    }
    sveshnikov::Postfix postf_expr(splitIntoTokens(expr));
    infix_exprs.push(postf_expr);
  }
  return infix_exprs;
}

sveshnikov::Queue< std::string > sveshnikov::splitIntoTokens(const std::string &expr)
{
  sveshnikov::Queue< std::string > splited_expr;
  std::string token;
  for (auto it = expr.cbegin(); it != expr.cend(); it++)
  {
    if (std::isspace(*it))
    {
      if (!token.empty())
      {
        splited_expr.push(token);
        token.clear();
      }
    }
    else
    {
      token += *it;
    }
  }
  if (!token.empty())
  {
    splited_expr.push(token);
  }
  return splited_expr;
}
