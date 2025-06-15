#include <fstream>
#include <iostream>
#include <cstring>
#include <tree/definition.hpp>
#include "keys_values_plus.hpp"

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
  try {
    if (std::strcmp(argv[1], "ascending") == 0) {
      plus = data.traverse_lnr(plus);
    } else if (std::strcmp(argv[1], "descending") == 0) {
      plus = data.traverse_rnl(plus);
    } else if (std::strcmp(argv[1], "breadth") == 0) {
      plus = data.traverse_breadth(plus);
    } else {
      std::cerr << "<INVALID COMMAND>\n";
      return 1;
    }
  } catch (const std::logic_error&) {
    std::cout << "<EMPTY>\n";
    return 0;
  } catch (...) {
    std::cerr << "<OVERFLOW ERROR>\n";
    return 1;
  }
  std::cout << plus.keys_sum << plus.values << '\n';
}
