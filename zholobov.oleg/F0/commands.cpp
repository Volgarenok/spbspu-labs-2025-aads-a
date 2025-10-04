#include "commands.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "utils.hpp"

void zholobov::printHelp(std::ostream& out)
{
  out << "Commands:\n";
  out << "dict-create <dict>\n";
  out << "dict-remove <dict>\n";
  out << "dict-import <dict> <file>\n";
  out << "dict-export <dict> <file> [overwrite]\n";
  out << "dict-count\n";
  out << "dict-list\n";
  out << "dict-print-words <dict>\n";
  out << "dict-print-translations [<dict>]\n";
  out << "dict-clear [<dict>]\n";
  out << "add-word <dict> <word> <translation>\n";
  out << "remove-word <dict> <word>\n";
  out << "count-words <dict>\n";
  out << "add-translation <dict> <word> <translation>\n";
  out << "remove-translation <dict> <word> <translation>\n";
  out << "change-word <dict> <word> <new_word>\n";
  out << "translate-word <word>\n";
  out << "union <new_dict> <dict-1> [<dict-N> ...]\n";
  out << "intersect <new_dict> <dict-1> [<dict-N> ...]\n";
  out << "rare <N> <new_dict> <dict-1> [<dict-K> ...]\n";
}

void zholobov::cmdDictCreate(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (args.size() != 2) {
    throw InvalidParams();
  }
  const std::string& name = args[1];
  if (dictionaries.count(name)) {
    std::cout << "<DICTIONARY ALREADY EXISTS>\n";
  } else {
    dictionaries[name] = Dictionary();
  }
}

void zholobov::cmdDictRemove(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (args.size() != 2) {
    throw InvalidParams();
  }
  const std::string& name = args[1];
  if (!dictionaries.count(name)) {
    std::cout << "<INVALID DICTIONARY>\n";
  } else {
    dictionaries.erase(name);
  }
}

void zholobov::cmdDictImport(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (args.size() != 3) {
    throw InvalidParams();
  }
  const std::string& dictName = args[1];
  const std::string& fileName = args[2];

  Dictionary& dict = dictionaries[dictName];

  std::ifstream fin(fileName);
  if (!fin) {
    std::cout << "<0 TRANSLATIONS IMPORTED>.\n";
    return;
  }

  int imported = 0;
  int dropped = 0;
  Words tokens;
  while (fin >> tokens) {
    if (tokens.empty()) {
      continue;
    }

    Word eng = tokens.front();
    tokens.pop_front();
    if (tokens.empty()) {
      ++dropped;
      continue;
    }

    Words& entry = dict[eng];
    for (size_t i = 0; i < tokens.size(); ++i) {
      bool found = false;
      for (size_t k = 0; k < entry.size(); ++k) {
        if (entry[k] == tokens[i]) {
          found = true;
          break;
        }
      }
      if (!found) {
        entry.push_back(tokens[i]);
      }
    }
    ++imported;
  }

  std::cout << "<" << imported << " TRANSLATIONS IMPORTED>.";
  if (dropped != 0) {
    std::cout << " [" << dropped << " - DROPPED]";
  }
  std::cout << '\n';
}

void zholobov::cmdDictExport(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (!(args.size() == 3 || args.size() == 4)) {
    throw InvalidParams();
  }
  const std::string& dictName = args[1];
  const std::string& fileName = args[2];
  bool overwrite = ((args.size() == 4) && (args[3] == "overwrite"));

  auto it = dictionaries.find(dictName);
  if (it == dictionaries.end()) {
    std::cout << "<INVALID DICTIONARY>\n";
    return;
  }

  std::ifstream check(fileName);
  if (check && !overwrite) {
    std::cout << "FILE EXISTS\n";
    return;
  }

  std::ofstream fout(fileName, std::ios::trunc);
  if (!fout) {
    return;
  }
  fout << it->second;
}

void zholobov::cmdDictCount(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (args.size() != 1) {
    throw InvalidParams();
  }
  std::cout << dictionaries.size() << '\n';
}

void zholobov::cmdDictList(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (args.size() != 1) {
    throw InvalidParams();
  }
  for (const auto& dict: dictionaries) {
    std::cout << dict.first << '\n';
  }
}

void zholobov::cmdDictPrintWords(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (args.size() != 2) {
    throw InvalidParams();
  }
  const std::string& name = args[1];
  auto it = dictionaries.find(name);
  if (it == dictionaries.end()) {
    std::cout << "<INVALID DICTIONARY>\n";
    return;
  }
  for (const auto& record: it->second) {
    std::cout << record.first << '\n';
  }
}

