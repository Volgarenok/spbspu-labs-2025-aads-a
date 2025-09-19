#include "commands.hpp"
#include <string>
#include <iostream>

void lebedev::printCMD(std::ostream& out, dictOfDicts dict)
{
  std::string name = "";
  std::cin >> name;
  const std::map< int, std::string >& current = dict.at(name);
  if (current.empty())
  {
    std::cout << "<EMPTY>";
    return;
  }
  out << name;
}
