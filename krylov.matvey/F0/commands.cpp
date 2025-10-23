#include "commands.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>

namespace krylov
{
  CommandProcessor::CommandProcessor(std::ostream& output) : out_(output)
  {
    initializeCommands();
  }

  void CommandProcessor::initializeCommands()
  {
    commands["create"] = &CommandProcessor::createIndex;
    commands["add"] = &CommandProcessor::addText;
    commands["printtext"] = &CommandProcessor::printText;
    commands["printindex"] = &CommandProcessor::printIndex;
    commands["find"] = &CommandProcessor::findWord;
    commands["zip"] = &CommandProcessor::zipIndexes;
    commands["weave"] = &CommandProcessor::weaveIndexes;
    commands["intersect"] = &CommandProcessor::intersectIndexes;
    commands["diff"] = &CommandProcessor::diffIndexes;
    commands["remove"] = &CommandProcessor::removeWord;
    commands["delete"] = &CommandProcessor::deleteIndex;
    commands["list"] = &CommandProcessor::listIndexes;
  }

  std::vector< std::string > CommandProcessor::parseCommandLine(const std::string& line)
  {
    std::vector< std::string > args;
    std::string currentArg;
    bool inQuotes = false;
    for (std::size_t i = 0; i <  line.length(); i++)
    {
      char c = line[i];
      if (c == '"')
      {
        inQuotes = !inQuotes;
      }
      else if (isWhitespace(c) && !inQuotes)
      {
        if (!currentArg.empty())
        {
          args.push_back(currentArg);
          currentArg.clear();
        }
      }
      else
      {
        currentArg += c;
      }
    }
    if (!currentArg.empty())
    {
      args.push_back(currentArg);
    }
    return args;
  }

  void CommandProcessor::execute(const std::string& line)
  {
    std::vector< std::string > args = parseCommandLine(line);
    if (args.empty())
    {
      throw std::invalid_argument("Empty command");
    }
    const std::string& command = args[0];
    auto commandIt = commands.find(command);
    if (commandIt == commands.cend())
    {
      throw std::invalid_argument("Unknown command");
    }
    std::vector< std::string > commandArgs(args.cbegin() + 1, args.cend());
    (this->*(commandIt->second))(commandArgs);
  }

  void CommandProcessor::createIndex(const std::vector< std::string >& args)
  {
    if (args.size() != 1)
    {
      throw std::invalid_argument("Invalid number of arguments for create");
    }
    const std::string& indexName = args[0];
    if (indexes_.find(indexName) != indexes_.cend())
    {
      throw std::invalid_argument("Index already exists");
    }
    DocumentIndex newIndex;
    indexes_[indexName] = newIndex;
  }

  void CommandProcessor::addText(const std::vector< std::string >& args)
  {
    if (args.size() < 2)
    {
      throw std::invalid_argument("Invalid number of arguments for add");
    }
    const std::string& indexName = args[0];
    DocumentIndex& docIndex = indexes_[indexName];
    std::string text;
    for (std::size_t i = 1; i < args.size(); i++)
    {
      if (i > 1) text += " ";
      text += args[i];
    }
    docIndex.textLines.push_back(text);
    std::size_t lineNumber = docIndex.textLines.size();
    std::vector< std::string > words = splitTextIntoWords(text);
    for (std::size_t i = 0; i < words.size(); i++)
    {
      addWordToIndex(docIndex.wordReferences, words[i], lineNumber);
    }
  }

  void CommandProcessor::printText(const std::vector< std::string >& args)
  {
    if (args.size() != 1)
    {
      throw std::invalid_argument("Invalid number of arguments for printtext");
    }
    const std::string& indexName = args[0];
    auto indexIt = indexes_.find(indexName);
    if (indexIt == indexes_.cend() || indexIt->second.textLines.empty())
    {
      throw std::invalid_argument("Index not found or empty");
    }
    const std::vector< std::string >& textLines = indexIt->second.textLines;
    for (std::size_t i = 0; i < textLines.size(); i++)
    {
      out_ << (i + 1) << ": " << textLines[i] << "\n";
    }
  }

