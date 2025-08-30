#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <hashTable/hashTable.hpp>

namespace maslov
{
  using Dict = HashTable< std::string, int >;
  using Dicts = HashTable< std::string, Dict >;

  void createDictionary(std::istream & in, Dicts & dicts);
  void showDictionary(std::ostream & out, const Dicts & dicts);
  void loadText(std::istream & in, Dicts & dicts);
  void unionDictionary(std::istream & in, Dicts & dicts);
  void intersectDictionary(std::istream & in, Dicts & dicts);
  void copyDictionary(std::istream & in, Dicts & dicts);
  void addWord(std::istream & in, Dicts & dicts);
  void printSize(std::istream & in, std::ostream & out, const Dicts & dicts);
  void cleanWord(std::istream & in, Dicts & dicts);
  void cleanDictionary(std::istream & in, Dicts & dicts);
  void printTopRare(std::istream & in, std::ostream & out, const Dicts & dicts, const std::string & order);
  void printFrequency(std::istream & in, std::ostream & out, const Dicts & dicts);
  void createWordRange(std::istream & in, Dicts & dicts);
  void saveDictionaries(std::istream & in, const Dicts & dicts);
  void loadFile(const std::string & filename, Dicts & dicts);
  void loadFileCommand(std::istream & file, Dicts & dicts);
  void printHelp(std::ostream & out);
}

#endif
