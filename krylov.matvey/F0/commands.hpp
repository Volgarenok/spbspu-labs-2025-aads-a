#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <iosfwd>
#include <vector>
#include <functional>
#include <binaryTree.hpp>
#include "index.hpp"

namespace krylov
{
  class CommandProcessor
  {
  public:
    CommandProcessor(std::ostream& output);
    void execute(const std::string& line);
  private:
    std::ostream& out_;
    BiTree< std::string, DocumentIndex, std::less< std::string > > indexes_;
    BiTree< std::string, void (CommandProcessor::*)(const std::vector< std::string >&), std::less< std::string > > commands;

    void initializeCommands();
    std::vector< std::string > parseCommandLine(const std::string& line);

    void createIndex(const std::vector< std::string >& args);
    void addText(const std::vector< std::string >& args);
    void printText(const std::vector< std::string >& args);
    void printIndex(const std::vector< std::string >& args);
    void findWord(const std::vector< std::string >& args);
    void zipIndexes(const std::vector< std::string >& args);
    void weaveIndexes(const std::vector< std::string >& args);
    void intersectIndexes(const std::vector< std::string >& args);
    void diffIndexes(const std::vector< std::string >& args);
    void removeWord(const std::vector< std::string >& args);
    void deleteIndex(const std::vector< std::string >& args);
    void listIndexes(const std::vector< std::string >& args);
  };
}

#endif
