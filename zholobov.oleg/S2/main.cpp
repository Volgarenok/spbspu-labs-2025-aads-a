#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

#include <queue.hpp>
#include <stack.hpp>

namespace {
  int operatorPriority(const std::string& op)
  {
    if (op == "+" || op == "-") {
      return 1;
    }
    if (op == "*" || op == "/" || op == "%") {
      return 2;
    }
    return 0;
  }

  constexpr long max_value = std::numeric_limits< long >::max();
  constexpr long min_value = std::numeric_limits< long >::min();

  bool isAdditionOverflow(long lhs, long rhs)
  {
    return (((rhs > 0) && (lhs > (max_value - rhs))) || ((rhs < 0) && (lhs < (min_value - rhs))));
  }

  bool isSubtractionOverflow(long lhs, long rhs)
  {
    return (((rhs > 0) && (lhs < (min_value + rhs))) || ((rhs < 0) && (lhs > (max_value + rhs))));
  }

  bool isMultiplicationOverflow(long lhs, long rhs)
  {
    bool error = false;
    if (lhs > 0) {
      if (rhs > 0) {
        error = (lhs > (max_value / rhs));
      } else {
        error = (rhs < (min_value / lhs));
      }
    } else {
      if (rhs > 0) {
        error = (lhs < (min_value / rhs));
      } else {
        error = ((lhs != 0) && (rhs < (max_value / lhs)));
      }
    }
    return error;
  }

  bool isDivisionOverflow(long lhs, long rhs)
  {
    return ((lhs == min_value) && (rhs == -1));
  }

  bool isModuloOverflow(long lhs, long rhs)
  {
    return ((lhs == min_value) && (rhs == -1));
  }
}

namespace zholobov {

  bool isOperator(const std::string& token)
  {
    return (token == "+") || (token == "-") || (token == "*") || (token == "/") || (token == "%");
  }

  bool strToLong(const std::string& str, long& result)
  {
    char* end;
    result = std::strtol(str.c_str(), &end, 10);
    if (errno == ERANGE) {
      throw std::range_error("Range error");
    }
    return *end == '\0';
  }

  bool isLessPriority(const std::string& op1, const std::string& op2)
  {
    return operatorPriority(op1) < operatorPriority(op2);
  }

  Queue< std::string > infixToPostfix(Queue< std::string > tokens)
  {
    Queue< std::string > result;
    Stack< std::string > op_stack;

    while (!tokens.empty()) {
      std::string token = tokens.front();
      tokens.pop();

      if (token == "(") {
        op_stack.push(std::move(token));
      } else if (token == ")") {
        while (!op_stack.empty() && op_stack.top() != "(") {
          result.push(op_stack.top());
          op_stack.pop();
        }
        if (op_stack.empty()) {
          throw std::runtime_error("Parentheses mismatch");
        }
        op_stack.pop();
      } else if (isOperator(token)) {
        while ((!op_stack.empty()) && (op_stack.top() != "(") && (!isLessPriority(op_stack.top(), token))) {
          result.push(op_stack.top());
          op_stack.pop();
        }
        op_stack.push(std::move(token));
      } else {
        result.push(std::move(token));
      }
    }

    while (!op_stack.empty()) {
      if (op_stack.top() == "(") {
        throw std::runtime_error("Parentheses mismatch");
      }
      result.push(op_stack.top());
      op_stack.pop();
    }

    return result;
  }

  long evaluatePostfixExpression(Queue< std::string > postfix_expr)
  {
    Stack< long > eval_stack;

    while (!postfix_expr.empty()) {
      std::string token = postfix_expr.front();
      postfix_expr.pop();

      long value = 0;
      if (strToLong(token, value)) {
        eval_stack.push(value);
      } else {
        if (eval_stack.empty()) {
          throw std::runtime_error("Not enough operands");
        }
        long rhs = eval_stack.top();
        eval_stack.pop();
        if (eval_stack.empty()) {
          throw std::runtime_error("Not enough operands");
        }
        long lhs = eval_stack.top();
        eval_stack.pop();

        if (token == "+") {
          if (isAdditionOverflow(lhs, rhs)) {
            throw std::runtime_error("Addition overflow");
          }
          eval_stack.push(lhs + rhs);
        } else if (token == "-") {
          if (isSubtractionOverflow(lhs, rhs)) {
            throw std::runtime_error("Subtraction overflow");
          }
          eval_stack.push(lhs - rhs);
        } else if (token == "*") {
          if (isMultiplicationOverflow(lhs, rhs)) {
            throw std::runtime_error("Multiplication overflow");
          }
          eval_stack.push(lhs * rhs);
        } else if (token == "/") {
          if (rhs == 0) {
            throw std::runtime_error("Division by zero");
          }
          if (isDivisionOverflow(lhs, rhs)) {
            throw std::runtime_error("Division overflow");
          }
          eval_stack.push(lhs / rhs);
        } else if (token == "%") {
          if (rhs == 0) {
            throw std::runtime_error("Modulo by zero");
          }
          if (isModuloOverflow(lhs, rhs)) {
            throw std::runtime_error("Modulo overflow");
          }
          long res = lhs % rhs;
          if (res < 0) {
            res += std::abs(rhs);
          }
          eval_stack.push(res);
        } else {
          throw std::runtime_error("Invalid operator");
        }
      }
    }

    if (eval_stack.empty()) {
      throw std::runtime_error("No result");
    }
    long result = eval_stack.top();
    eval_stack.pop();
    if (!eval_stack.empty()) {
      throw std::runtime_error("Too many operands");
    }
    return result;
  }

  Queue< std::string > readExpressions(std::istream& input)
  {
    Queue< std::string > expressions;
    std::string line;
    while (std::getline(input, line)) {
      if (!line.empty()) {
        expressions.push(std::move(line));
      }
    }
    return expressions;
  }

  Queue< std::string > stringToTokens(const std::string& str)
  {
    Queue< std::string > tokens;
    size_t start = 0;
    size_t end = 0;
    while ((start = str.find_first_not_of(' ', end)) != std::string::npos) {
      end = str.find_first_of(' ', start);
      tokens.push(str.substr(start, end - start));
    }
    return tokens;
  }

  zholobov::Stack< long > precessExpressions(zholobov::Queue< std::string > expressions)
  {
    zholobov::Stack< long > results;
    while (!expressions.empty()) {
      std::string expr_str = expressions.front();
      expressions.pop();

      zholobov::Queue< std::string > tokens = zholobov::stringToTokens(expr_str);
      zholobov::Queue< std::string > postfix = zholobov::infixToPostfix(std::move(tokens));
      results.push(zholobov::evaluatePostfixExpression(std::move(postfix)));
    }
    return results;
  }

  void printResults(std::ostream& out, zholobov::Stack< long > results)
  {
    out << results.top();
    results.pop();
    while (!results.empty()) {
      out << " " << results.top();
      results.pop();
    }
  }

}

int main(int argc, char* argv[])
{
  try {
    zholobov::Queue< std::string > expressions;
    if (argc > 1) {
      std::ifstream file(argv[1]);
      if (!file) {
        throw std::runtime_error("Cannot open file");
      }
      expressions = zholobov::readExpressions(file);
    } else {
      expressions = zholobov::readExpressions(std::cin);
    }

    zholobov::Stack< long > results = zholobov::precessExpressions(std::move(expressions));
    if (!results.empty()) {
      zholobov::printResults(std::cout, std::move(results));
    }
    std::cout << "\n";
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
