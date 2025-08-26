#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include "dict-input-output.hpp"

namespace
{
  using namespace alymova;

  List< std::string >::ListConstIterator findTranslate(const WordSet& list, const std::string& word)
  {
    auto it = list.begin();
    for (; it != list.end(); it++)
    {
      if (*it == word)
      {
        return it;
      }
    }
    return it;
  }
  void unionLists(List< std::string >& list, const List< std::string >& unioned)
  {
    List< std::string > copy(unioned);
    list.sort();
    copy.sort();
    list.merge(copy);
    list.unique();
  }
  void intersectLists(List< std::string >& list, const List< std::string >& intersected)
  {
    for (auto it = list.begin(); it != list.end();)
    {
      auto it1 = findTranslate(intersected, *it);
      if (it1 == intersected.cend())
      {
        it = list.erase(it);
      }
      else
      {
        it++;
      }
    }
  }
}

void alymova::create(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string name;
  in >> name;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  if (set.find(name) != set.end())
  {
    out << "<ALREADY CREATED>";
    return;
  }
  Dictionary dict;
  set.emplace(name, dict);
  out << "<SUCCESSFULLY CREATED>";
}

void alymova::dicts(std::ostream& out, const DictSet& set)
{
  if (set.empty())
  {
    return;
  }
  auto it = set.begin();
  for (; it != --set.end(); it++)
  {
    out << it->first << '\n';
  }
  out << it->first;
}

void alymova::size(std::istream& in, std::ostream& out, const DictSet& set)
{
  std::string name;
  in >> name;
  if (!in)
  {
    throw std::logic_error("<INVALID ERROR>");
  }
  const Dictionary& dict = set.at(name);
  if (dict.size() == 0)
  {
    out << "<EMPTY>";
    return;
  }
  out << dict.size();
}

void alymova::removeDict(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string name;
  in >> name;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  set.at(name);
  set.erase(name);
  out << "<SUCCESSFULLY REMOVED>";
}

void alymova::addWord(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string name, word, translate;
  in >> name >> word >> translate;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Dictionary& dict = set.at(name);
  auto it_word = dict.find(word);
  if (it_word == dict.end())
  {
    WordSet translates{translate};
    dict.emplace(word, translates);
    out << "<WORD AND TRANSLATE WERE ADDED>";
    return;
  }
  WordSet& translates = it_word->second;
  if (findTranslate(translates, translate) == translates.cend())
  {
    translates.push_back(translate);
    out << "<TRANSLATE WAS ADDED>";
    return;
  }
  out << "<WORD AND TRANSLATE ALREADY WERE ADDED>";
}

void alymova::fixWord(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string name, word, new_word;
  in >> name >> word >> new_word;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Dictionary& dict = set.at(name);
  dict.at(word);
  dict.find(word)->first = new_word;
  out << "<SUCCESSFULLY FIXED>";
}

void alymova::findWord(std::istream& in, std::ostream& out, const DictSet& set)
{
  std::string name, word;
  in >> name >> word;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  const Dictionary& dict = set.at(name);
  out << dict.at(word);
}

void alymova::containSubword(std::istream& in, std::ostream& out, const DictSet& set)
{
  std::string name, subword;
  in >> name >> subword;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  const Dictionary& dict = set.at(name);
  WordSet suitable;
  for (auto it = dict.begin(); it != dict.end(); it++)
  {
    if (it->first.find(subword) != std::string::npos)
    {
      suitable.push_back(it->first);
    }
  }
  if (suitable.empty())
  {
    out << "<NOT FOUND>";
  }
  out << suitable;
}

void alymova::removeWord(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string name, word;
  in >> name >> word;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Dictionary& dict = set.at(name);
  dict.at(word);
  dict.erase(word);
  out << "<SUCCESSFULLY REMOVED>";
}

void alymova::addTranslate(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string name, word, translate;
  in >> name >> word >> translate;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Dictionary& dict = set.at(name);
  WordSet& translates = dict.at(word);
  if (findTranslate(translates, translate) != translates.cend())
  {
    out << "<TRANSLATE WAS ALREADY ADDED>";
    return;
  }
  translates.push_back(translate);
  out << "<TRANSLATE WAS ADDED>";
}

void alymova::findEnglishEquivalent(std::istream& in, std::ostream& out, const DictSet& set)
{
  std::string name;
  WordSet translates;
  in >> name >> translates;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  const Dictionary& dict = set.at(name);
  WordSet equivalents;
  for (auto it = dict.begin(); it != dict.end(); it++)
  {
    WordSet current_translates = it->second;
    intersectLists(current_translates, translates);
    if (!current_translates.empty())
    {
      equivalents.push_back(it->first);
    }
  }
  if (equivalents.empty())
  {
    out << "<NOT FOUND>";
    return;
  }
  out << equivalents;
}

void alymova::removeTranslate(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string name, word, translate;
  in >> name >> word >> translate;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Dictionary& dict = set.at(name);
  WordSet& translates = dict.at(word);
  auto it_translate = findTranslate(translates, translate);
  if (it_translate == translates.cend())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  translates.erase(it_translate);
  out << "<SUCCESSFULLY REMOVED>";
}

void alymova::printContent(std::istream& in, std::ostream& out, const DictSet& set)
{
  std::string name;
  in >> name;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  const Dictionary& dict = set.at(name);
  if (dict.empty())
  {
    return;
  }
  auto it = dict.begin();
  char first_letter = it->first[0];
  out << first_letter << ' ' << (it++)->first;
  for (; it != dict.end(); it++)
  {
    if (it->first[0] == first_letter)
    {
      out << ' ' << it->first;
    }
    else
    {
      first_letter = it->first[0];
      out << '\n' << first_letter << ' ' << it->first;
    }
  }
}

