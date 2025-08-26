#include "commands.hpp"
#include <fstream>
#include <string>
#include <numeric>
#include <iomanip>

namespace
{
  using constWord  = const std::pair< std::string, mozhegova::Xrefs >;
  using Word = std::pair< const std::string, mozhegova::Xrefs >;

  struct PrintWordPos
  {
    std::ostream & out;
    void operator()(const std::pair< size_t, size_t > & pos) const
    {
      out << " (" << pos.first << ", " << pos.second << ')';
    }
  };

  struct PrintWords
  {
    std::ostream & out;
    size_t width;
    void operator()(constWord & word) const
    {
      out << std::left << std::setw(width) << word.first;
      for (size_t i = 0; i < word.second.size(); ++i)
      {
        PrintWordPos{out}(word.second[i]);
      }
      out << '\n';
    }
  };

  void bubbleSort(mozhegova::DynamicArray< std::pair< mozhegova::WordPos, std::string > > & arr)
  {
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; i++)
    {
      bool swapped = false;
      for (size_t j = 0; j < n - i - 1; j++)
      {
        if (arr[j].first > arr[j + 1].first)
        {
          std::pair< mozhegova::WordPos, std::string > temp = arr[j];
          arr[j] = arr[j + 1];
          arr[j + 1] = temp;
          swapped = true;
        }
      }
      if (swapped == false)
      {
        break;
      }
    }
  }

  std::string reconstructText(const mozhegova::Text & text)
  {
    mozhegova::DynamicArray< std::pair< mozhegova::WordPos, std::string > > sortedWords;
    for (auto it1 = text.cbegin(); it1 != text.cend(); ++it1)
    {
      for (size_t i = 0; i < it1->second.size(); ++i)
      {
        sortedWords.push_back({it1->second[i], it1->first});
      }
    }
    bubbleSort(sortedWords);
    if (sortedWords.empty())
    {
      return "";
    }
    std::string result;
    for (size_t i = 0; i < sortedWords.size(); ++i)
    {
      result += sortedWords[i].second;
      if (i < sortedWords.size() - 1)
      {
        if (sortedWords[i].first.first != sortedWords[i + 1].first.first)
        {
          result += '\n';
        }
        else
        {
          result += ' ';
        }
      }
    }
    return result;
  }

  size_t getCountWords(const mozhegova::Text & text)
  {
    size_t count = 0;
    for (auto it1 = text.cbegin(); it1 != text.cend(); ++it1)
    {
      count += it1->second.size();
    }
    return count;
  }

  struct PrintText
  {
    std::ostream & out;
    void operator()(const std::pair< std::string, mozhegova::Text > & text) const
    {
      out << text.first << ' ';
      out << getCountWords(text.second) << '\n';
      out << reconstructText(text.second) << '\n';
    }
  };

  size_t getMaxLineNumWord(constWord & word)
  {
    mozhegova::Xrefs xrefs = word.second;
    if (xrefs.empty())
    {
      return 0;
    }
    size_t maxNumWord = 1;
    for (size_t i = 0; i < word.second.size(); ++i)
    {
      if (maxNumWord < word.second[i].first)
      {
        maxNumWord = word.second[i].first;
      }
    }
    return maxNumWord;
  }

  size_t getMaxLineNum(const mozhegova::Text & text)
  {
    size_t maxLineNum = 1;
    for (auto it1 = text.cbegin(); it1 != text.cend(); ++it1)
    {
      if (maxLineNum < getMaxLineNumWord(*it1))
      {
        maxLineNum = getMaxLineNumWord(*it1);
      }
    }
    return maxLineNum;
  }

  size_t getMaxNumWord(constWord & word)
  {
    mozhegova::Xrefs xrefs = word.second;
    if (xrefs.empty())
    {
      return 0;
    }
    size_t maxNumWord = 1;
    for (size_t i = 0; i < word.second.size(); ++i)
    {
      if (maxNumWord < word.second[i].second)
      {
        maxNumWord = word.second[i].second;
      }
    }
    return maxNumWord;
  }

  size_t getMaxNum(const mozhegova::Text & text)
  {
    size_t maxLineNum = 1;
    for (auto it1 = text.cbegin(); it1 != text.cend(); ++it1)
    {
      if (maxLineNum < getMaxNumWord(*it1))
      {
        maxLineNum = getMaxNumWord(*it1);
      }
    }
    return maxLineNum;
  }

  mozhegova::Text extractSubstring(const mozhegova::Text & text, size_t begin, size_t end)
  {
    mozhegova::Text result;
    for (auto it1 = text.cbegin(); it1 != text.cend(); ++it1)
    {
      mozhegova::Xrefs newXrefs;
      for (size_t i = 0; i < it1->second.size(); ++i)
      {
        if (it1->second[i].first >= begin && it1->second[i].first < end)
        {
          newXrefs.push_back(it1->second[i]);
        }
      }
      if (!newXrefs.empty())
      {
        result[it1->first] = newXrefs;
      }
    }
    return result;
  }

  void insertTextTo(mozhegova::Text & text1, const mozhegova::Text & text2, size_t n, size_t begin, size_t end)
  {
    mozhegova::Text temp = extractSubstring(text2, begin, end);
    for (auto it1 = text1.begin(); it1 != text1.end(); ++it1)
    {
      for (size_t i = 0; i < it1->second.size(); ++i)
      {
        if (it1->second[i].first >= n)
        {
          it1->second[i].first += (end - begin);
        }
      }
    }
    for (auto it2 = temp.cbegin(); it2 != temp.cend(); ++it2)
    {
      for (size_t i = 0; i < it2->second.size(); ++i)
      {
        text1[it2->first].push_back({it2->second[i].first + n - begin, it2->second[i].second});
      }
    }
  }

  void removeSubstring(mozhegova::Text & text, size_t begin, size_t end)
  {
    for (auto it1 = text.begin(); it1 != text.end();)
    {
      bool isDeleted = false;
      for (size_t i = 0; i < it1->second.size(); ++i)
      {
        if (it1->second[i].first >= begin && it1->second[i].first < end)
        {
          it1 = text.erase(it1);
          isDeleted = true;
          break;
        }
        else if (it1->second[i].first >= end)
        {
          it1->second[i].first -= (end - begin);
        }
      }
      if (!isDeleted)
      {
        ++it1;
      }
    }
  }
}

