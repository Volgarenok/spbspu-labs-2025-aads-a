#include "convertExpressions.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>

namespace
{
  bool isDigit(std::string& a)
  {
    for (size_t i = 0; i < a.size(); ++i)
    {
      if (a[i] > '9' || a[i] < '0')
      {
        return false;
      }
    }
    return true;
  }

  bool isExpression(std::string& a)
  {
    if ((a[0] == '+' || a[0] == '-' || a[0] == '*' || a[0] == '/' || a[0] == '%') && a.size() == 1)
    {
      return true;
    }
    else
    {
      return false;
    }
  }

  bool isMorePriority(std::string& a, std::string& b)
  {
    if ((a[0] == '+' || a[0] == '-') && (b[0] == '*' || b[0] == '/' || b[0] == '%'))
    {
      return false;
    }
    return true;
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
    std::istringstream iss(str);
    std::string a;
    while (iss >> a)
    {
      inf.push(a);
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

finaev::Queue< std::string > finaev::fromInfToPost(finaev::Queue< std::string >& inf)
{
  finaev::Stack< std::string > temp;
  finaev::Queue< std::string > res;
  bool isBracket = 0;
  while (!inf.isEmpty())
  {
    if (inf.top()[0] == '(' && inf.top().size() == 1)
    {
      isBracket = 1;
      temp.push(inf.top());
      inf.pop();
    }
    else if (isDigit(inf.top()))
    {
      res.push(inf.top());
      inf.pop();
    }
    else if (isExpression(inf.top()))
    {
      if (!temp.isEmpty() && isExpression(temp.top()))
      {
        if (isMorePriority(temp.top(), inf.top()))
        {
          res.push(temp.top());
          temp.pop();
        }
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
      throw std::invalid_argument("invalid argument!\n");
    }
  }
  if (!temp.isEmpty())
  {
    while (!temp.isEmpty())
    {
      res.push(temp.top());
      temp.pop();
    }
  }
  if (isBracket)
  {
    throw std::logic_error("Incorrect Expression!\n");
  }
  return res;
}

long long finaev::calculatePost(finaev::Queue< std::string >& post)
{
  finaev::Stack< long long > res;
  while (post.size() != 0)
  {
    while (!post.isEmpty() && isDigit(post.top()))
    {
      try
      {
        long long num = std::stoll(post.top(), nullptr, 10);
        res.push(num);
        post.pop();
      }
      catch (const std::overflow_error&)
      {
        throw;
      }
    }
    if (res.size() == 1)
    {
      return res.top();
    }
    long long s = res.top();
    res.pop();
    long long f = res.top();
    res.pop();
    res.push(finaev::count(f, s, post.top()[0]));
    post.pop();
  }
  return res.top();
}
