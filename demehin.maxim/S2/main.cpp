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
    std::string line;
    while (std::getline(in, line))
    {
      if (line.empty())
      {
        continue;
      }

      demehin::Queue< std::string > exprQueue;
      size_t token_begin = 0;
      size_t token_end = line.find(' ');

      while (token_end != std::string::npos)
      {
        exprQueue.push(line.substr(token_begin, token_end - token_begin));
        token_begin = token_end + 1;
        token_end = line.find(' ', token_begin);
      }

      exprQueue.push(line.substr(token_begin));
      if (!exprQueue.empty())
      {
        stack.push(exprQueue);
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
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}

