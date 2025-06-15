#include <fstream>
#include <iostream>
#include <string>

#include <Queue.hpp>
#include <Stack.hpp>
#include "parse_expr.hpp"

int main(int argc, char* argv[])
{
  using namespace gavrilova;

  std::ifstream inputFile;
  if (argc > 1) {
    inputFile.open(argv[1]);
    if (!inputFile.is_open()) {
      std::cerr << "Error! Invalid opening file!" << "\n";
      return 1;
    }
  }
  std::istream& in = (argc > 1) ? inputFile : std::cin;
  std::string line;
  Stack< long long > results;
  while (std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }
    try {
      Queue< std::string > infix_q = split(line, ' ');
      Queue< std::string > postfix_q = infix_to_postfix(infix_q);
      results.push(calculate(postfix_q));
    } catch (const std::exception& e) {
      std::cerr << e.what() << "\n";
      return 1;
    }
  }

  if (results.empty()) {
    std::cout << "\n";
    return 0;
  }

  std::cout << results.top();
  results.pop();
  while (!results.empty()) {
    std::cout << " ";
    std::cout << results.top();
    results.pop();
  }
  std::cout << "\n";
}
