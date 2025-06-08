#include <iostream>
#include <fstream>
#include <limits>
#include <cstddef>
#include "queue.hpp"
#include "stack.hpp"
#include "print.hpp"

namespace
{
  int getPriority(const std::string& op)
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

  bool isHighPriority(const std::string& op1, const std::string& op2)
  {
    return getPriority(op1) > getPriority(op2);
  }

  bool isOperator(const std::string& s)
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
    std::string infix = "";
    std::getline(in, infix);
    if (infix.empty())
    {
      return;
    }
    size_t pos = 0, pos2 = 0;
    std::string temp = "";
    while (pos2 != std::string::npos)
    {
      pos2 = infix.find(' ', pos);
      temp = infix.substr(pos, pos2 - pos);
      if (!temp.empty())
      {
        queue.push(temp);
        temp.clear();
      }
      pos = pos2 + 1;
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
      try
      {
        std::stoll(temp);
        post_queue.push(temp);
      }
      catch (...)
      {
        if (temp == "(")
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
          while (!stack.empty() && stack.top() != "(")
          {
            std::string stack_temp = stack.top();
            if (isOperator(stack_temp) && !isHighPriority(temp, stack_temp))
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

  bool isOverflowedAdd(long long a, long long b)
  {
    const long long max = std::numeric_limits< long long >::max();
    const long long min = std::numeric_limits< long long >::min();
    if (a > 0 && b > 0)
    {
      return (max - a < b);
    }
    if (a < 0 && b < 0)
    {
      return (min - a > b);
    }
    return false;
  }

  bool isOverflowedSubstr(long long a, long long b)
  {
    return isOverflowedAdd(a, -1 * b);
  }

  bool isOverflowedMult(long long a, long long b)
  {
    const long long max = std::numeric_limits< long long >::max();
    const long long min = std::numeric_limits< long long >::min();
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

  bool isOverflowedDivide(long long a, long long b)
  {
    const long long min = std::numeric_limits< long long >::min();
    return ((a == min && b == (-1)) || (b == min && a == (-1)));
  }

  long long calculateExpr(tkach::Queue< std::string > post)
  {
    tkach::Stack< long long > stack;
    while (!post.empty())
    {
      std::string temp = post.front();
      try
      {
        stack.push(std::stoll(temp));
      }
      catch (...)
      {
        if (isOperator(temp))
        {
          if (stack.size() < 2)
          {
            throw std::logic_error("Error: wrong postfix expression");
          }
          long long val1 = stack.top();
          stack.pop();
          long long val2 = stack.top();
          stack.pop();
          if (temp == "+")
          {
            if (isOverflowedAdd(val2, val1))
            {
              throw std::overflow_error("overflow/underflow with add");
            }
            stack.push(val1 + val2);
          }
          else if (temp == "-")
          {
            if (isOverflowedSubstr(val2, val1))
            {
              throw std::overflow_error("overflow/underflow with substract");
            }
            stack.push(val2 - val1);
          }
          else if (temp == "*")
          {
            if (isOverflowedMult(val2, val1))
            {
              throw std::overflow_error("overflow/underflow with multiply");
            }
            stack.push(val2 * val1);
          }
          else if (temp == "/")
          {
            if (val1 == 0)
            {
              throw std::logic_error("division by zero");
            }
            if (isOverflowedDivide(val2, val1))
            {
              throw std::overflow_error("overflow/underflow with divide");
            }
            stack.push(val2 / val1);
          }
          else if (temp == "%")
          {
            if (val1 == 0)
            {
              throw std::logic_error("division by zero");
            }
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
      }
      post.pop();
    }
    return stack.top();
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
  outputStack(std::cout, results);
  std::cout << "\n";
  return 0;
}
