#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <functional>
#include <tree/tree-2-3.hpp>
#include <list/list.hpp>

namespace alymova
{
  using WordSet = List< std::string >;
  using Dictionary = TwoThreeTree< std::string, WordSet, std::less< std::string > >;
  using DictSet = TwoThreeTree< std::string, Dictionary, std::less< std::string > >;

  void create(std::istream& in, std::ostream& out, DictSet& set);
  void dicts(std::ostream& out, const DictSet& set);
  void size(std::istream& in, std::ostream& out, const DictSet& set);
  void removeDict(std::istream& in, std::ostream& out, DictSet& set);
  void addWord(std::istream& in, std::ostream& out, DictSet& set);
  void fixWord(std::istream& in, std::ostream& out, DictSet& set);
  void findWord(std::istream& in, std::ostream& out, const DictSet& set);
  void containSubword(std::istream& in, std::ostream& out, const DictSet& set);
  void removeWord(std::istream& in, std::ostream& out, DictSet& set);
  void addTranslate(std::istream& in, std::ostream& out, DictSet& set);
  void findEnglishEquivalent(std::istream& in, std::ostream& out, const DictSet& set);
  void removeTranslate(std::istream& in, std::ostream& out, DictSet& set);
  void printContent(std::istream& in, std::ostream& out, const DictSet& set);
  void translate(std::istream& in, std::ostream& out, const DictSet& set);
  void unionDicts(std::istream& in, std::ostream& out, DictSet& set);
  void intersectDicts(std::istream& in, std::ostream& out, DictSet& set);
  void printDayWord(std::istream& in, std::ostream& out, const DictSet& set);

  DictSet readDictionaryFile(std::istream& in);
  void saveDictionaryFile(std::ostream& out, const DictSet& set);

  void printHelp(std::ostream& out);
}

#endif
