#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "queue.hpp"
#include "stack.hpp"

namespace
{
  void inputExprs(std::istream & in, mozhegova::Queue< mozhegova::Queue< std::string > > & queue)
  {
    std::string line;
    while (std::getline(in, line))
    {
      mozhegova::Queue< std::string > infExpr;
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

  mozhegova::Queue< mozhegova::Queue< std::string > > convertInfToPost(mozhegova::Queue< mozhegova::Queue< std::string > > & queue)
  {
    mozhegova::Queue< mozhegova::Queue< std::string > > newQueue;
    while (!queue.empty())
    {
      mozhegova::Queue< std::string > infExpr = queue.front();
      queue.pop();
      mozhegova::Queue< std::string > postExpr;
      mozhegova::Stack< std::string > stack;
      while (!infExpr.empty())
      {
        std::string token = infExpr.front();
        infExpr.pop();
        try
        {
          std::stoll(token);
          stack.push(token);
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
            while (!stack.empty() && stack.top() != "(" && isHighPriority(token, stack.top()))
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
    if ((a > max / b) || (a < min / b))
    {
      throw std::overflow_error("overflow");
    }
    return a * b;
  }

  long long divWithCheck(long long a, long long b)
  {
    const long long min = std::numeric_limits< long long >::min();
    if (a == min && b == -1)
    {
      throw std::overflow_error("overflow");
    }
    return a / b;
  }

  long long divRemWithCheck(long long a, long long b)
  {
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
    else
    {
      return divRemWithCheck(a, b);
    }
  }

  mozhegova::Stack< long long > calculateExprs(mozhegova::Queue< mozhegova::Queue< std::string > > & queue)
  {
    mozhegova::Stack< long long > res;
    while (!queue.empty())
    {
      mozhegova::Queue< std::string > postExpr = queue.front();
      queue.pop();
      mozhegova::Stack< long long > stack;
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
          long long res = calculateWithCheck(token, a, b);
          stack.push(res);
        }
      }
      res.push(stack.top());
    }
    return res;
  }

  void outputRes(std::ostream & out, mozhegova::Stack< long long > & res)
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
  using namespace mozhegova;
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
}
