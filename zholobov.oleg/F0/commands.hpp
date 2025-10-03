#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <exception>
#include <iosfwd>
#include <vector>

#include <array.hpp>
#include <list/CircularFwdList.hpp>
#include <tree/tree.hpp>

namespace zholobov {

  using Word = std::string;
  using Words = CircularFwdList< Word >;
  using Dictionary = Tree< Word, Words >;
  using Dictionaries = Tree< std::string, Dictionary >;

  struct InvalidParams: public std::exception {};

  void printHelp(std::ostream& out);

  void cmdDictCreate(Dictionaries& dictionaries, const Array< std::string >& args);
  void cmdDictRemove(Dictionaries& dictionaries, const Array< std::string >& args);
  void cmdDictImport(Dictionaries& dictionaries, const Array< std::string >& args);
  void cmdDictExport(Dictionaries& dictionaries, const Array< std::string >& args);
  void cmdDictCount(Dictionaries& dictionaries, const Array< std::string >& args);
  void cmdDictList(Dictionaries& dictionaries, const Array< std::string >& args);
  void cmdDictPrintWords(Dictionaries& dictionaries, const Array< std::string >& args);
  void cmdDictPrintTranslations(Dictionaries& dictionaries, const Array< std::string >& args);
  void cmdDictClear(Dictionaries& dictionaries, const Array< std::string >& args);
  void cmdAddWord(Dictionaries& dictionaries, const Array< std::string >& args);
  void cmdRemoveWord(Dictionaries& dictionaries, const Array< std::string >& args);
  void cmdCountWords(Dictionaries& dictionaries, const Array< std::string >& args);
  void cmdAddTranslation(Dictionaries& dictionaries, const Array< std::string >& args);
  void cmdRemoveTranslation(Dictionaries& dictionaries, const Array< std::string >& args);
  void cmdChangeWord(Dictionaries& dictionaries, const Array< std::string >& args);
  void cmdTranslateWord(Dictionaries& dictionaries, const Array< std::string >& args);
  void cmdUnion(Dictionaries& dictionaries, const Array< std::string >& args);
  void cmdIntersect(Dictionaries& dictionaries, const Array< std::string >& args);
  void cmdRare(Dictionaries& dictionaries, const Array< std::string >& args);

}

#endif
