#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <forward-list.hpp>
#include "io-utils.hpp"
#include "postfix-expression.hpp"

int main(int argc, char** argv)
{
  using namespace kizhin;
  try {
    if (argc > 2) {
      std::cerr << "Usage: " << argv[0] << " [filename]\n";
      return 1;
    }
    std::ifstream fin;
    if (argc == 2) {
      std::string filePath = argv[1];
      fin.open(filePath);
      if (!fin) {
        throw std::logic_error("Failed to open file: " + filePath);
      }
    }
    std::istream& in = fin.is_open() ? fin : std::cin;
    std::string current;
    ForwardList< PostfixExpression::number_type > results;
    while (std::getline(in, current)) {
      if (!current.empty()) {
        std::stringstream stream(current);
        results.pushFront(inputPostfixExpression(stream).evaluate());
      }
    }
    if (!results.empty()) {
      std::cout << results.front();
      results.popFront();
    }
    while (!results.empty()) {
      std::cout << ' ' << results.front();
      results.popFront();
    }
    std::cout << '\n';
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
}