void alymova::translate(std::istream& in, std::ostream& out, const DictSet& set)
{
  WordSet names;
  std::string word;
  in >> names >> word;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  List< Dictionary > dicts;
  for (auto it = names.begin(); it != names.end(); it++)
  {
    dicts.push_back(set.at(*it));
  }
  WordSet translates;
  for (auto it = dicts.begin(); it != dicts.end(); it++)
  {
    auto it_word = it->find(word);
    if (it_word != it->end())
    {
      translates.insert(translates.end(), it_word->second.begin(), it_word->second.end());
    }
  }
  if (translates.empty())
  {
    out << "<NOT FOUND>";
    return;
  }
  translates.sort();
  translates.unique();
  out << translates;
}

void alymova::unionDicts(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string newname, name1, name2;
  in >> newname >> name1 >> name2;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Dictionary dict1 = set.at(name1);
  Dictionary dict2 = set.at(name2);
  for (auto it = dict2.begin(); it != dict2.end(); it++)
  {
    auto it1 = dict1.find(it->first);
    if (it1 == dict1.end())
    {
      dict1.insert({it->first, it->second});
    }
    else
    {
      unionLists(it1->second, it->second);
    }
  }
  set[newname] = dict1;
  out << "<SUCCESSFULLY UNIONED>";
}

void alymova::intersectDicts(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string newname, name1, name2;
  in >> newname >> name1 >> name2;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Dictionary dict1 = set.at(name1);
  Dictionary dict2 = set.at(name2);
  for (auto it = dict2.begin(); it != dict2.end();)
  {
    auto it1 = dict1.find(it->first);
    if (it1 == dict1.end())
    {
      it = dict2.erase(it);
    }
    else
    {
      intersectLists(it->second, it1->second);
      it++;
    }
  }
  set[newname] = dict2;
  out << "<SUCCESSFULLY INTERSECTED>";
}

void alymova::printDayWord(std::istream& in, std::ostream& out, const DictSet& set)
{
  std::string name;
  in >> name;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  const Dictionary& dict = set.at(name);
  if (dict.empty())
  {
    out << "Sorry, today is a bad day";
    return;
  }
  std::srand(std::time(nullptr));
  int random_num = std::rand() % dict.size();
  auto it_word = dict.begin();
  for (int i = 0; i < random_num; i++, it_word++);
  out << "Have a good day with word:\n";
  out << it_word->first << ' ' << it_word->second;
}

alymova::DictSet alymova::readDictionaryFile(std::istream& in)
{
  DictSet dataset;
  std::string name;
  size_t size;
  while (in >> name >> size)
  {
    Dictionary dict;
    std::string key;
    WordSet value;
    for (size_t i = 0; i < size && in; i++)
    {
      in >> key >> value;
      dict[key] = value;
    }
    if (in)
    {
      dataset[name] = dict;
    }
  }
  if ((in).fail() && !(in).eof())
  {
    throw std::logic_error("<INVALID DICTIONARIES DESCRIPTION>");
  }
  return dataset;
}

void alymova::saveDictionaryFile(std::ostream& out, const DictSet& set)
{
  if (set.empty())
  {
    return;
  }
  auto it = set.begin();
  for (; it != --set.end(); it++)
  {
    const Dictionary& dict = it->second;
    out << it->first << ' ' << dict.size();
    out << dict << '\n';
  }
  const Dictionary& dict = it->second;
  out << it->first << ' ' << dict.size();
  out << dict;
}

void alymova::printHelp(std::ostream& out)
{
  out << std::setw(60) << std::left << "create <dict> " << "create new empty dictionary\n";
  out << std::setw(60) << std::left << "dictionary_list" << "print list of current dictionaries\n";
  out << std::setw(60) << std::left << "size <name> " << "print dictionary size\n";
  out << std::setw(60) << std::left << "remove_dictionary <name> " << "remove existing dictionary\n";
  out << std::setw(60) << std::left << "add_word <dict> <word> <translate> ";
  out << "add word with translate in dictionary\n";
  out << std::setw(60) << std::left << "fix_word <dict> <word> <new_word> ";
  out << "replace word in dictionary by new_word\n";
  out << std::setw(60) << std::left << "find_word <dict> <word> ";
  out << "print word translates from dictionary\n";
  out << std::setw(60) << std::left << "contain <dict> <subword> ";
  out << "print words that contain subword in dictionary\n";
  out << std::setw(60) << std::left << "remove_word <dict> <word> ";
  out << "remove existing word from existing dictionary\n";
  out << std::setw(60) << std::left << "add_translate <dict> <word> <translate> ";
  out << "add new translate to existing word in dictionary\n";
  out << std::setw(60) << std::left << "find_english_equivalent <dict> <tr_count> <tr_1> ... <tr_n> ";
  out << "find english equivalents of translates in dictionary\n";
  out << std::setw(60) << std::left << "remove_translate <dict> <word> <translate> ";
  out << "remove existing translate of existing word in existing dictionary\n";
  out << std::setw(60) << std::left << "print_content <dict> ";
  out << "print dictionary content by letters\n";
  out << std::setw(60) << std::left << "translate <dict_count> <dict_1> ... <dict_n> <word> ";
  out << "print word translates from dictionaries\n";
  out << std::setw(60) << std::left << "union <dict1> <dict2> ";
  out << "create new dictionary as union of dictionary1, dictionary2\n";
  out << std::setw(60) << std::left << "intersect <dict1> <dict2> ";
  out << "create new dictionary as intersection of dictionary1, dictionary2\n";
  out << std::setw(60) << std::left << "day_word <dict> ";
  out << "print word of day:)\n";
  out << "\nIf dictionaries were load from file, result of programm will be saved in this file.\n";
  out << "In the opposite case result of programm will be saved in file \"dictionaries\".";
}
