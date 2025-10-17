#include "commands.hpp"
#include <string>
#include <fstream>
#include "array.hpp"

void smirnov::printCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  const Dict & dict = dictIt->second;
  if (dict.empty())
  {
    out << dictName << " is empty.\n";
    return;
  }
  for (auto it = dict.cbegin(); it != dict.cend(); ++it)
  {
    out << it->first << " -";
    const Array & translations = it->second;
    for (std::size_t i = 0; i < translations.size(); ++i)
    {
      out << " " << translations[i];
    }
    out << "\n";
  }
}

void smirnov::addCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName, word, translation;
  in >> dictName >> word >> translation;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  Dict & dict = dictIt->second;
  auto wordIt = dict.find(word);
  if (wordIt != dict.end())
  {
    Array & translations = wordIt->second;
    for (std::size_t i = 0; i < translations.size(); ++i)
    {
      if (translations[i] == translation)
      {
        out << "The word " << word << " already exists in " << dictName << "\n";
        return;
      }
    }
    translations.push_back(translation);
    return;
  }
  Array translations;
  translations.push_back(translation);
  dict.insert(word, translations);
}

void smirnov::translateCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName, word;
  in >> dictName >> word;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  const Dict & dict = dictIt->second;
  auto wordIt = dict.find(word);
  if (wordIt == dict.cend())
  {
    out << "The word " << word << " doesn't exist in " << dictName << "\n";
    return;
  }
  const Array & translations = wordIt->second;
  for (std::size_t i = 0; i < translations.size(); ++i)
  {
    out << translations[i];
    if (i + 1 < translations.size())
    {
      out << " ";
    }
  }
  out << "\n";
}

void smirnov::removeCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName, word;
  in >> dictName >> word;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  Dict & dict = dictIt->second;
  auto wordIt = dict.find(word);
  if (wordIt == dict.end())
  {
    out << "The word " << word << " doesn't exist in " << dictName << "\n";
    return;
  }
  dict.erase(wordIt);
  out << "The word " << word << " successfully deleted from " << dictName << "\n";
}

void smirnov::createCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string name;
  in >> name;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (dicts.find(name) != dicts.end())
  {
    out << "The dictionary with name " << name << " already exists.\n";
    return;
  }
  Dict dict;
  dicts.insert(name, dict);
  out << name << " is successfully created.\n";
}

void smirnov::saveCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName, fileName;
  in >> dictName >> fileName;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (fileName.size() < 4 || fileName.substr(fileName.size() - 4) != ".txt")
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  std::ofstream file(fileName);
  if (!file)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  file << dictName << "\n";
  const Dict & dict = dictIt->second;
  for (auto it = dict.cbegin(); it != dict.cend(); ++it)
  {
    file << it->first << " -";
    const Array & translations = it->second;
    for (std::size_t i = 0; i < translations.size(); ++i)
    {
      file << " " << translations[i];
    }
    file << "\n";
  }
}

void smirnov::mergeCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string newName, dict1Name, dict2Name;
  in >> newName >> dict1Name >> dict2Name;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (dicts.find(newName) != dicts.end())
  {
    out << "The dictionary with name " << newName << " already exists.\n";
    return;
  }
  auto it1 = dicts.find(dict1Name);
  auto it2 = dicts.find(dict2Name);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const Dict & dict1 = it1->second;
  const Dict & dict2 = it2->second;
  Dict result;
  for (auto it = dict1.cbegin(); it != dict1.cend(); ++it)
  {
    result.insert(it->first, it->second);
  }
  for (auto it = dict2.cbegin(); it != dict2.cend(); ++it)
  {
    auto resIt = result.find(it->first);
    if (resIt == result.end())
    {
      result.insert(it->first, it->second);
    }
    else
    {
      Array merged = resIt->second;
      const Array & addTr = it->second;
      for (std::size_t i = 0; i < addTr.size(); ++i)
      {
        bool found = false;
        for (std::size_t j = 0; j < merged.size(); ++j)
        {
          if (merged[j] == addTr[i])
          {
            found = true;
            break;
          }
        }
        if (!found)
        {
          merged.push_back(addTr[i]);
        }
      }
      result.erase(resIt);
      result.insert(it->first, merged);
    }
  }
  dicts.insert(newName, result);
  out << "Dictionary " << newName << " is successfully created\n";
}

void smirnov::deleteCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto it = dicts.find(dictName);
  if (it == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  dicts.erase(it);
}

