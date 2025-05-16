#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <stdexcept>

#include "queue.hpp"
#include "stack.hpp"
#include "variant.hpp"
#include "safe_math.hpp"

namespace rychkov
{
  int getPriority(char operatorChar)
  {
    switch (operatorChar)
    {
    case '(':
    case ')':
      return 1;
    case '%':
      return 2;
    case '+':
    case '-':
      return 3;
    case '*':
    case '/':
      return 4;
    default:
      return -1;
    }
  }
  char popLowPrioritized(rychkov::Queue< rychkov::Variant< long long, char > >& expression,
      rychkov::Stack< char >& operators, int referencePriority = 0)
  {
    while (!operators.empty() && (rychkov::getPriority(operators.top()) > referencePriority))
    {
      expression.push(operators.top());
      operators.pop();
    }
    char result = '\0';
    if (!operators.empty() && (rychkov::getPriority(operators.top()) == referencePriority))
    {
      result = operators.top();
      if (operators.top() != '(')
      {
        expression.push(operators.top());
      }
      operators.pop();
    }
    return result;
  }
}

int main(int argc, char** argv)
{
  std::istream* inPtr = &std::cin;
  std::ifstream inFile;
  if (argc == 2)
  {
    inFile.open(argv[1]);
    if (!inFile)
    {
      std::cerr << "failed to open file \"" << argv[1] << "\"\n";
      return 1;
    }
    inPtr = &inFile;
  }
  std::istream& in = *inPtr;

  in >> std::noskipws;
  rychkov::Stack< long long > results;
  while (!in.eof())
  {
    rychkov::Queue< rychkov::Variant< long long, char > > expression;
    rychkov::Stack< char > operators;
    bool isNumber = false;
    long long number = 0;
    char c = 0;
    while (in >> c)
    {
      if (std::isdigit(c))
      {
        isNumber = true;
        try
        {
          number = rychkov::safeMul< long long >(number, 10);
          number = rychkov::safeAdd< long long >(number, c - '0');
        }
        catch (...)
        {
          std::cerr << "input overflow\n";
          return 1;
        }
      }
      else
      {
        if (isNumber)
        {
          expression.push(number);
          isNumber = false;
          number = 0;
        }
        if (c == '\n')
        {
          break;
        }
        if (!std::isspace(c))
        {
          int priority = rychkov::getPriority(c);
          if (priority == -1)
          {
            std::cerr << "found unknown symbol - '" << c << "'\n";
            return 1;
          }
          if (c == '(')
          {
            operators.push(c);
            continue;
          }
          char equalPrioritized = rychkov::popLowPrioritized(expression, operators, priority);
          if ((equalPrioritized != '(') && (c == ')'))
          {
            std::cerr << "wrong parentheses order\n";
            return 1;
          }
          if (c != ')')
          {
            operators.push(c);
          }
        }
      }
    }
    rychkov::popLowPrioritized(expression, operators, 0);
    operators.clear();
    if (expression.empty())
    {
      continue;
    }

    rychkov::Stack< long long > operands;
    if (!rychkov::holds_alternative< long long >(expression.front()))
    {
      std::cerr << "expression does not start with number\n";
      return 1;
    }
    operands.push(rychkov::get< long long >(expression.front()));
    expression.pop();
    for (; !expression.empty(); expression.pop())
    {
      if (rychkov::holds_alternative< char >(expression.front()))
      {
        if (operands.size() < 2)
        {
          std::cerr << "missing operand (number)\n";
          return 1;
        }
        long long rightOperand = operands.top();
        operands.pop();
        try
        {
          operands.top() = rychkov::executeOperation(operands.top(),
                rychkov::get< char >(expression.front()), rightOperand);
        }
        catch (const std::invalid_argument& e)
        {
          std::cerr << e.what() << '\n';
          return 1;
        }
      }
      else
      {
        operands.push(rychkov::get< long long >(expression.front()));
      }
    }
    if (operands.size() != 1)
    {
      std::cerr << "missing operator\n";
      return 1;
    }
    results.push(operands.top());
  }

  char space[2] = "\0";
  while (!results.empty())
  {
    std::cout << space << results.top();
    space[0] = ' ';
    results.pop();
  }
  std::cout << '\n';
}
