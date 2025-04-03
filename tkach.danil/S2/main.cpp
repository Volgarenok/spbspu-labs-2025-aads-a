#include <iostream>
#include <fstream>
#include <limits>
#include <cstddef>
#include "queue.hpp"
#include "stack.hpp"

namespace
{
  int getPriority(std::string op)
  {
    if (op == "+" || op == "-")
    {
      return 0;
    }
    else if (op == "*" || op == "/" || op == "%")
    {
      return 1;
    }
    return -1;
  }

  bool isLowPriority(std::string op1, std::string op2)
  {
    return getPriority(op1) < getPriority(op2);
  }

  bool isNumber(std::string& s)
  {
    if (s.empty())
    {
      return false;
    }
    size_t i = 0;
    if (s[0] == '-' && s.size() != 1)
    {
      ++i;
    }
    for (; i < s.size(); ++i)
    {
      if (!std::isdigit(s[i]))
      {
        return false;
      }
    }
    return true;
  }

  bool isOperator(std::string& s)
  {
    if (s.empty())
    {
      return false;
    }
    if (s == "+" || s == "-" || s == "%" || s == "/" || s == "*")
    {
      return true;
    }
    return false;
  }

  void inputInfix(std::istream& in, tkach::Queue< std::string >& queue)
  {
    in >> std::noskipws;
    std::string infix = "";
    std::getline(in, infix);
    if (infix.empty())
    {
      return;
    }
    std::string temp = "";
    for(char c : infix)
    {
      if (('0' <= c && '9' >= c) || c == '-')
      {
        temp += c;
      }
      else if (c == ' ')
      {
        if (!temp.empty())
        {
          queue.push(temp);
          temp.clear();
        }
      }
      else
      {
        temp += c;
        queue.push(temp);
        temp.clear();
      }
    }
    if (!temp.empty())
    {
      queue.push(temp);
      temp.clear();
    }
  }

  void input(std::istream& in, tkach::Queue< tkach::Queue< std::string > >& queue)
  {
    while (!in.eof())
    {
      tkach::Queue< std::string > temp_inf_expr;
      inputInfix(in, temp_inf_expr);
      if (!temp_inf_expr.empty())
      {
        queue.push(temp_inf_expr);
      }
    }
  }

  tkach::Queue< std::string > convertInfToPost(tkach::Queue< std::string > inf)
  {
    tkach::Queue< std::string > post_queue;
    tkach::Stack< std::string > stack;
    while (!inf.empty())
    {
      std::string temp = inf.front();
      if (isNumber(temp))
      {
        post_queue.push(temp);
      }
      else if (temp == "(")
      {
        stack.push(temp);
      }
      else if (temp == ")")
      {
        if (stack.empty())
        {
          throw std::logic_error("no open bracket");
        }
        std::string stack_temp = stack.top();
        while (stack_temp != "(")
        {
          post_queue.push(stack_temp);
          stack.pop();
          stack_temp = stack.top();
        }
        stack.pop();
      }
      else if (isOperator(temp))
      {
        while (!stack.empty())
        {
          std::string stack_temp = stack.top();
          if (stack_temp != "(" || (isOperator(stack_temp) && !isLowPriority(stack_temp, temp)))
          {
            post_queue.push(stack_temp);
            stack.pop();
          }
          else
          {
            break;
          }
        }
        stack.push(temp);
      }
      else
      {
        throw std::logic_error("no that sign needed in expression");
      }
      inf.pop();
    }
    while (!stack.empty())
    {
      std::string stack_temp = stack.top();
      post_queue.push(stack_temp);
      stack.pop();
    }
    return post_queue;
  }

  bool isSameSign(long long a, long long b)
  {
    return ((a > 0 && b > 0) || ((a < 0) && (b < 0)));
  }

  bool isOverflowed(std::string s, long long a, long long b)
  {
    const long long max = std::numeric_limits< long long >::max();
    const long long min = std::numeric_limits< long long >::min();
    if (s == "+" || s == "-")
    {
      if (s == "-")
      {
        b *= (-1);
      }
      if (a > 0 && b > 0)
      {
        return (max - a < b);
      }
      if (a < 0 && b < 0)
      {
        return (min - a > b);
      }
    }
    if (s == "*")
    {
      if (b == 0 || a == 0)
      {
        return false;
      }
      if ((a == min && b == (-1)) || (b == min && a == (-1)))
      {
        return true;
      }
      if (isSameSign(a,b))
      {
        if (b < 0)
        {
          return (a < max / b);
        }
        else
        {
          return (a > max / b);
        }
      }
      else
      {
        if (b < 0)
        {
          return (b < min / a);
        }
        else
        {
          return (a < min / b);
        }
      }
    }
    if (s == "/")
    {
      if (b == 0)
      {
        throw std::logic_error("division by zero");
      }
      return ((a == min && b == (-1)) || (b == min && a == (-1)));
    }
    if (s == "%")
    {
      if (b == 0)
      {
        throw std::logic_error("mod by zero");
      }
    }
    return false;
  }

  long long calculateExpr(tkach::Queue< std::string > post)
  {
    tkach::Stack< long long > stack;
    while (!post.empty())
    {
      std::string temp = post.front();
      if (isNumber(temp))
      {
        stack.push(std::stoll(temp));
      }
      else if (isOperator(temp))
      {
        if (stack.size() < 2)
        {
          throw std::logic_error("Error: wrong postfix expression");
        }
        long long val1 = stack.top();
        stack.pop();
        long long val2 = stack.top();
        stack.pop();
        if (isOverflowed(temp, val2, val1))
        {
          throw std::overflow_error("overflow");
        }
        if (temp == "+")
        {
          stack.push(val1 + val2);
        }
        else if (temp == "-")
        {
          stack.push(val2 - val1);
        }
        else if (temp == "*")
        {
          stack.push(val2 * val1);
        }
        else if (temp == "/")
        {
          stack.push(val2 / val1);
        }
        else if (temp == "%")
        {
          long long result = val2 % val1;
          if (val2 < 0)
          {
            result += std::abs(val1);
          }
          stack.push(result);
        }
      }
      else
      {
        throw std::invalid_argument("wrong posstfix expression");
      }
      post.pop();
    }
    return stack.top();
  }

  void print(std::ostream& out, tkach::Stack< long long > stack)
  {
    if (stack.empty())
    {
      return;
    }
    out << stack.top();
    stack.pop();
    while (!stack.empty())
    {
      out << " " << stack.top();
      stack.pop();
    }
  }
}
int main(const int argc, const char* const* const argv)
{
  using namespace tkach;
  Queue< Queue< std::string > > inf_exprs;
  Stack< long long > results;
  try
  {
    if (argc == 1)
    {
      input(std::cin, inf_exprs);
    }
    else
    {
      std::ifstream file(argv[1]);
      if (!file)
      {
        std::cerr << "Error: file not found\n";
        return 1;
      }
      input(file, inf_exprs);
    }
    while (!inf_exprs.empty())
    {
      Queue< std::string > temp_inf = inf_exprs.front();
      if (!temp_inf.empty())
      {
        Queue< std::string > temp_post = convertInfToPost(temp_inf);
        results.push(calculateExpr(temp_post));
      }
      inf_exprs.pop();
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  print(std::cout, results);
  std::cout << "\n";
  return 0;
}