  void CommandProcessor::printIndex(const std::vector< std::string >& args)
  {
    if (args.size() != 1)
    {
      throw std::invalid_argument("Invalid number of arguments for printindex");
    }
    const std::string& indexName = args[0];
    auto indexIt = indexes_.find(indexName);
    if (indexIt == indexes_.cend() || indexIt->second.wordReferences.empty())
    {
      throw std::invalid_argument("Index not found or empty");
    }
    const WordReferenceMap& index = indexIt->second.wordReferences;
    for (auto it = index.cbegin(); it != index.cend(); it++)
    {
      IndexEntry entry(*it);
      out_ << entry << "\n";
    }
  }

  void CommandProcessor::findWord(const std::vector< std::string >& args)
  {
    if (args.size() != 2)
    {
      throw std::invalid_argument("Invalid number of arguments for find");
    }
    const std::string& indexName = args[0];
    const std::string& word = args[1];
    auto indexIt = indexes_.find(indexName);
    if (indexIt == indexes_.cend())
    {
      throw std::invalid_argument("Index not found");
    }
    const WordReferenceMap& index = indexIt->second.wordReferences;
    auto wordIt = index.find(word);
    if (wordIt == index.cend())
    {
      throw std::invalid_argument("Word not found in index");
    }
    IndexEntry entry(*wordIt);
    out_ << entry << "\n";
  }

  void CommandProcessor::zipIndexes(const std::vector< std::string >& args)
  {
    if (args.size() != 3)
    {
      throw std::invalid_argument("Invalid number of arguments for zip");
    }
    const std::string& newIndex = args[0];
    const std::string& index1 = args[1];
    const std::string& index2 = args[2];
    if (indexes_.find(newIndex) != indexes_.cend() ||
          indexes_.find(index1) == indexes_.cend() ||
            indexes_.find(index2) == indexes_.cend())
    {
      throw std::invalid_argument("Invalid index names for zip operation");
    }
    const std::vector< std::string >& text1 = indexes_[index1].textLines;
    const std::vector< std::string >& text2 = indexes_[index2].textLines;
    std::vector< std::string > newText;
    std::size_t maxSize = text1.size() > text2.size() ? text1.size() : text2.size();
    for (std::size_t i = 0; i < maxSize; i++)
    {
      std::string line;
      if (i < text1.size())
      {
        line += text1[i];
      }
      if (i < text2.size())
      {
        line += text2[i];
      }
      newText.push_back(line);
    }
    DocumentIndex result;
    result.textLines = newText;
    buildIndexFromText(result.wordReferences, newText);
    indexes_[newIndex] = result;
  }

  void CommandProcessor::weaveIndexes(const std::vector< std::string >& args)
  {
    if (args.size() != 3)
    {
      throw std::invalid_argument("Invalid number of arguments for weave");
    }
    const std::string& newIndex = args[0];
    const std::string& index1 = args[1];
    const std::string& index2 = args[2];
    if (indexes_.find(newIndex) != indexes_.cend() ||
          indexes_.find(index1) == indexes_.cend() ||
            indexes_.find(index2) == indexes_.cend())
    {
      throw std::invalid_argument("Invalid index names for weave operation");
    }
    const std::vector< std::string >& text1 = indexes_[index1].textLines;
    const std::vector< std::string >& text2 = indexes_[index2].textLines;
    std::vector< std::string > newText;
    std::size_t i = 0;
    std::size_t j = 0;
    while (i < text1.size() || j < text2.size())
    {
      if (i < text1.size())
      {
        newText.push_back(text1[i]);
        i++;
      }
      if (j < text2.size())
      {
        newText.push_back(text2[j]);
        j++;
      }
    }
    DocumentIndex result;
    result.textLines = newText;
    buildIndexFromText(result.wordReferences, newText);
    indexes_[newIndex] = result;
  }

