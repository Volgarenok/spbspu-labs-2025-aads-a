#include "expr.hpp"
#include <string>
#include <limits>
#include <cmath>
#include <map>
#include "stack.hpp"

namespace
{
  bool checkMultiplyOverFLow(long long int a, long long int b)
  {
    if (a == 0 || b == 0) {
      return false;
    }
    if (a > 0 && b > 0) {
      return a >= std::numeric_limits< long long int >::max() / b;
    } else if (a < 0 && b < 0) {
      return a <= std::numeric_limits< long long int >::min() / b;
    } else if (a < 0 && b > 0) {
      return a <= std::numeric_limits< long long int >::min() / b;
    }
    return b <= std::numeric_limits< long long int >::min() / a;
  }

  int countDigits(long long int num)
  {
    if (num == 0) {
      return 1;
    }
    int count = 0;
    num = std::abs(num);
    while (num != 0) {
      num /= 10;
      count++;
    }
    return count;
  }

  bool checkSubOverflow(long long int a, long long int b)
  {
    if (b < 0) {
      if (a > std::numeric_limits< long long int >::max() + b) {
        return true;
      };
    } else {
      if (a < std::numeric_limits< long long int >::min() + b) {
        return true;
      }
    }
    return false;
  }

  bool checkSumOverflow(long long int a, long long int b)
  {
    if (b > 0) {
      if (a > std::numeric_limits< long long int >::max() - b) {
        return true;
      }
    } else {
      if (a < std::numeric_limits< long long int >::min() - b) {
        return true;
      }
    }
    return false;
  }

  bool checkDivOverflow(long long int a, long long b)
  {
    if (a == std::numeric_limits< long long int >::min() && b == -1) {
      return true;
    }
    return false;
  }

  bool checkConcatinationOverflow(long long int a, long long int b)
  {
    int dif = countDigits(b);
    bool result = checkMultiplyOverFLow(a, std::pow(10, dif));
    if (!result) {
      long long int preresult = a * std::pow(10, dif);
      result = checkSumOverflow(preresult, b);
    }
    return result;
  }

  long long int concatination(long long int a, long long b)
  {
    long long int result = a * std::pow(10, countDigits(b));
    result += b;
    return result;
  }

  bool isOperator(const std::string& in)
  {
    const std::string operations[6] = { "+", "-", "*", "/", "%", "|" };
    for (std::string s: operations) {
      if (in == s) {
        return true;
      }
    }
    return false;
  }

  bool getPrecedence(const std::string& lhs, const std::string& rhs)
  {
    std::map< std::string, int > precedence;
    precedence["+"] = 1;
    precedence["-"] = 1;
    precedence["*"] = 2;
    precedence["/"] = 2;
    precedence["%"] = 3;
    precedence["|"] = 4;
    if (precedence.find(lhs) == precedence.end() || precedence.find(rhs) == precedence.end()) {
      return false;
    }
    return precedence[lhs] >= precedence[rhs];
  }
}
long long dribas::evaluatePostfix(Queue< std::string >& postfixQueue)
{
  Stack< long long > operandStack;

  while (!postfixQueue.empty()) {
    std::string token = postfixQueue.back();
    postfixQueue.pop();

    if (isOperator(token)) {
      if (operandStack.size() < 2) {
        throw std::runtime_error("Invalid postfix expression");
      }
      long long b = operandStack.top();
      operandStack.pop();
      long long a = operandStack.top();
      operandStack.pop();
      long long result = 0;
      if (token == "|") {
        if (checkConcatinationOverflow(a, b)) {
          throw std::overflow_error("Concatenation overflow");
        }
        result = concatination(a, b);
      } else if (token == "%") {
        if (checkDivOverflow(a, b)) {
          throw std::overflow_error("Division overflow");
        }
        if (a >= 0) {
          result = a % b;
        } else {
          result = (b - std::abs(a % b)) % b;
        }
      } else if (token == "*") {
        if (checkMultiplyOverFLow(a, b)) {
          throw std::overflow_error("Multiplication overflow");
        }
        result = a * b;
      } else if (token == "/") {
        if (b == 0) {
          throw std::runtime_error("Division by zero");
        }
        if (checkDivOverflow(a, b)) {
          throw std::overflow_error("Division overflow");
        }
        result = a / b;
      } else if (token == "+") {
        if (checkSumOverflow(a, b)) {
          throw std::overflow_error("Addition overflow");
        }
        result = a + b;
      } else if (token == "-") {
        if (checkSubOverflow(a, b)) {
          throw std::overflow_error("Subtraction overflow");
        }
        result = a - b;
      }
      operandStack.push(result);
    } else {
      long long num = std::stoll(token);
      operandStack.push(num);
    }
  }

  if (operandStack.size() != 1) {
    throw std::runtime_error("Invalid postfix expression");
  }
  return operandStack.top();
}

dribas::Queue< std::string > dribas::infixToPostfix(Queue< std::string >& infixQueue)
{
  Stack< std::string > operatorStack;
  Queue< std::string > postfixQueue;
  bool hasError = false;

  while (!infixQueue.empty() && !hasError) {
    std::string token = infixQueue.back();
    infixQueue.pop();

    if (token == "(") {
      operatorStack.push(token);
    } else if (token == ")") {
      while (!operatorStack.empty() && operatorStack.top() != "(") {
        postfixQueue.push(operatorStack.top());
        operatorStack.pop();
      }
      if (operatorStack.empty()) {
        hasError = true;
      } else {
        operatorStack.pop();
      }
    } else if (isOperator(token)) {
      while (!operatorStack.empty() && operatorStack.top() != "(" && getPrecedence(operatorStack.top(), token)) {
        postfixQueue.push(operatorStack.top());
        operatorStack.pop();
      }
      operatorStack.push(token);
    } else {
      postfixQueue.push(token);
    }
  }

  while (!operatorStack.empty() && !hasError) {
    if (operatorStack.top() == "(") {
      hasError = true;
    } else {
      postfixQueue.push(operatorStack.top());
      operatorStack.pop();
    }
  }
  if (hasError) {
    return Queue< std::string >();
  }
  return postfixQueue;
}

dribas::Queue< std::string > dribas::inputInfix(const std::string& input)
{
  Queue< std::string > queue;

  size_t start = 0;
  size_t end = input.find(' ');
  while (end != std::string::npos) {
    if (start != end) {
      queue.push(input.substr(start, end - start));
    }
    start = end + 1;
    end = input.find(' ', start);
  }
  if (start < input.length()) {
    queue.push(input.substr(start));
  }
  return queue;
}
