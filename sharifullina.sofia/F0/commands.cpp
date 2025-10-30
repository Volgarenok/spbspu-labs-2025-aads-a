#include "commands.hpp"
#include <fstream>
#include <numeric>
#include <iterator>
#include <iomanip>
#include <sstream>

namespace
{
  bool dictExists(const std::string & name, const sharifullina::DictCollection & dicts)
  {
    for (auto it = dicts.begin(); it != dicts.end(); ++it)
    {
      if (it->first == name)
      {
        return true;
      }
    }
    return false;
  }

  bool wordExists(const std::string & dictName, const std::string & word, const sharifullina::DictCollection & dicts)
  {
    for (auto it = dicts.begin(); it != dicts.end(); ++it)
    {
      if (it->first == dictName)
      {
        const auto & dict = it->second;
        for (auto jt = dict.begin(); jt != dict.end(); ++jt)
        {
          if (jt->first == word)
          {
            return true;
          }
        }
      }
    }
    return false;
  }

  void readTranslations(std::istream & in, sharifullina::AVLtree< std::string, bool > & translations)
  {
    std::string translation;
    while (in >> translation)
    {
      translations.insert({translation, true});
    }
  }

  sharifullina::Dictionary readDictionary(std::istream & in)
  {
    sharifullina::Dictionary dict;
    std::string line;
    while (std::getline(in, line))
    {
      if (line.empty())
      {
        continue;
      }
      std::string word;
      sharifullina::AVLtree< std::string, bool > translations;
      std::stringstream lineStream(line);
      if (lineStream >> word)
      {
        readTranslations(lineStream, translations);
      }
      if (!translations.empty())
      {
        auto & temp = dict.at(word);
        temp = translations;
      }
    }
    return dict;
  }

  const std::string & findInList(const sharifullina::List< std::string > & list, size_t i)
  {
    if (i > list.size())
    {
      throw std::out_of_range("list is too small");
    }
    auto it = list.cbegin();
    for (size_t k = 0; k != i; ++k, ++it)
    {
      if (i == k)
      {
        return *it;
      }
    }
    throw std::runtime_error("unexpected behaviour");
  }
}
void sharifullina::createDict(std::istream & in, DictCollection & dicts)
{
  std::string name;

  if (!(in >> name))
  {
    throw std::runtime_error("invalid arguments for createdict");
  }

  if (dictExists(name, dicts))
  {
    throw std::runtime_error("dictionary already exists");
  }

  auto & temp = dicts.at(name);
  temp = Dictionary{};
}

void sharifullina::deleteDict(std::istream & in, DictCollection & dicts)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::runtime_error("invalid arguments for deletedict");
  }
  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    throw std::runtime_error("dictionary not found");
  }
  dicts.erase(it);
}