void zholobov::cmdDictPrintTranslations(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (!(args.size() == 1 || args.size() == 2)) {
    throw InvalidParams();
  }
  if (args.size() == 2) {
    auto it = dictionaries.find(args[1]);
    if (it == dictionaries.end()) {
      std::cout << "<INVALID DICTIONARY>\n";
      return;
    }
    std::cout << it->second;
  } else {
    if (dictionaries.empty()) {
      std::cout << "<NO DICTIONARIES>\n";
      return;
    }
    for (const auto& dict: dictionaries) {
      std::cout << dict.second;
    }
  }
}

void zholobov::cmdDictClear(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (!(args.size() == 1 || args.size() == 2)) {
    throw InvalidParams();
  }
  if (args.size() == 2) {
    auto it = dictionaries.find(args[1]);
    if (it == dictionaries.end()) {
      std::cout << "<INVALID DICTIONARY>\n";
      return;
    }
    it->second.clear();
  } else {
    for (auto& dict: dictionaries) {
      dict.second.clear();
    }
  }
}

void zholobov::cmdAddWord(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (args.size() != 4) {
    throw InvalidParams();
  }
  auto it = dictionaries.find(args[1]);
  if (it == dictionaries.end()) {
    std::cout << "<INVALID DICTIONARY>\n";
    return;
  }
  it->second[args[2]].push_back(args[3]);
}

void zholobov::cmdRemoveWord(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (args.size() != 3) {
    throw InvalidParams();
  }
  auto it = dictionaries.find(args[1]);
  if (it == dictionaries.end()) {
    std::cout << "<INVALID DICTIONARY>\n";
    return;
  }
  if (!it->second.erase(args[2])) {
    std::cout << "<INVALID WORD>\n";
  }
}

void zholobov::cmdCountWords(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (args.size() != 2) {
    throw InvalidParams();
  }
  auto it = dictionaries.find(args[1]);
  if (it == dictionaries.end()) {
    std::cout << "<INVALID DICTIONARY>\n";
    return;
  }
  std::cout << it->second.size() << '\n';
}

void zholobov::cmdAddTranslation(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (args.size() != 4) {
    throw InvalidParams();
  }
  auto dit = dictionaries.find(args[1]);
  if (dit == dictionaries.end()) {
    std::cout << "<INVALID DICTIONARY>\n";
    return;
  }
  auto wit = dit->second.find(args[2]);
  if (wit == dit->second.end()) {
    std::cout << "<INVALID WORD>\n";
    return;
  }

  for (size_t i = 0; i < wit->second.size(); ++i) {
    if (wit->second[i] == args[3]) {
      std::cout << "<TRANSLATION EXISTS>\n";
      return;
    }
  }
  wit->second.push_back(args[3]);
}

void zholobov::cmdRemoveTranslation(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (args.size() != 4) {
    throw InvalidParams();
  }
  auto dit = dictionaries.find(args[1]);
  if (dit == dictionaries.end()) {
    std::cout << "<INVALID DICTIONARY>\n";
    return;
  }
  auto wit = dit->second.find(args[2]);
  if (wit == dit->second.end()) {
    std::cout << "<INVALID WORD>\n";
    return;
  }
  auto& lst = wit->second;
  bool found = false;
  size_t i = 0;
  for (; i < lst.size(); ++i) {
    if (lst[i] == args[3]) {
      found = true;
      break;
    }
  }
  if (!found) {
    std::cout << "<INVALID TRANSLATION>\n";
    return;
  }
  lst.erase(i);
  if (lst.empty()) {
    dit->second.erase(wit);
    std::cout << "<REMOVED LAST TRANSLATION. WORD REMOVED>\n";
  }
}

void zholobov::cmdChangeWord(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (args.size() != 4) {
    throw InvalidParams();
  }
  auto dit = dictionaries.find(args[1]);
  if (dit == dictionaries.end()) {
    std::cout << "<INVALID DICTIONARY>\n";
    return;
  }
  auto wit = dit->second.find(args[2]);
  if (wit == dit->second.end()) {
    std::cout << "<INVALID WORD>\n";
    return;
  }
  if (dit->second.count(args[3])) {
    std::cout << "<NEW WORD EXISTS>\n";
    return;
  }
  dit->second[args[3]] = wit->second;
  dit->second.erase(wit);
}

void zholobov::cmdTranslateWord(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (args.size() != 2) {
    throw InvalidParams();
  }
  HashSet< Word > uniq;
  for (const auto& dict: dictionaries) {
    auto wit = dict.second.find(args[1]);
    if (wit != dict.second.end()) {
      for (size_t i = 0; i < wit->second.size(); ++i) {
        uniq[wit->second[i]] = {};
      }
    }
  }
  if (uniq.empty()) {
    std::cout << "<INVALID WORD>\n";
    return;
  }

  auto it = uniq.cbegin();
  std::cout << it->first;
  for (++it; it != uniq.cend(); ++it) {
    std::cout << " " << it->first;
  }
  std::cout << '\n';
}