  void CommandProcessor::intersectIndexes(const std::vector< std::string >& args)
  {
    if (args.size() != 3)
    {
      throw std::invalid_argument("Invalid number of arguments for intersect");
    }
    const std::string& newIndex = args[0];
    const std::string& index1 = args[1];
    const std::string& index2 = args[2];
    if (indexes_.find(newIndex) != indexes_.cend() ||
        indexes_.find(index1) == indexes_.cend() ||
        indexes_.find(index2) == indexes_.cend())
    {
      throw std::invalid_argument("Invalid index names for intersect operation");
    }
    const WordReferenceMap& idx1 = indexes_[index1].wordReferences;
    const WordReferenceMap& idx2 = indexes_[index2].wordReferences;
    WordReferenceMap tempResult;
    for (auto it1 = idx1.cbegin(); it1 != idx1.cend(); it1++)
    {
      auto it2 = idx2.find(it1->first);
      if (it2 != idx2.cend())
      {
        std::set< std::size_t > mergedLines;
        for (auto lineIt = it1->second.cbegin(); lineIt != it1->second.cend(); lineIt++)
        {
          mergedLines.insert(*lineIt);
        }
        for (auto lineIt = it2->second.cbegin(); lineIt != it2->second.cend(); lineIt++)
        {
          mergedLines.insert(*lineIt);
        }
        tempResult[it1->first] = mergedLines;
      }
    }
    if (tempResult.empty())
    {
      throw std::invalid_argument("No intersection found between indexes");
    }
    DocumentIndex& result = indexes_[newIndex];
    result.wordReferences = tempResult;
  }

  void CommandProcessor::diffIndexes(const std::vector< std::string >& args)
  {
    if (args.size() != 3)
    {
      throw std::invalid_argument("Invalid number of arguments for diff");
    }
    const std::string& newIndex = args[0];
    const std::string& index1 = args[1];
    const std::string& index2 = args[2];
    if (indexes_.find(newIndex) != indexes_.cend() ||
          indexes_.find(index1) == indexes_.cend() ||
            indexes_.find(index2) == indexes_.cend())
    {
      throw std::invalid_argument("Invalid index names for diff operation");
    }
    const WordReferenceMap& idx1 = indexes_[index1].wordReferences;
    const WordReferenceMap& idx2 = indexes_[index2].wordReferences;
    WordReferenceMap tempResult;
    for (auto it = idx1.cbegin(); it != idx1.cend(); it++)
    {
      if (idx2.find(it->first) == idx2.cend())
      {
        tempResult[it->first] = it->second;
      }
    }
    DocumentIndex& result = indexes_[newIndex];
    result.wordReferences = tempResult;
  }

  void CommandProcessor::removeWord(const std::vector< std::string >& args)
  {
    if (args.size() != 2)
    {
      throw std::invalid_argument("Invalid number of arguments for remove");
    }
    const std::string& indexName = args[0];
    const std::string& word = args[1];
    DocumentIndex& docIndex = indexes_[indexName];
    WordReferenceMap& wordRefs = docIndex.wordReferences;
    auto wordIt = wordRefs.find(word);
    if (wordIt == wordRefs.end())
    {
      throw std::invalid_argument("Word not found in index");
    }
    wordIt->second.clear();
  }

  void CommandProcessor::deleteIndex(const std::vector< std::string >& args)
  {
    if (args.size() != 1)
    {
      throw std::invalid_argument("Invalid number of arguments for delete");
    }
    const std::string& indexName = args[0];
    DocumentIndex& docIndex = indexes_[indexName];
    docIndex.wordReferences.clear();
    docIndex.textLines.clear();
    docIndex.sourceFileName.clear();
  }

  void CommandProcessor::listIndexes(const std::vector< std::string >& args)
  {
    if (!args.empty())
    {
      throw std::invalid_argument("Invalid number of arguments for list");
    }
    for (auto it = indexes_.cbegin(); it != indexes_.cend(); it++)
    {
      out_ << it->first << "\n";
    }
  }
}