void mozhegova::generateLinks(std::istream & in, Texts & texts)
{
  std::string textName, fileName;
  in >> textName >> fileName;
  std::ifstream file(fileName);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  if (texts.find(textName) != texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text text{};
  size_t line = 0;
  while (!file.eof())
  {
    ++line;
    std::string word;
    size_t num = 0;
    while (file.peek() != '\n' && file >> word)
    {
      ++num;
      text[word].push_back({line, num});
    }
    file.ignore();
  }
  texts[textName] = std::move(text);
}

void mozhegova::removeLinks(std::istream & in, Texts & texts)
{
  std::string textName;
  in >> textName;
  if (texts.find(textName) == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  texts.erase(textName);
}

void mozhegova::printLinks(std::istream & in, std::ostream & out, const Texts & texts)
{
  std::string textName;
  in >> textName;
  auto it = texts.find(textName);
  if (it == texts.cend())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  const Text & text = it->second;
  auto maxWordIt = text.cbegin();
  size_t maxWordLen = maxWordIt->first.size();
  for (; maxWordIt != text.cend(); ++maxWordIt)
  {
    if (maxWordLen < maxWordIt->first.size())
    {
      maxWordLen = maxWordIt->first.size();
    }
  }
  for (auto it2 = text.cbegin(); it2 != text.cend(); ++it2)
  {
    PrintWords{out, maxWordLen + 2}(*it2);
  }
}

void mozhegova::printText(std::istream & in, std::ostream & out, const Texts & texts)
{
  std::string textName;
  in >> textName;
  auto it = texts.find(textName);
  if (it == texts.cend())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  const Text & text = it->second;
  std::string reconstructed = reconstructText(text);
  out << reconstructed << '\n';
}

void mozhegova::printTextInFile(std::istream & in, const Texts & texts)
{
  std::string fileName;
  in >> fileName;
  std::ofstream file(fileName);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  printText(in, file, texts);
}

void mozhegova::mergeTexts(std::istream & in, Texts & texts)
{
  std::string newText, textName1, textName2;
  in >> newText >> textName1 >> textName2;
  auto it1 = texts.find(textName1);
  auto it2 = texts.find(textName2);
  auto it = texts.find(newText);
  if (it1 == texts.end() || it2 == texts.end() || it != texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  const Text & text1 = it1->second;
  const Text & text2 = it2->second;
  Text temp = text1;
  size_t num = getMaxLineNum(temp) + 1;
  insertTextTo(temp, text2, num, 1, getMaxLineNum(text2) + 1);
  texts[newText] = std::move(temp);
}

void mozhegova::insertText(std::istream & in, Texts & texts)
{
  std::string textName1, textName2;
  size_t num = 0, begin = 0, end = 0;
  in >> textName1 >> num >> textName2 >> begin >> end;
  auto it1 = texts.find(textName1);
  auto it2 = texts.find(textName2);
  if (it1 == texts.end() || it2 == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & text1 = it1->second;
  const Text & text2 = it2->second;
  if ((num > 1 + getMaxLineNum(text1)) || (begin < 1) || (end > getMaxLineNum(text2) + 1))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  insertTextTo(text1, text2, num, begin, end);
}

void mozhegova::removeLines(std::istream & in, Texts & texts)
{
  std::string textName;
  size_t begin = 0, end = 0;
  in >> textName >> begin >> end;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & text = it->second;
  if ((begin < 1) || (end > getMaxLineNum(text) + 1))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  removeSubstring(text, begin, end);
}

void mozhegova::moveText(std::istream & in, Texts & texts)
{
  std::string textName1, textName2;
  size_t num = 0, begin = 0, end = 0;
  in >> textName1 >> num >> textName2 >> begin >> end;
  auto it1 = texts.find(textName1);
  auto it2 = texts.find(textName2);
  if (it1 == texts.end() || it2 == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & text1 = it1->second;
  Text & text2 = it2->second;
  if ((num > 1 + getMaxLineNum(text1)) || (begin < 1) || (end > getMaxLineNum(text2) + 1))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  insertTextTo(text1, text2, num, begin, end);
  removeSubstring(text2, begin, end);
}

void mozhegova::sideMergeTexts(std::istream & in, Texts & texts)
{
  std::string newText, textName1, textName2;
  in >> newText >> textName1 >> textName2;
  auto it1 = texts.find(textName1);
  auto it2 = texts.find(textName2);
  auto it = texts.find(newText);
  if (it1 == texts.end() || it2 == texts.end() || it != texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  const Text & text1 = it1->second;
  const Text & text2 = it2->second;
  Text temp1 = text1;
  Text temp2 = text2;
  size_t maxLines = std::max(getMaxLineNum(text1), getMaxLineNum(text2));
  size_t maxNum = getMaxNum(text1);
  for (size_t line = 1; line <= maxLines; ++line)
  {
    for (auto it1 = temp2.begin(); it1 != temp2.end(); ++it1)
    {
      for (size_t i = 0; i < it1->second.size(); ++i)
      {
        if (it1->second[i].first == line)
        {
          it1->second[i].second += maxNum;
          temp1[it1->first].push_back(it1->second[i]);
        }
      }
    }
  }
  texts[newText] = std::move(temp1);
}

void mozhegova::splitTexts(std::istream & in, Texts & texts)
{
  std::string textName, newText1, newText2;
  size_t num = 0;
  in >> textName >> num >> newText1 >> newText2;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & text = it->second;
  size_t end = getMaxLineNum(text);
  if (num > end)
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text newText = extractSubstring(text, num, end + 1);
  removeSubstring(text, num, end + 1);
  texts[newText1] = std::move(text);
  texts[newText2] = std::move(newText);
  texts.erase(textName);
}

void mozhegova::invertLines(std::istream & in, Texts & texts)
{
  std::string textName;
  in >> textName;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & text = it->second;
  size_t maxLine = getMaxLineNum(text);
  for (auto it1 = text.begin(); it1 != text.end(); ++it1)
  {
    for (size_t i = 0; i < it1->second.size(); ++i)
    {
      it1->second[i].first = maxLine - it1->second[i].first + 1;
    }
  }
}

void mozhegova::invertWords(std::istream & in, Texts & texts)
{
  std::string textName;
  in >> textName;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & text = it->second;
  size_t maxLines = getMaxLineNum(text);
  size_t maxNum = getMaxNum(text);
  for (size_t line = 1; line <= maxLines; ++line)
  {
    for (auto it1 = text.begin(); it1 != text.end(); ++it1)
    {
      for (size_t i = 0; i < it1->second.size(); ++i)
      {
        if (it1->second[i].first == line)
        {
          it1->second[i].second = maxNum - it1->second[i].second + 1;
        }
      }
    }
  }
}

void mozhegova::replaceWord(std::istream & in, Texts & texts)
{
  std::string textName, oldWord, newWord;
  in >> textName >> oldWord >> newWord;
  auto it = texts.find(textName);
  if (it == texts.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  Text & text = it->second;
  auto wordIt = text.find(oldWord);
  if (wordIt == text.end())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  text[newWord] = std::move(wordIt->second);
  text.erase(oldWord);
}

void mozhegova::save(std::istream & in, const Texts & texts)
{
  std::string fileName;
  in >> fileName;
  std::ofstream file(fileName);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  file << texts.size() << '\n';
  for (auto it = texts.cbegin(); it != texts.cend(); ++it)
  {
    PrintText{file}(*it);
  }
}

void mozhegova::loadFileCmd(std::istream & in, Texts & texts)
{
  std::string fileName;
  in >> fileName;
  loadFile(fileName, texts);
}

void mozhegova::loadFile(const std::string & fileName, Texts & texts)
{
  std::ifstream file(fileName);
  if (!file.is_open())
  {
    throw std::runtime_error("<INVALID FILE>");
  }
  size_t textsCount;
  file >> textsCount;
  for (size_t i = 0; i < textsCount; ++i)
  {
    std::string textName;
    size_t wordCount;
    file >> textName >> wordCount;
    Text & currText = texts[textName];
    std::string word;
    size_t line = 0;
    size_t j = 0;
    while (j < wordCount)
    {
      ++line;
      size_t num = 0;
      while (file.peek() != '\n' && file >> word)
      {
        ++num;
        currText[word].push_back({line, num});
        ++j;
      }
      file.ignore();
    }
  }
}

void mozhegova::printHelp(std::ostream & out)
{
  out << "Available commands:\n";
  out << "generatelinks <text name> <text file> - generate new links from file\n";
  out << "removelinks <text name> - delete text\n";
  out << "printlinks <text name> - print words from text with xref\n";
  out << "printtext <text name> - print text\n";
  out << "printtextinfile <text file> <text name> - print text in file\n";
  out << "merge <text result name> <text1 name> <text2 name> - merge two texts\n";
  out << "insert <text1 name> <num> <text2 name> <begin> <end> - insert part of the text into another\n";
  out << "removelines <text name> <begin> <end> - delete part of the text\n";
  out << "move <text1 name> <num> <text2 name> <begin> <end> - move part of the text into another\n";
  out << "sidemerge <text result name> <text1 name> <text2 name> - merge two texts along the lines\n";
  out << "split <text name> <num> <new text1 name> <new text2 name> - split the text into two texts\n";
  out << "invertlines <text name> - reverse the order of lines in the text\n";
  out << "invertwords <text name> - reverse the order of words in each line in the text\n";
  out << "replaceword <text name> <old word> <new word> - replace a word in the text with a new one \n";
  out << "save <file name> - save all texts\n";
  out << "loadfile <file name> - load the file\n";
  out << "--continue - load the saves\n";
  out << "--help - show this help\n";
}
