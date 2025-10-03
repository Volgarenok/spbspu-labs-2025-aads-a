#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <hashTable.hpp>
#include <dynamicArray.hpp>

namespace mozhegova
{
  using WordPos = std::pair< size_t, size_t >;
  using Xrefs = DynamicArray< WordPos >;
  using Text = HashTable< std::string, Xrefs >;
  using Texts = HashTable< std::string, Text >;

  void generateLinks(std::istream & in, Texts & texts);
  void removeLinks(std::istream & in, Texts & texts);
  void printLinks(std::istream & in, std::ostream & out, const Texts & texts);
  void printText(std::istream & in, std::ostream & out, const Texts & texts);
  void printTextInFile(std::istream & in, const Texts & texts);
  void mergeTexts(std::istream & in, Texts & texts);
  void insertText(std::istream & in, Texts & texts);
  void removeLines(std::istream & in, Texts & texts);
  void moveText(std::istream & in, Texts & texts);
  void sideMergeTexts(std::istream & in, Texts & texts);
  void splitTexts(std::istream & in, Texts & texts);
  void invertLines(std::istream & in, Texts & texts);
  void invertWords(std::istream & in, Texts & texts);
  void replaceWord(std::istream & in, Texts & texts);
  void save(std::istream & in, const Texts & texts);
  void loadFileCmd(std::istream & in, Texts & texts);
  void loadFile(const std::string & fileName, Texts & texts);
  void printHelp(std::ostream & out);
}

#endif
