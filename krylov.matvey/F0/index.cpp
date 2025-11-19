#include "index.hpp"
#include <cctype>
#include <iostream>
#include <stdexcept>

namespace krylov
{
  IndexEntry::IndexEntry(const std::pair< const std::string, std::set< std::size_t > >& entry):
    word(entry.first),
    lineNumbers(entry.second)
  {}

  void addWordToIndex(WordReferenceMap& index, const std::string& word, std::size_t lineNumber)
  {
    index[word].insert(lineNumber);
  }

  bool isWhitespace(char c)
  {
    return std::isspace(static_cast< unsigned char >(c));
  }

  std::vector< std::string > splitTextIntoWords(const std::string& text)
  {
    std::vector< std::string > words;
    std::string currentWord;
    const char* ptr = text.c_str();
    while (*ptr != '\0')
    {
      if (isWhitespace(*ptr))
      {
        if (!currentWord.empty())
        {
          words.push_back(currentWord);
          currentWord.clear();
        }
      }
      else
      {
        currentWord += *ptr;
      }
      ptr++;
    }
    if (!currentWord.empty())
    {
      words.push_back(currentWord);
    }
    return words;
  }

  void buildIndexFromText(WordReferenceMap& index, const std::vector< std::string >& textLines)
  {
    for (std::size_t i = 0; i < textLines.size(); i++)
    {
      std::vector< std::string > words = splitTextIntoWords(textLines[i]);
      for (std::size_t j = 0; j < words.size(); j++)
      {
        addWordToIndex(index, words[j], i + 1);
      }
    }
  }

  std::ostream& operator<<(std::ostream& out, const std::set< std::size_t >& lineNumbers)
  {
    if (lineNumbers.empty())
    {
      return out;
    }

    auto it = lineNumbers.begin();
    out << *it;
    it++;
    while (it != lineNumbers.end())
    {
      out << ", " << *it;
      it++;
    }
    return out;
  }

  std::ostream& operator<<(std::ostream& out, const IndexEntry& entry)
  {
    out << entry.word << ": " << entry.lineNumbers;
    return out;
  }
}
