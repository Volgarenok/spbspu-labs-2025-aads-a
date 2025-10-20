#include "io-infix.hpp"

sveshnikov::Queue< sveshnikov::Postfix > sveshnikov::inputInfix(std::istream &in)
{
  Queue< Postfix > infix_exprs;
  while (!in.eof())
  {
    std::string expr;
    std::getline(in, expr);
    if (expr.empty())
    {
      continue;
    }
    Postfix postf_expr(splitIntoTokens(expr));
    infix_exprs.push(postf_expr);
  }
  return infix_exprs;
}

sveshnikov::Queue< std::string > sveshnikov::splitIntoTokens(const std::string &expr)
{
  Queue< std::string > splited_expr;
  size_t end = 0;
  size_t start = expr.find_first_not_of(" \n");
  while (start != std::string::npos)
  {
    end = expr.find_first_of(" \n", start);
    splited_expr.push(expr.substr(start, end - start));
    start = expr.find_first_not_of(" \n", end);
  }
  return splited_expr;
}
