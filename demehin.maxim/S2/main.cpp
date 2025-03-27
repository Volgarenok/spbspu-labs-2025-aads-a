#include <iostream>
#include <fstream>
#include <string>
#include <stack.hpp>
#include <queue.hpp>
#include "exprs_manip.hpp"

namespace
{
  using ExprsStack = demehin::Stack< demehin::Queue < std::string > >;

  void inputExprs(std::istream& in, ExprsStack& stack)
  {
    demehin::Queue< std::string > exprQueue;
    std::string line;

    while (std::getline(in, line))
    {
      if (line.empty())
      {
        if (!exprQueue.empty())
        {
          stack.push(exprQueue);
        }
      }

      std::string token;
      for (char c : line)
      {
        if (std::isspace(c))
        {
          continue;
        }
        else if (std::isdigit(c))
        {
          token += c;
        }
        else
        {
          if (!token.empty())
          {
            exprQueue.push(token);
            token.clear();
          }
          exprQueue.push(std::string(1, c));
        }
      }
      if (!token.empty())
      {
        exprQueue.push(token);
      }
      if (!exprQueue.empty())
      {
        stack.push(exprQueue);
        exprQueue.clear();
      }
    }
  }

  void printValues(std::ostream& out, demehin::Stack< long long int  > stack)
  {
    if (stack.empty())
    {
      return;
    }
    out << stack.top();
    stack.pop();
    while (!stack.empty())
    {
      std::cout << " " << stack.top();
      stack.pop();
    }
  }
}

int main(int argc, char* argv[])
{
  ExprsStack infExprsStack;
  try
  {
    if (argc > 1)
    {
      std::ifstream file(argv[1]);
      inputExprs(file, infExprsStack);
    }
    else
    {
      inputExprs(std::cin, infExprsStack);
    }

    ExprsStack postExprsStack;
    demehin::convertStack(infExprsStack, postExprsStack);

    demehin::Stack< long long int > res;
    demehin::getExprsValues(postExprsStack, res);
    printValues(std::cout, res);
    std::cout << "\n";
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}

