#include <iostream>
#include <fstream>
#include <string>

#include <queue.hpp>
#include <stack.hpp>
#include "expr.hpp"

int main(int argc, char* argv[])
{
  std::ifstream file;
  if (argc > 1) {
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "Error with file!!!" << '\n';
      return 1;
    }
  }
  std::istream& in = (argc > 1) ? file : std::cin;

  dribas::Stack< long long int > result;
  std::string str;

  while(std::getline(in, str)) {
    try {
      dribas::Queue< std::string > infix = dribas::inputInfix(str);
      if (infix.empty()) {
        continue;
      }
      dribas::Queue< std::string > postfix = dribas::infixToPostfix(infix);
      long long int resultStr = dribas::evaluatePostfix(postfix);
      result.push(resultStr);
    } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }

  if (!result.empty()) {
    std::cout << result.top();
    result.pop();
  }

  while(!result.empty()) {
    std::cout << ' ' << result.top();
    result.pop();
  }
  std::cout << '\n';

  return 0;

}