void sharifullina::listDicts(std::istream &, const DictCollection & dicts, std::ostream & out)
{
  if (dicts.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  for (auto it = dicts.begin(); it != dicts.end(); ++it)
  {
    out << it->first << '\n';
  }
}

void sharifullina::addWord(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  std::string word;
  if (!(in >> dictName >> word))
  {
    throw std::runtime_error("invalid arguments for addword");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary not found");
  }
  auto & dict = dicts.at(dictName);
  if (dict.find(word) != dict.end())
  {
    throw std::runtime_error("word already exists");
  }
  sharifullina::AVLtree< std::string, bool > translations;
  readTranslations(in, translations);
  if (translations.empty())
  {
    throw std::runtime_error("no translations provided");
  }
  auto & temp = dict.at(word);
  temp = translations;
}

void sharifullina::addTranslation(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  std::string word;
  std::string translation;
  if (!(in >> dictName >> word >> translation))
  {
    throw std::runtime_error("invalid arguments for addtranslation");
  }
  if (!wordExists(dictName, word, dicts))
  {
    throw std::runtime_error("dictionary or word not found");
  }
  auto & temp = dicts.at(dictName).at(word);
  temp.insert({translation, true});
}

void sharifullina::removeTranslation(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  std::string word;
  std::string translation;
  if (!(in >> dictName >> word >> translation))
  {
    throw std::runtime_error("invalid arguments for removetranslation");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary, word or translation not found");
  }
  auto & dict = dicts.at(dictName);
  auto wordIt = dict.find(word);
  if (wordIt == dict.end())
  {
    throw std::runtime_error("dictionary, word or translation not found");
  }
  auto & translations = wordIt->second;
  auto transIt = translations.find(translation);
  if (transIt == translations.cend())
  {
    throw std::runtime_error("dictionary, word or translation not found");
  }
  auto & temp = dict.at(word);
  temp.erase(transIt->first);
  if (translations.empty())
  {
    dict.erase(wordIt);
  }
}

void sharifullina::deleteWord(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  std::string word;
  if (!(in >> dictName >> word))
  {
    throw std::runtime_error("invalid arguments for deleteword");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary or word not found");
  }
  auto & dict = dicts.at(dictName);
  if (dict.erase(word) == 0)
  {
    throw std::runtime_error("dictionary or word not found");
  }
}

void sharifullina::findTranslations(std::istream & in, const DictCollection & dicts, std::ostream & out)
{
  std::string dictName;
  std::string word;
  if (!(in >> dictName >> word))
  {
    throw std::runtime_error("invalid arguments for findtranslations");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary or word not found");
  }
  const auto & translations = dicts.at(dictName).at(word);
  bool first = true;
  for (auto t = translations.cbegin(); t != translations.cend(); ++t)
  {
    if (!first) out << ' ';
    out << t->first;
    first = false;
  }
  out << '\n';
}

void sharifullina::listWords(std::istream & in, const DictCollection & dicts, std::ostream & out)
{
  std::string dictName;
  if (!(in >> dictName))
  {
    throw std::runtime_error("invalid arguments for listwords");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary not found");
  }
  const auto & dict = dicts.at(dictName);
  if (dict.empty())
  {
    out << "<EMPTY>\n";
    return;
  }

  for (auto it = dict.begin(); it != dict.end(); ++it)
  {
    out << it->first << ' ';
    const auto & translations = it->second;
    bool first = true;
    for (auto t = translations.cbegin(); t != translations.cend(); ++t)
    {
      if (!first) out << ' ';
      out << t->first;
      first = false;
    }
    out << '\n';
  }
}

void sharifullina::mergeDicts(std::istream & in, DictCollection & dicts)
{
  std::string newDictName;
  size_t count = 0;
  if (!(in >> newDictName >> count))
  {
    throw std::runtime_error("invalid arguments for merge");
  }
  if (count < 2)
  {
    throw std::runtime_error("invalid count");
  }
  sharifullina::List< std::string > dictNames;
  for (size_t i = 0; i < count; ++i)
  {
    std::string name;
    if (!(in >> name))
    {
      throw std::runtime_error("invalid count");
    }
    dictNames.pushBack(name);
  }
  for (size_t i = 0; i < dictNames.size(); ++i)
  {
    if (!dictExists(findInList(dictNames, i), dicts))
    {
      throw std::runtime_error("dictionary not found");
    }
  }
  if (dictExists(newDictName, dicts))
  {
    throw std::runtime_error("dictionary already exists");
  }

  Dictionary mergedDict;
  for (size_t i = 0; i < dictNames.size(); ++i)
  {
    const Dictionary & current = dicts.at(findInList(dictNames, i));
    for (auto it = current.begin(); it != current.end(); ++it)
    {
      const std::string & word = it->first;
      const auto & translations = it->second;

      if (mergedDict.find(word) == mergedDict.end())
      {
        auto & temp = mergedDict.at(word);
        temp = translations;
      }
      else
      {
        auto & exist = mergedDict.at(word);
        for (auto t = translations.cbegin(); t != translations.cend(); ++t)
        {
          exist.insert(*t);
        }
      }
    }
  }
  dicts.insert(newDictName, mergedDict);
}

void sharifullina::findCommon(std::istream & in, const DictCollection & dicts, std::ostream & out)
{
  std::string dictName;
  size_t count = 0;
  if (!(in >> dictName >> count))
  {
    throw std::runtime_error("invalid arguments for findcommon");
  }
  if (count < 1)
  {
    throw std::runtime_error("invalid count");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary or word(s) not found");
  }
  sharifullina::List< std::string > words;
  for (size_t i = 0; i < count; ++i)
  {
    std::string word;
    if (!(in >> word))
    {
      throw std::runtime_error("invalid count");
    }
    words.pushBack(word);
  }
  const Dictionary & dict = dicts.at(dictName);
  for (auto it = words.begin(); it != words.end(); ++it)
  {
    if (dict.find(*it) == dict.end())
    {
      throw std::runtime_error("dictionary or word(s) not found");
    }
  }

  AVLtree< std::string, bool > commonTranslations;
  bool first = true;
  for (auto it = words.begin(); it != words.end(); ++it)
  {
    const TransSet & ts = dict.at(*it);
    if (first)
    {
      for (auto t = ts.cbegin(); t != ts.cend(); ++t)
      {
        commonTranslations.insert(*t);
      }
      first = false;
    }
    else
    {
      AVLtree< std::string, bool > temp;
      for (auto t = ts.cbegin(); t != ts.cend(); ++t)
      {
        if (commonTranslations.find(t->first) != commonTranslations.end())
        {
          temp.insert(*t);
        }
      }
      commonTranslations = temp;
    }
  }
  if (commonTranslations.empty())
  {
    out << "<EMPTY>\n"; return;
  }
  for (auto t = commonTranslations.begin(); t != commonTranslations.end(); ++t)
  {
    out << t->first << ' ';
  }
  out << '\n';
}

void sharifullina::saveDict(std::istream & in, const DictCollection & dicts, std::ostream &)
{
  std::string dictName;
  std::string filename;
  if (!(in >> dictName >> filename))
  {
    throw std::runtime_error("invalid arguments for save");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary not found or file error");
  }
  std::ofstream file(filename);
  if (!file)
  {
    throw std::runtime_error("dictionary not found or file error");
  }
  const Dictionary & dict = dicts.at(dictName);
  for (auto it = dict.begin(); it != dict.end(); ++it)
  {
    file << it->first;
    const TransSet & ts = it->second;
    for (auto t = ts.cbegin(); t != ts.cend(); ++t)
    {
      file << ' ' << t->first;
    }
    file << '\n';
  }
}

void sharifullina::loadDict(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  std::string filename;
  if (!(in >> dictName >> filename))
  {
    throw std::runtime_error("invalid arguments for load");
  }
  if (dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary already exists");
  }
  std::ifstream file(filename);
  if (!file)
  {
    throw std::runtime_error("file not found or invalid format");
  }
  Dictionary newDict = readDictionary(file);
  if (newDict.empty())
  {
    throw std::runtime_error("file not found or invalid format");
  }
  dicts.insert(dictName, newDict);
}

void sharifullina::statDict(std::istream & in, const DictCollection & dicts, std::ostream & out)
{
  std::string dictName;
  if (!(in >> dictName))
  {
    throw std::runtime_error("invalid arguments for stat");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary not found");
  }
  const Dictionary & dict = dicts.at(dictName);
  size_t totalWords = dict.size();
  size_t totalTranslations = 0;
  for (auto it = dict.begin(); it != dict.end(); ++it)
  {
    totalTranslations += it->second.size();
  }
  double avgTranslations = totalWords > 0 ? static_cast< double >(totalTranslations) / totalWords : 0.0;

  out << "Words: " << totalWords << "\n";
  out << "Translations: " << totalTranslations << "\n";
  out << "Average translations per word: " << avgTranslations << "\n";
}

void sharifullina::subtractDicts(std::istream & in, DictCollection & dicts)
{
  std::string newDictName;
  size_t count = 0;
  if (!(in >> newDictName >> count))
  {
    throw std::runtime_error("invalid arguments for subtract");
  }
  if (count < 2)
  {
    throw std::runtime_error("invalid count");
  }
  if (dictExists(newDictName, dicts))
  {
    throw std::runtime_error("dictionary already exists");
  }
  sharifullina::List< std::string > dictNames;
  for (size_t i = 0; i < count; ++i)
  {
    std::string name;
    if (!(in >> name))
    {
      throw std::runtime_error("invalid count");
    }
    dictNames.pushBack(name);
    if (!dictExists(name, dicts))
    {
      throw std::runtime_error("dictionary not found");
    }
  }
  const Dictionary & baseDict = dicts.at(findInList(dictNames, 0));
  Dictionary resultDict = baseDict;
  for (size_t i = 1; i < dictNames.size(); ++i)
  {
    const Dictionary & other = dicts.at(findInList(dictNames, i));
    for (auto it = other.begin(); it != other.end(); ++it)
    {
      resultDict.erase(it->first);
    }
  }
  dicts.insert(newDictName, resultDict);
}

void sharifullina::symdiffDicts(std::istream & in, DictCollection & dicts)
{
  std::string newDictName;
  size_t count = 0;
  if (!(in >> newDictName >> count))
  {
    throw std::runtime_error("invalid arguments for symdiff");
  }
  if (count < 2)
  {
    throw std::runtime_error("invalid count");
  }
  sharifullina::List< std::string > dictNames;
  for (size_t i = 0; i < count; ++i)
  {
    std::string name;
    if (!(in >> name))
    {
      throw std::runtime_error("invalid count");
    }
    dictNames.pushBack(name);
    if (!dictExists(name, dicts))
    {
      throw std::runtime_error("dictionary not found");
    }
  }
  if (dictExists(newDictName, dicts))
  {
    throw std::runtime_error("dictionary already exists");
  }
  Dictionary resultDict;
  for (size_t i = 0; i < dictNames.size(); ++i)
  {
    const Dictionary & current = dicts.at(findInList(dictNames, i));
    for (auto it = current.begin(); it != current.end(); ++it)
    {
      if (resultDict.find(it->first) == resultDict.end())
      {
        auto & temp = resultDict.at(it->first);
        temp = it->second;
      }
      else
      {
        auto & existing = resultDict.at(it->first);
        for (auto t = it->second.cbegin(); t != it->second.cend(); ++t)
        {
          if (existing.find(t->first) != existing.end())
          {
            existing.erase(t->first);
          }
          else
          {
            existing.insert({t->first, true});
          }
        }
      }
    }
  }
  dicts.insert(newDictName, resultDict);
}

void sharifullina::loadFile(const std::string & filename, DictCollection & dicts)
{
  std::ifstream file(filename);
  if (!file)
  {
    throw std::runtime_error("file not found or invalid format");
  }
  Dictionary newDict = readDictionary(file);
  if (!newDict.empty())
  {
    dicts.insert("default", newDict);
  }
}

void sharifullina::printHelp(std::ostream & out)
{
  out << std::left;
  out << "Available commands:\n" << '\n';
  constexpr size_t cmdWidth = 30;
  constexpr size_t numWidth = 3;
  out << std::setw(numWidth) << "1." << std::setw(cmdWidth);
  out << "createdict <name>" << "create a new dictionary\n";

  out << std::setw(numWidth) << "2." << std::setw(cmdWidth);
  out << "deletedict <name>" << "delete a dictionary\n";

  out << std::setw(numWidth) << "3." << std::setw(cmdWidth);
  out << "listdicts" << "list all dictionaries\n";

  out << std::setw(numWidth) << "4." << std::setw(cmdWidth);
  out << "addword <dict> <word> <trans...>" << "add word with translations\n";

  out << std::setw(numWidth) << "5." << std::setw(cmdWidth);
  out << "addtranslation <dict> <word> <trans>" << "add translation to word\n";

  out << std::setw(numWidth) << "6." << std::setw(cmdWidth);
  out << "removetranslation <dict> <word> <trans>" << "remove translation\n";

  out << std::setw(numWidth) << "7." << std::setw(cmdWidth);
  out << "deleteword <dict> <word>" << "delete word\n";

  out << std::setw(numWidth) << "8." << std::setw(cmdWidth);
  out << "findtranslations <dict> <word>" << "find word translations\n";

  out << std::setw(numWidth) << "9." << std::setw(cmdWidth);
  out << "listwords <dict>" << "list all words in dictionary\n";

  out << std::setw(numWidth) << "10." << std::setw(cmdWidth);
  out << "merge <new> <count> <dicts...>" << "merge dictionaries\n";

  out << std::setw(numWidth) << "11." << std::setw(cmdWidth);
  out << "findcommon <dict> <count> <words...>" << "find common translations\n";

  out << std::setw(numWidth) << "12." << std::setw(cmdWidth);
  out << "save <dict> <file>" << "save dictionary to file\n";

  out << std::setw(numWidth) << "13." << std::setw(cmdWidth);
  out << "load <dict> <file>" << "load dictionary from file\n";

  out << std::setw(numWidth) << "14." << std::setw(cmdWidth);
  out << "stat <dict>" << "show dictionary statistics\n";

  out << std::setw(numWidth) << "15." << std::setw(cmdWidth);
  out << "subtract <new> <count> <dicts...>" << "dictionary subtraction\n";

  out << std::setw(numWidth) << "16." << std::setw(cmdWidth);
  out << "symdiff <new> <count> <dicts...>" << "symmetric difference\n";
}

void sharifullina::printError(const std::string & message, std::ostream & out)
{
  out << "<ERROR: " << message << ">\n";
}