void zholobov::cmdUnion(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (args.size() < 3) {
    throw InvalidParams();
  }

  const std::string& newName = args[1];

  for (std::size_t i = 2; i < args.size(); ++i) {
    if (dictionaries.find(args[i]) == dictionaries.end()) {
      std::cout << "<INVALID DICTIONARY>\n";
      return;
    }
  }

  Tree< Word, HashSet< Word > > temp;
  for (std::size_t i = 2; i < args.size(); ++i) {
    const auto it = dictionaries.find(args[i]);
    const Dictionary& src = it->second;
    for (const auto& kv: src) {
      HashSet< Word >& s = temp[kv.first];
      for (size_t k = 0; k < kv.second.size(); ++k) {
        s[kv.second[k]] = {};
      }
    }
  }

  Dictionary result;
  for (const auto& kv: temp) {
    Words words;
    for (auto it = kv.second.cbegin(); it != kv.second.cend(); ++it) {
      words.push_back(it->first);
    }
    result[kv.first] = words;
  }

  dictionaries[newName] = std::move(result);
}

void zholobov::cmdIntersect(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (args.size() < 3) {
    throw InvalidParams();
  }

  const std::string& newName = args[1];

  for (std::size_t i = 2; i < args.size(); ++i) {
    if (dictionaries.find(args[i]) == dictionaries.end()) {
      std::cout << "<INVALID DICTIONARY>\n";
      return;
    }
  }

  const auto firstIt = dictionaries.find(args[2]);
  const Dictionary& firstDict = firstIt->second;
  HashSet< Word > commonKeys;
  for (const auto& kv: firstDict) {
    commonKeys[kv.first] = {};
  }

  for (std::size_t i = 3; i < args.size(); ++i) {
    const auto it = dictionaries.find(args[i]);
    const Dictionary& d = it->second;
    HashSet< Word > newCommon;
    for (const auto& key: commonKeys) {
      if (d.find(key.first) != d.end()) newCommon[key.first] = {};
    }
    commonKeys.swap(newCommon);
    if (commonKeys.empty()) {
      break;
    }
  }

  Dictionary result;
  for (const auto& key: commonKeys) {
    HashSet< Word > uniq;
    for (std::size_t i = 2; i < args.size(); ++i) {
      const auto it = dictionaries.find(args[i]);
      const Dictionary& d = it->second;
      auto wit = d.find(key.first);
      if (wit != d.end()) {
        for (size_t i = 0; i < wit->second.size(); ++i) {
          uniq[wit->second[i]] = {};
        }
      }
    }
    Words words;
    for (auto it = uniq.cbegin(); it != uniq.cend(); ++it) {
      words.push_back(it->first);
    }
    result[key.first] = words;
  }

  dictionaries[newName] = std::move(result);
}

void zholobov::cmdRare(Dictionaries& dictionaries, const Array< std::string >& args)
{
  if (args.size() < 4) {
    throw InvalidParams();
  }

  int n = 0;
  try {
    n = std::stoi(args[1]);
  } catch (...) {
    std::cout << "<INVALID NUMBER>\n";
    return;
  }

  if (n < 1) {
    std::cout << "<INVALID NUMBER>\n";
    return;
  }

  const std::string& newName = args[2];

  for (std::size_t i = 3; i < args.size(); ++i) {
    if (dictionaries.find(args[i]) == dictionaries.end()) {
      std::cout << "<INVALID DICTIONARY>\n";
      return;
    }
  }

  Tree< Word, int > wordFrequency;
  for (std::size_t i = 3; i < args.size(); ++i) {
    const auto& dict = dictionaries[args[i]];
    for (const auto& kv: dict) {
      wordFrequency[kv.first]++;
    }
  }

  Tree< Word, HashSet< Word > > temp;
  for (std::size_t i = 3; i < args.size(); ++i) {
    const auto& dict = dictionaries[args[i]];
    for (const auto& kv: dict) {
      if (wordFrequency[kv.first] <= n) {
        HashSet< Word >& s = temp[kv.first];
        for (size_t i = 0; i < kv.second.size(); ++i) {
          s[kv.second[i]] = {};
        }
      }
    }
  }

  Dictionary result;
  for (const auto& kv: temp) {
    Words words;
    for (auto it = kv.second.cbegin(); it != kv.second.cend(); ++it) {
      words.push_back(it->first);
    }
    result[kv.first] = words;
  }

  dictionaries[newName] = std::move(result);
}
