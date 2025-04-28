#include <iostream>
#include <fstream>
#include <string>
#include "expression_transform.hpp"

int main(int argc, char *argv[])
{
  std::string str;
  evstyunichev::Queue< long long > result{};
  std::ifstream fin{};
  if (argc == 2)
  {
    fin = std::ifstream{ argv[1] };
  }
  while (((argc == 2) ? std::getline(fin, str, '\n') : std::getline(std::cin, str, '\n')))
  {
    try
    {
      result.push(evstyunichev::result(str));
    }
    catch (const std::exception &e)
    {
      std::cerr << e.what() << '\n';
      return 2;
    }
  }
  if (result.empty())
  {
    std::cerr << "!empty\n";
    return 1;
  }
  std::cout << result.front();
  while (result.size() > 1)
  {
    result.pop();
    std::cout << ' ' << result.front();
  }
  std::cout << '\n';
  return 0;
}
