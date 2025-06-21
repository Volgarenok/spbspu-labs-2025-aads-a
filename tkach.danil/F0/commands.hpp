#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <AVLtree.hpp>
#include <list.hpp>

namespace tkach
{
  using tree_of_words = AvlTree< std::string, List< std::string > >;
  using tree_of_dict = AvlTree< std::string, tree_of_words >;
  void import(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree);
  void doExportOverwrite(std::istream& in, const AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree);
  void doExportInEnd(std::istream& in, const AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree);
  void addWord(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree);
  void printCommonTranslations(std::istream& in, std::ostream& out, const tree_of_dict& avltree);
  void removeWord(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree);
  void clear(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree);
  void printCount(std::istream& in, std::ostream& out, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree);
  void mergeWords(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree);
  void mergeNumberDicts(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree);
  void doCommonPartDicts(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree);
  void copyTranslations(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree);
  void printTranslations(std::istream& in, std::ostream& out, const tree_of_dict& avltree);
  void printEngWordsWithTraslation(std::istream& in, std::ostream& out, const tree_of_dict& avltree);
  void addTranslation(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree);
  void removeTranslation(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree);
  void substructDicts(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree);
  void printAll(std::ostream& out, const AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree);
  void help(std::ostream& out);
}

#endif
