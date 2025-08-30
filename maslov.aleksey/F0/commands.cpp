#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <list/list.hpp>

namespace
{
  bool isAlpha(char c)
  {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
  }

  char toLowercase(char c)
  {
    if (c >= 'A' && c <= 'Z')
    {
      return c + ('a' - 'A');
    }
    return c;
  }

  void selectionSort(maslov::FwdList< std::pair< std::string, int > > & list, const std::string & order)
  {
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      auto targetIt = it;
      for (auto curr = it; curr != list.end(); ++curr)
      {
        if (order == "descending")
        {
          if (curr->second > targetIt->second)
          {
            targetIt = curr;
          }
        }
        else if (order == "ascending")
        {
          if (curr->second < targetIt->second)
          {
            targetIt = curr;
          }
        }
      }
      if (targetIt != it)
      {
        std::swap(*it, *targetIt);
      }
    }
  }
}

void maslov::createDictionary(std::istream & in, Dicts & dicts)
{
  std::string dictName;
  in >> dictName;
  if (dicts.find(dictName) != dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  dicts[dictName] = Dict{};
}

void maslov::showDictionary(std::ostream & out, const Dicts & dicts)
{
  if (dicts.empty())
  {
    out << '\n';
  }
  for (auto it = dicts.cbegin(); it != dicts.cend(); it++)
  {
    out << it->first << '\n';
  }
}

void maslov::loadText(std::istream & in, Dicts & dicts)
{
  std::string filename, dictName;
  in >> dictName >> filename;
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  auto it = dicts.find(dictName);
  if (it == dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  std::string word;
  while (file >> word)
  {
    std::string realWord;
    for (char c: word)
    {
      if (isAlpha(c))
      {
        realWord += toLowercase(c);
      }
    }
    if (!realWord.empty())
    {
      it->second[realWord] += 1;
    }
  }
}

void maslov::unionDictionary(std::istream & in, Dicts & dicts)
{
  std::string dict1, dict2, resultName;
  in >> resultName >> dict1 >> dict2;
  auto it1 = dicts.find(dict1);
  auto it2 = dicts.find(dict2);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  if (dicts.find(resultName) != dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  auto & resultDict = dicts[resultName];
  resultDict = it1->second;
  for (auto it = it2->second.begin(); it != it2->second.end(); it++)
  {
    resultDict[it->first] += it->second;
  }
}

void maslov::intersectDictionary(std::istream & in, Dicts & dicts)
{
  std::string dict1, dict2, resultName;
  in >> resultName >> dict1 >> dict2;
  auto it1 = dicts.find(dict1);
  auto it2 = dicts.find(dict2);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  if (dicts.find(resultName) != dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  auto & resultDict = dicts[resultName];
  for (auto it = it1->second.begin(); it != it1->second.end(); it++)
  {
    auto wordIt = it2->second.find(it->first);
    if (wordIt != it2->second.end())
    {
      resultDict[it->first] = std::min(it->second, wordIt->second);
    }
  }
}

void maslov::copyDictionary(std::istream & in, Dicts & dicts)
{
  std::string resultName, dictName;
  in >> resultName >> dictName;
  auto it = dicts.find(dictName);
  if (it == dicts.end() || dicts.find(resultName) != dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  dicts[resultName] = it->second;
}

void maslov::addWord(std::istream & in, Dicts & dicts)
{
  std::string dictName, wordName, frequency;
  in >> dictName >> wordName >> frequency;
  auto it = dicts.find(dictName);
  if (it == dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  if (it->second.find(wordName) != it->second.end())
  {
    throw std::runtime_error("<INVALID WORD>");
  }
  int num = std::stoi(frequency);
  if (num <= 0)
  {
    throw std::runtime_error("<INVALID NUMBER>");
  }
  it->second[wordName] = num;
}

void maslov::printSize(std::istream & in, std::ostream & out, const Dicts & dicts)
{
  std::string dictName;
  in >> dictName;
  auto it = dicts.find(dictName);
  if (it == dicts.cend())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  out << it->second.size() << '\n';
}

void maslov::cleanWord(std::istream & in, Dicts & dicts)
{
  std::string dictName, wordName;
  in >> dictName >> wordName;
  auto it = dicts.find(dictName);
  if (it == dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  if (it->second.find(wordName) == it->second.end())
  {
    throw std::runtime_error("<INVALID WORD>");
  }
  it->second.erase(wordName);
}

void maslov::cleanDictionary(std::istream & in, Dicts & dicts)
{
  std::string dictName;
  in >> dictName;
  auto it = dicts.find(dictName);
  if (it == dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  dicts.erase(dictName);
}

void maslov::printTopRare(std::istream & in, std::ostream & out, const Dicts & dicts, const std::string & order)
{
  std::string dictName;
  size_t number;
  in >> dictName >> number;
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.cend())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  if (number == 0 || number > dictIt->second.size())
  {
    throw std::runtime_error("<INVALID NUMBER>");
  }
  maslov::FwdList< std::pair< std::string, int > > words;
  for (auto it = dictIt->second.cbegin(); it != dictIt->second.cend(); it++)
  {
    words.pushFront({it->first, it->second});
  }
  selectionSort(words, order);
  auto it = words.begin();
  for (size_t i = 0; i < number && it != words.end(); i++, it++)
  {
    out << it->first << ' ' << it->second << '\n';
  }
}

void maslov::printFrequency(std::istream & in, std::ostream & out, const Dicts & dicts)
{
  std::string dictName, wordName;
  in >> dictName >> wordName;
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.cend())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  if (dictIt->second.find(wordName) == dictIt->second.cend())
  {
    throw std::runtime_error("<INVALID WORD>");
  }
  out << dictIt->second.at(wordName) << '\n';
}

void maslov::createWordRange(std::istream & in, Dicts & dicts)
{
  std::string resultName, dictName;
  int freq1, freq2;
  in >> resultName >> dictName >> freq1 >> freq2;
  if (freq2 < freq1)
  {
    throw std::runtime_error("<INVALID INTERVAL>");
  }
  auto resultIt = dicts.find(resultName);
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end() || resultIt != dicts.end())
  {
    throw std::runtime_error("<INVALID DICTIONARY>");
  }
  auto & resultDict = dicts[resultName];
  for (auto it = dictIt->second.cbegin(); it != dictIt->second.cend(); it++)
  {
    if (it->second >= freq1 && it->second <= freq2)
    {
      resultDict[it->first] = it->second;
    }
  }
  if (resultDict.empty())
  {
    dicts.erase(resultName);
    throw std::runtime_error("<EMPTY INTERVAL>");
  }
}

void maslov::saveDictionaries(std::istream & in, const Dicts & dicts)
{
  std::string fileName;
  in >> fileName;
  std::ofstream file(fileName);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  file << dicts.size() << '\n';
  for (auto dictIt = dicts.cbegin(); dictIt != dicts.cend(); dictIt++)
  {
    file << dictIt->first << ' ' << dictIt->second.size() << '\n';
    for (auto it = dictIt->second.cbegin(); it != dictIt->second.cend(); it++)
    {
      file << it->first << ' ' << it->second << '\n';
    }
  }
}

void maslov::loadFileCommand(std::istream & in, Dicts & dicts)
{
  std::string filename;
  in >> filename;
  loadFile(filename, dicts);
}

void maslov::loadFile(const std::string & filename, Dicts & dicts)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  size_t dictCount;
  file >> dictCount;
  for (size_t i = 0; i < dictCount; ++i)
  {
    std::string dictName;
    size_t wordCount;
    file >> dictName >> wordCount;
    Dict & currDict = dicts[dictName];
    std::string word;
    int freq = 0;
    for (size_t j = 0; j < wordCount; ++j)
    {
      file >> word >> freq;
      currDict[word] += freq;
    }
  }
}

void maslov::printHelp(std::ostream & out)
{
  out << std::left;
  out << "Available commands:\n" << '\n';
  constexpr size_t cmdWidth = 50;
  constexpr size_t numWidth = 4;
  out << std::setw(numWidth) << "1." << std::setw(cmdWidth);
  out << "createdict <dict_name>" << "create a new dictionary\n";

  out << std::setw(numWidth) << "2." << std::setw(cmdWidth);
  out << "showdicts" << "shows all dictionary names\n";

  out << std::setw(numWidth) << "3." << std::setw(cmdWidth);
  out << "loadtext <dict_name> <file>" << "upload text from a file to the dictionary\n";

  out << std::setw(numWidth) << "4." << std::setw(cmdWidth);
  out << "union <result> <dict1> <dict2>" << "creates a new dictionary that combines the other two\n";

  out << std::setw(numWidth) << "5." << std::setw(cmdWidth);
  out << "intersect <result> <dict1> <dict2>" << "creates a new dictionary that intersects the other two\n";

  out << std::setw(numWidth) << "6." << std::setw(cmdWidth);
  out << "copy <result> <dict>" << "copies dictionary data\n";

  out << std::setw(numWidth) << "7." << std::setw(cmdWidth);
  out << "addword <dict_name> <word> <frequency>" << "adding a word to the dictionary\n";

  out << std::setw(numWidth) << "8." << std::setw(cmdWidth);
  out << "size <dict_name>" << "print the number of words in the dictionary\n";

  out << std::setw(numWidth) << "9." << std::setw(cmdWidth);
  out << "cleanword <dict_name> <word>" << "deleting a word from the dictionary\n";

  out << std::setw(numWidth) << "10." << std::setw(cmdWidth);
  out << "cleandict <dict_name>" << "deleting all words from the dictionary\n";

  out << std::setw(numWidth) << "11." << std::setw(cmdWidth);
  out << "top <dict_name> <number>" << "print the most frequently used words\n";

  out << std::setw(numWidth) << "12." << std::setw(cmdWidth);
  out << "rare <dict_name> <number>" << "print the least frequently used words\n";

  out << std::setw(numWidth) << "13." << std::setw(cmdWidth);
  out << "frequency <dict_name> <word>" << "print the word frequency\n";

  out << std::setw(numWidth) << "14." << std::setw(cmdWidth);
  out << "wordrange <result> <dict_name> <freq1> <freq2>";
  out << "creates a dictionary of words whose frequencies are in the range\n";

  out << std::setw(numWidth) << "15." << std::setw(cmdWidth);
  out << "loadfile <file>" << "loads all dictionaries\n";

  out << std::setw(numWidth) << "16." << std::setw(cmdWidth);
  out << "save <file>" << "saves all dictionaries to a file\n";
}
