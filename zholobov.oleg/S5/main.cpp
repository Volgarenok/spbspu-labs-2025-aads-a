#include <unistd.h>
#include <fstream>
#include <iostream>

#include <tree/tree.hpp>
#include "key_sum.hpp"

namespace {

  using TreeMap = zholobov::Tree< int, std::string >;

  TreeMap readTree(std::istream& input)
  {
    TreeMap result;
    TreeMap::key_type key{};
    TreeMap::mapped_type value{};
    while (input >> key >> value) {
      result.emplace(key, value);
    }
    return result;
  }

}

int main(int argc, char* argv[])
{
  if (argc != 3) {
    std::cerr << "Error in parameters. Run as ./lab [ascending|descending|breadth] filename\n";
    return 1;
  }
  std::string traversal{argv[1]};
  std::ifstream inputFile(argv[2]);
  if (!inputFile) {
    std::cerr << "Error opening file\n";
    return 1;
  }
  const TreeMap map = readTree(inputFile);
  if (!inputFile.eof()) {
    std::cerr << "Bad file content\n";
    return 1;
  }
  if (map.empty()) {
    std::cout << "<EMPTY>\n";
    return 0;
  }

  try {
    zholobov::KeySum< TreeMap::key_type > keySum;
    if (traversal == "ascending") {
      keySum = map.traverse_lnr(keySum);
    } else if (traversal == "descending") {
      keySum = map.traverse_rnl(keySum);
    } else if (traversal == "breadth") {
      keySum = map.traverse_breadth(keySum);
    } else {
      std::cerr << "Invalid 1st parameter. Use one of [ascending|descending|breadth]\n";
      return 1;
    }
    std::cout << keySum.sum_ << keySum.values_ << "\n";
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
  return 0;
}
