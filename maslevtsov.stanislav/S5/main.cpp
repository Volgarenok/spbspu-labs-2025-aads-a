#include <fstream>
#include <iostream>
#include "functional"
#include <tree/definition.hpp>
#include "keys_values_plus.hpp"
#include "traverse_commands.hpp"

int main(int argc, char** argv)
{
  using namespace maslevtsov;

  if (argc != 3) {
    std::cerr << "<INVALID PARAMETERS NUMBER>\n";
    return 1;
  }
  std::ifstream fin(argv[2]);
  if (!fin) {
    std::cerr << "<INVALID DATA FILE>\n";
    return 1;
  }
  Tree< int, std::string > data;
  int key = 0;
  std::string value = "";
  while (fin && fin >> key >> value) {
    data[key] = value;
  }
  if (!fin.eof()) {
    std::cerr << "<INVALID INPUT>\n";
    return 1;
  }
  std::string result = "";
  KeysValuesPlus plus;
  maslevtsov::Tree< std::string, std::function< void() > > commands;
  commands["ascending"] = std::bind(traverse_ascend< KeysValuesPlus& >, std::ref(data), std::ref(plus));
  commands["descending"] = std::bind(traverse_descend< KeysValuesPlus& >, std::ref(data), std::ref(plus));
  commands["breadth"] = std::bind(traverse_breadth< KeysValuesPlus& >, std::ref(data), std::ref(plus));
  try {
    commands.at(std::string(argv[1]))();
  } catch (const std::out_of_range&) {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  } catch (const std::logic_error&) {
    std::cout << "<EMPTY>\n";
    return 0;
  } catch (...) {
    std::cerr << "<OVERFLOW ERROR>\n";
    return 1;
  }
  std::cout << plus.keys_sum << plus.values << '\n';
}
