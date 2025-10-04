#ifndef INDEX_HPP
#define INDEX_HPP
#include <set>
#include <string>
#include <vector>
#include <functional>
#include <binaryTree.hpp>

namespace krylov
{
  using WordReferenceMap = BiTree< std::string, std::set< std::size_t >, std::less< std::string > >;
  struct IndexEntry
  {
    const std::string& word;
    const std::set< std::size_t >& lineNumbers;
    IndexEntry(const std::pair< const std::string, std::set< std::size_t > >& entry);
  };

  struct DocumentIndex
  {
    WordReferenceMap wordReferences;
    std::vector< std::string > textLines;
    std::string sourceFileName;
  };

  void addWordToIndex(WordReferenceMap& index, const std::string& word, std::size_t lineNumber);
  void buildIndexFromText(WordReferenceMap& index, const std::vector< std::string >& textLines);
  std::vector<std::string> splitTextIntoWords(const std::string& text);
  bool isWhitespace(char c);
  std::ostream& operator<<(std::ostream& out, const std::set< std::size_t >& lineNumbers);
  std::ostream& operator<<(std::ostream& out, const IndexEntry& entry);
}

#endif
