#include "convertExpressions.hpp"
#include <stdexcept>
#include <iostream>

namespace
{
  bool isExpression(const std::string& a)
  {
    return ((a[0] == '+' || a[0] == '-' || a[0] == '*' || a[0] == '/' || a[0] == '%' || a[0] == '^') && a.size() == 1);
  }

  bool isMorePriority(const std::string& a, const std::string& b)
  {
    if (b[0] == '^')
    {
      return false;
    }
    else if ((a[0] == '*' || a[0] == '/' || a[0] == '%') && (b[0] == '+' || b[0] == '-'))
    {
      return true;
    }
    else if (a[0] == '^' && b[0] != '^')
    {
      return true;
    }
    return false;
  }
}

void finaev::countFinalResults(std::istream& in)
{
  std::string str = "";
  finaev::Stack< long long > res;
  while (in)
  {
    std::getline(in, str);
    if (str.size() == 0)
    {
      continue;
    }
    finaev::Queue< std::string > inf;
    size_t start = 0;
    size_t end = str.find(' ', start);
    while (end != std::string::npos)
    {
      std::string substring = str.substr(start, end - start);
      if (!substring.empty())
      {
        inf.push(substring);
      }
      start = end + 1;
      end = str.find(' ', start);
    }
    std::string substring = str.substr(start, str.size());
    if (!substring.empty())
    {
      inf.push(substring);
    }
    finaev::Queue< std::string > post = fromInfToPost(inf);
    res.push(calculatePost(post));
  }
  if (res.isEmpty())
  {
    std::cout << "\n";
    return;
  }
  std::cout << res.top();
  res.pop();
  while (!res.isEmpty())
  {
    std::cout << " " << res.top();
    res.pop();
  }
  std::cout << "\n";
}

finaev::Queue< std::string > finaev::fromInfToPost(Queue< std::string >& inf)
{
  Stack< std::string > temp;
  Queue< std::string > res;
  bool isBracket = 0;
  while (!inf.isEmpty())
  {
    if (inf.top()[0] == '(' && inf.top().size() == 1)
    {
      isBracket = 1;
      temp.push(inf.top());
      inf.pop();
    }
    else if (isExpression(inf.top()))
    {
      while (!temp.isEmpty() && isExpression(temp.top()) && (isMorePriority(temp.top(), inf.top()) || (!isMorePriority(inf.top(), temp.top()))))
      {
        res.push(temp.top());
        temp.pop();
      }
      temp.push(inf.top());
      inf.pop();
    }
    else if (inf.top()[0] == ')' && inf.top().size() == 1)
    {
      isBracket = 0;
      while (temp.top()[0] != '(')
      {
        res.push(temp.top());
        temp.pop();
      }
      temp.pop();
      inf.pop();
    }
    else
    {
      res.push(inf.top());
      inf.pop();
    }
  }
  while (!temp.isEmpty())
  {
    res.push(temp.top());
    temp.pop();
  }
  if (isBracket)
  {
    throw std::logic_error("Incorrect Expression!\n");
  }
  return res;
}

long long finaev::calculatePost(finaev::Queue< std::string >& post)
{
  Stack< long long > res;
  while (!post.isEmpty())
  {
    if (isExpression(post.top()))
    {
      long long s = res.top();
      res.pop();
      long long f = res.top();
      res.pop();
      res.push(finaev::count(f, s, post.top()[0]));
      post.pop();
    }
    else
    {
      long long num = std::stoll(post.top(), nullptr, 10);
      res.push(num);
      post.pop();
    }
  }
  return res.top();
}
