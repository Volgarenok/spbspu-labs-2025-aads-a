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
  enum Operator: char
  {
    plus = '+',
    minus = '-',
    multiplies = '*',
    divides = '/',
    modulus = '%',
    openinigParenthesis = '(',
    closingParenthesis = ')',
  };
  namespace details
  {
    int getPriority(Operator oper)
    {
      switch (oper)
      {
      case openinigParenthesis:
      case closingParenthesis:
        return 1;
      case modulus:
        return 2;
      case plus:
      case minus:
        return 3;
      case multiplies:
      case divides:
        return 4;
      default:
        return -1;
      }
    }
  }
  bool lessPriority(Operator lhs, Operator rhs)
  {
    return details::getPriority(lhs) < details::getPriority(rhs);
  }
  bool equalPriority(Operator lhs, Operator rhs)
  {
    return details::getPriority(lhs) == details::getPriority(rhs);
  }
  bool isOperator(char c)
  {
    return details::getPriority(static_cast< Operator >(c)) != -1;
  }
  Operator popLowPrioritized(Queue< Variant< long long, Operator > >& expression,
        Stack< Operator >& operators, Operator referenceOperator = static_cast< Operator >('\0'))
  {
    while (!operators.empty() && lessPriority(referenceOperator, operators.top()))
    {
      expression.push(operators.top());
      operators.pop();
    }
    Operator result = static_cast< Operator >('\0');
    if (!operators.empty() && equalPriority(operators.top(), referenceOperator))
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
  using rychkov::Stack;
  using rychkov::Queue;
  using rychkov::Variant;
  using rychkov::Operator;

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
  Stack< long long > results;
  while (!in.eof())
  {
    Queue< Variant< long long, Operator > > expression;
    Stack< Operator > operators;
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
          if (!rychkov::isOperator(c))
          {
            std::cerr << "found unknown symbol - '" << c << "'\n";
            return 1;
          }
          Operator oper = static_cast< Operator >(c);
          if (c == '(')
          {
            operators.push(oper);
            continue;
          }
          Operator equalPrioritized = rychkov::popLowPrioritized(expression, operators, oper);
          if ((equalPrioritized != '(') && (c == ')'))
          {
            std::cerr << "wrong parentheses order\n";
            return 1;
          }
          if (c != ')')
          {
            operators.push(oper);
          }
        }
      }
    }
    rychkov::popLowPrioritized(expression, operators);
    operators.clear();
    if (expression.empty())
    {
      continue;
    }

    Stack< long long > operands;
    if (!rychkov::holds_alternative< long long >(expression.front()))
    {
      std::cerr << "expression does not start with number\n";
      return 1;
    }
    operands.push(rychkov::get< long long >(expression.front()));
    expression.pop();
    for (; !expression.empty(); expression.pop())
    {
      if (rychkov::holds_alternative< Operator >(expression.front()))
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
                rychkov::get< Operator >(expression.front()), rightOperand);
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
