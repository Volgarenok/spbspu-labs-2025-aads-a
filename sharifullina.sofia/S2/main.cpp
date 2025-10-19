#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "queue.hpp"
#include "stack.hpp"

namespace
{
  void inputExprs(std::istream & in,
    sharifullina::Queue< sharifullina::Queue< std::string > > & queue)
  {
    std::string line;
    while (std::getline(in, line))
    {
      sharifullina::Queue< std::string > infExpr;
      if (line.empty())
      {
        continue;
      }
      size_t start = 0;
      size_t end = line.find(' ');
      while (end != std::string::npos)
      {
        infExpr.push(line.substr(start, end - start));
        start = end + 1;
        end = line.find(' ', start);
      }
      infExpr.push(line.substr(start));
      if (!infExpr.empty())
      {
        queue.push(infExpr);
      }
    }
  }

  bool isOperation(std::string op)
  {
    return op == "*" || op == "/" || op == "%" || op == "+" || op == "-";
  }

  int getPriority(std::string op)
  {
    if (op == "+" || op == "-")
    {
      return 1;
    }
    else if (op == "*" || op == "/" || op == "%")
    {
      return 2;
    }
    else
    {
      return 0;
    }
  }

  bool isHighPriority(const std::string & op1, const std::string & op2)
  {
    return getPriority(op1) >= getPriority(op2);
  }

  sharifullina::Queue< sharifullina::Queue< std::string > > convertInfToPost(
    sharifullina::Queue< sharifullina::Queue< std::string > > & queue)
  {
    sharifullina::Queue< sharifullina::Queue< std::string > > newQueue;
    while (!queue.empty())
    {
      sharifullina::Queue< std::string > infExpr = queue.front();
      queue.pop();
      sharifullina::Queue< std::string > postExpr;
      sharifullina::Stack< std::string > stack;
      while (!infExpr.empty())
      {
        std::string token = infExpr.front();
        infExpr.pop();
        try
        {
          std::stoll(token);
          postExpr.push(token);
        }
        catch (const std::exception &)
        {
          if (token == "(")
          {
            stack.push(token);
          }
          else if (token == ")")
          {
            while (!stack.empty() && stack.top() != "(")
            {
              postExpr.push(stack.top());
              stack.pop();
            }
            if (stack.empty())
            {
              throw std::logic_error("invalid brackets");
            }
            stack.pop();
          }
          else if (isOperation(token))
          {
            while (!stack.empty() && stack.top() != "(" &&
              isHighPriority(stack.top(), token))
            {
              postExpr.push(stack.top());
              stack.pop();
            }
            stack.push(token);
          }
          else
          {
            throw std::logic_error("invalid expression");
          }
        }
      }
      while (!stack.empty())
      {
        if (stack.top() == "(")
        {
          throw std::logic_error("invalid brackets");
        }
        postExpr.push(stack.top());
        stack.pop();
      }
      newQueue.push(postExpr);
    }
    return newQueue;
  }

  long long addWithCheck(long long a, long long b)
  {
    const long long max = std::numeric_limits< long long >::max();
    if (a > max - b)
    {
      throw std::overflow_error("overflow");
    }
    return a + b;
  }

  long long subWithCheck(long long a, long long b)
  {
    const long long min = std::numeric_limits< long long >::min();
    if (a < min + b)
    {
      throw std::overflow_error("overflow");
    }
    return a - b;
  }

  long long mulWithCheck(long long a, long long b)
  {
    const long long max = std::numeric_limits< long long >::max();
    const long long min = std::numeric_limits< long long >::min();
    if (a > 0)
    {
      if (b > 0)
      {
        if (a > max / b)
        {
          throw std::overflow_error("overflow");
        }
      }
      else
      {
        if (b < min / a)
        {
          throw std::overflow_error("overflow");
        }
      }
    }
    else
    {
      if (b > 0)
      {
        if (a < min / b)
        {
          throw std::overflow_error("overflow");
        }
      }
      else
      {
        if (a != 0 && b < max / a)
        {
          throw std::overflow_error("overflow");
        }
      }
    }
    return a * b;
  }

  long long divWithCheck(long long a, long long b)
  {
    if (b == 0)
    {
      throw std::logic_error("division by zero");
    }
    const long long min = std::numeric_limits< long long >::min();
    if (a == min && b == -1)
    {
      throw std::overflow_error("overflow");
    }
    return a / b;
  }

  long long modWithCheck(long long a, long long b)
  {
    if (b == 0)
    {
      throw std::logic_error("modulo by zero");
    }
    long long res = a % b;
    if (res < 0)
    {
      res += std::abs(b);
    }
    return res;
  }

  long long calculateWithCheck(std::string op, long long a, long long b)
  {
    if (op == "+")
    {
      return addWithCheck(a, b);
    }
    else if (op == "-")
    {
      return subWithCheck(a, b);
    }
    else if (op == "*")
    {
      return mulWithCheck(a, b);
    }
    else if (op == "/")
    {
      return divWithCheck(a, b);
    }
    else if (op == "%")
    {
      return modWithCheck(a, b);
    }
    else
    {
      throw std::logic_error("unknown operation");
    }
  }

  sharifullina::Stack< long long > calculateExprs(
    sharifullina::Queue< sharifullina::Queue< std::string > > & queue)
  {
    sharifullina::Stack< long long > res;
    while (!queue.empty())
    {
      sharifullina::Queue< std::string > postExpr = queue.front();
      queue.pop();
      sharifullina::Stack< long long > stack;
      while (!postExpr.empty())
      {
        std::string token = postExpr.front();
        postExpr.pop();
        try
        {
          stack.push(std::stoll(token));
        }
        catch (const std::exception &)
        {
          long long b = stack.top();
          stack.pop();
          long long a = stack.top();
          stack.pop();
          long long result = calculateWithCheck(token, a, b);
          stack.push(result);
        }
      }
      res.push(stack.top());
    }
    return res;
  }

  void outputRes(std::ostream & out, sharifullina::Stack< long long > & res)
  {
    if (!res.empty())
    {
      out << res.top();
      res.pop();
      while (!res.empty())
      {
        out << " " << res.top();
        res.pop();
      }
    }
  }
}

int main(int argc, char * argv[])
{
  using namespace sharifullina;
  Queue< Queue< std::string > > infExprs;
  Queue< Queue< std::string > > postExprs;
  Stack< long long > results;
  try
  {
    if (argc > 1)
    {
      std::ifstream file(argv[1]);
      inputExprs(file, infExprs);
    }
    else
    {
      inputExprs(std::cin, infExprs);
    }
    postExprs = convertInfToPost(infExprs);
    results = calculateExprs(postExprs);
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  outputRes(std::cout, results);
  std::cout << "\n";
  return 0;
}
