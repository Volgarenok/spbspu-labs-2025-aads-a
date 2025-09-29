#include <iostream>
#include <string>
#include <stdexcept>
#include "binary_search_tree.hpp"
#include "files.hpp"
#include "file_utils.hpp"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " [ascending|descending|breadth] filename" << std::endl;
    return 1;
  }
  std::string traversal_type = argv[1];
  std::string filename = argv[2];
  if (traversal_type != "ascending" && traversal_type != "descending" && traversal_type != "breadth") {
    std::cerr << "Invalid traversal type. Use: ascending, descending, or breadth" << std::endl;
    return 1;
  }
  try {
    auto tree = readTreeFromFile(filename);
    if (tree.empty()) {
      std::cout << "<EMPTY>" << std::endl;
      return 0;
    }
    KeySumm summator;
    if (traversal_type == "ascending") {
      tree.traverse_lnr(summator);
    } else if (traversal_type == "descending") {
      tree.traverse_rnl(summator);
    } else if (traversal_type == "breadth") {
      tree.traverse_breadth(summator);
    }
    std::cout << summator.result_ << " " << summator.values_ << std::endl;
  } catch (const std::overflow_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  } catch (const std::underflow_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
