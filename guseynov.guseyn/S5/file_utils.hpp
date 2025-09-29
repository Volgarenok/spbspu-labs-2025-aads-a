#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP

#include <fstream>
#include <string>
#include <stdexcept>
#include "binary_search_tree.hpp"

guseynov::BinarySearchTree< int, std::string > readTreeFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file: " + filename);
  }
  guseynov::BinarySearchTree< int, std::string > tree;
  int key;
  std::string value;
  while (file >> key >> value) {
    tree.push(key, value);
  }
  if (file.bad()) {
    throw std::runtime_error("Error reading from file: " + filename);
  }
  return tree;
}

#endif