void smirnov::editCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName, word, newTranslation;
  in >> dictName >> word >> newTranslation;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  Dict & dict = dictIt->second;
  auto wordIt = dict.find(word);
  if (wordIt == dict.end())
  {
    out << "The word " << word << " doesn't exist in " << dictName << "\n";
    return;
  }
  wordIt->second.clear();
  wordIt->second.push_back(newTranslation);
}

void smirnov::renameCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string oldName, newName;
  in >> oldName >> newName;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto oldIt = dicts.find(oldName);
  if (oldIt == dicts.end())
  {
    out << "The dictionary with name " << oldName << " doesn't exist.\n";
    return;
  }
  if (dicts.find(newName) != dicts.end())
  {
    out << "The dictionary with name " << newName << " already exists.\n";
    return;
  }
  Dict dictCopy = oldIt->second;
  dicts.erase(oldIt);
  dicts.insert(newName, dictCopy);
}

void smirnov::moveCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string fromDict, toDict, word;
  in >> fromDict >> toDict >> word;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto fromIt = dicts.find(fromDict);
  if (fromIt == dicts.end())
  {
    out << "The dictionary with name " << fromDict << " doesn't exist.\n";
    return;
  }
  auto toIt = dicts.find(toDict);
  if (toIt == dicts.end())
  {
    out << "The dictionary with name " << toDict << " doesn't exist.\n";
    return;
  }
  Dict & from = fromIt->second;
  Dict & to = toIt->second;
  auto wordIt = from.find(word);
  if (wordIt == from.end())
  {
    out << "The word " << word << " doesn't exist in " << fromDict << "\n";
    return;
  }
  if (to.find(word) != to.end())
  {
    out << "The word " << word << " already exists in " << toDict << "\n";
    return;
  }
  Array copyArr = wordIt->second;
  to.insert(word, copyArr);
  from.erase(wordIt);
  out << "The word " << word << " moved from " << fromDict << " to " << toDict << "\n";
}

void smirnov::copyCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string fromDict, toDict, word;
  in >> fromDict >> toDict >> word;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto fromIt = dicts.find(fromDict);
  if (fromIt == dicts.end())
  {
    out << "The dictionary with name " << fromDict << " doesn't exist.\n";
    return;
  }
  auto toIt = dicts.find(toDict);
  if (toIt == dicts.end())
  {
    out << "The dictionary with name " << toDict << " doesn't exist.\n";
    return;
  }
  const Dict & from = fromIt->second;
  Dict & to = toIt->second;
  auto wordIt = from.find(word);
  if (wordIt == from.cend())
  {
    out << "The word " << word << " doesn't exist in " << fromDict << "\n";
    return;
  }
  if (to.find(word) != to.end())
  {
    out << "The word " << word << " already exists in " << toDict << "\n";
    return;
  }
  Array copyArr = wordIt->second;
  to.insert(word, copyArr);
  out << "The word " << word << " copied from " << fromDict << " to " << toDict << "\n";
}

void smirnov::intersectCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string newName, dict1Name, dict2Name;
  in >> newName >> dict1Name >> dict2Name;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (dicts.find(newName) != dicts.end())
  {
    out << "The dictionary with name " << newName << " already exists.\n";
    return;
  }
  auto it1 = dicts.find(dict1Name);
  auto it2 = dicts.find(dict2Name);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const Dict & dict1 = it1->second;
  const Dict & dict2 = it2->second;
  Dict result;
  for (auto wit = dict1.cbegin(); wit != dict1.cend(); ++wit)
  {
    if (dict2.find(wit->first) != dict2.cend())
    {
      result.insert(wit->first, wit->second);
    }
  }
  dicts.insert(newName, result);
  out << "Dictionary " << newName << " is successfully created\n";
}

void smirnov::differenceCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string newName, dict1Name, dict2Name;
  in >> newName >> dict1Name >> dict2Name;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (dicts.find(newName) != dicts.end())
  {
    out << "The dictionary with name " << newName << " already exists.\n";
    return;
  }
  auto it1 = dicts.find(dict1Name);
  auto it2 = dicts.find(dict2Name);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const Dict & dict1 = it1->second;
  const Dict & dict2 = it2->second;
  Dict result;
  for (auto wit = dict1.cbegin(); wit != dict1.cend(); ++wit)
  {
    if (dict2.find(wit->first) == dict2.cend())
    {
      result.insert(wit->first, wit->second);
    }
  }
  dicts.insert(newName, result);
  out << "Dictionary " << newName << " is successfully created\n";
}

