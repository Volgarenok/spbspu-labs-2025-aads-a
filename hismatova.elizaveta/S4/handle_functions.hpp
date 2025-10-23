#include <string>
#include <unordered_map>
#include "BinarySeachTree.hpp"
namespace hismatova
{
  void handle_print(const std::unordered_map< std::string, BinarySearchTree< int, std::string > >& datasets, const std::string& datasetName);
  void handle_complement(std::unordered_map< std::string, BinarySearchTree< int, std::string > >& datasets,
    const std::string& result,
    const std::string& a,
    const std::string& b);
  void handle_intersect(std::unordered_map< std::string, BinarySearchTree< int, std::string > >& datasets,
    const std::string& result,
    const std::string& a,
    const std::string& b);
  void handle_union(std::unordered_map< std::string, BinarySearchTree< int, std::string > >& datasets,
    const std::string& result,
    const std::string& a,
    const std::string& b);
}
