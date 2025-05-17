#include <iostream>
#include <iomanip>
#include <fstream>
#include "countFunctions.h"

int main(int argc, char* argv[])
{
  std::stack<asafov::num_t> result;
  std::istream* is = nullptr;
  std::ifstream fin;
  if (argc == 1)
  {
    is = &std::cin;
  }
  else if (argc == 2)
  {
    fin.open(argv[1]);
    is = &fin;
  }
  else
  {
    std::cerr << "invalid arguments\n";
    return 1;
  }

  while (!is->eof())
  {
    int error = asafov::evaluateExpressionFromString(*is, result);
    if (error == 1) continue;
    else if (error == 2) return 1;
  }
  if (result.empty())
  {
    std::cout << '\n';
    return 0;
  }
  while (result.size() > 1)
  {
    std::cout << std::fixed << std::setprecision(0) << result.top() << ' ';
    result.pop();
  }
  std::cout << std::fixed << std::setprecision(0) << result.top() << '\n';
  result.pop();
  return 0;
}