void smirnov::uniqueCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string newName, dict1Name, dict2Name;
  in >> newName >> dict1Name >> dict2Name;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (dicts.find(newName) != dicts.end())
  {
    out << "The dictionary with name " << newName << " already exists.\n";
    return;
  }
  auto it1 = dicts.find(dict1Name);
  auto it2 = dicts.find(dict2Name);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const Dict & dict1 = it1->second;
  const Dict & dict2 = it2->second;
  Dict result;
  for (auto wit = dict1.cbegin(); wit != dict1.cend(); ++wit)
  {
    if (dict2.find(wit->first) == dict2.cend())
    {
      result.insert(wit->first, wit->second);
    }
  }
  for (auto wit = dict2.cbegin(); wit != dict2.cend(); ++wit)
  {
    if (dict1.find(wit->first) == dict1.cend())
    {
      result.insert(wit->first, wit->second);
    }
  }
  dicts.insert(newName, result);
  out << "Dictionary " << newName << " is successfully created\n";
}

void smirnov::prefixCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string newName, dictName, prefix;
  in >> newName >> dictName >> prefix;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  if (dicts.find(newName) != dicts.end())
  {
    out << "The dictionary with name " << newName << " already exists.\n";
    return;
  }
  const Dict & dict = dictIt->second;
  if (dict.empty())
  {
    out << dictName << " is empty.\n";
    return;
  }
  Dict result;
  for (auto wit = dict.cbegin(); wit != dict.cend(); ++wit)
  {
    if (wit->first.compare(0, prefix.size(), prefix) == 0)
    {
      result.insert(wit->first, wit->second);
    }
  }
  if (result.empty())
  {
    out << "There aren't any words in " << dictName << " with prefix " << prefix << ".\n";
    return;
  }
  dicts.insert(newName, result);
}

void smirnov::importDictfromFile(Dicts & dicts, const std::string & filename)
{
  std::ifstream file(filename);
  if (!file)
  {
    throw std::runtime_error("Cannot open file\n");
  }
  std::string line;
  std::string dictName;
  Dict dict;
  bool hasDict = false;
  while (std::getline(file, line))
  {
    if (line.empty())
    {
      continue;
    }
    if (line.find(" - ") == std::string::npos)
    {
      if (hasDict)
      {
        dicts.insert(dictName, dict);
        dict = Dict();
      }
      dictName = line;
      hasDict = true;
    }
    else
    {
      std::size_t dashPos = line.find(" - ");
      if (dashPos == std::string::npos)
      {
        continue;
      }
      std::string word = line.substr(0, dashPos);
      std::string translationsStr = line.substr(dashPos + 3);
      Array translations;
      std::size_t start = 0;
      std::size_t end = translationsStr.find(' ');
      while (end != std::string::npos)
      {
        std::string translation = translationsStr.substr(start, end - start);
        if (!translation.empty())
        {
          translations.push_back(translation);
        }
        start = end + 1;
        end = translationsStr.find(' ', start);
      }
      std::string lastTranslation = translationsStr.substr(start);
      if (!lastTranslation.empty())
      {
        translations.push_back(lastTranslation);
      }
      dict.insert(word, translations);
    }
  }
  if (hasDict)
  {
    dicts.insert(dictName, dict);
  }
}

void smirnov::helpCommand(std::ostream & out)
{
  out << "Available commands:\n";
  out << "1) create <dict> - Create a new dictionary\n";
  out << "2) add <dict> <key> <translation> - Add a word/translation to dictionary\n";
  out << "3) translate <dict> <key> - Output all translations for a word\n";
  out << "4) remove <dict> <key> - Delete a word from the dictionary\n";
  out << "5) print <dict> - Show all words and translations from a dictionary\n";
  out << "6) save <dict> <filename> - Save dictionary to file\n";
  out << "7) merge <newdict> <dict1> <dict2> - Merge two dictionaries into a new one\n";
  out << "8) delete <dict> - Delete a dictionary\n";
  out << "9) edit <dict> <key> <translation> - Replace all translations for a word\n";
  out << "10) rename <dict> <newname> - Rename a dictionary\n";
  out << "11) move <from> <to> <key> - Move word (with translations) to another dictionary\n";
  out << "12) copy <from> <to> <key> - Copy word (with translations) to another dictionary\n";
  out << "13) intersect <newdict> <dict1> <dict2> - Add common words of both to a new dictionary\n";
  out << "14) difference <newdict> <dict1> <dict2> - Add words from dict1 missing in dict2 to a new\n";
  out << "15) unique <newdict> <dict1> <dict2> - Add words unique to either to a new dictionary\n";
  out << "16) prefix <newdict> <dict> <prefix> - Add all words with prefix to a new dictionary\n";
  out << "help - Show this help\n";
}
