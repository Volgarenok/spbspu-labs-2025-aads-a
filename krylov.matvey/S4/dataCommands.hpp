#ifndef DATACOMMANDS_HPP
#define DATACOMMANDS_HPP

#include <string>
#include <binaryTree.hpp>

namespace krylov
{
  using Dictionary = BiTree< std::string, BiTree< int, std::string, std::less< int > >, std::less< std::string > >;
  void inputFile(const std::string& filename, Dictionary& dicts);
  void printComm(std::istream& in, std::ostream& out, const Dictionary& dicts);
  void complementComm(std::istream& in, Dictionary& dicts);
  void intersectComm(std::istream& in, Dictionary& dicts);
  void unionComm(std::istream& in, Dictionary& dicts);
}

#endif
