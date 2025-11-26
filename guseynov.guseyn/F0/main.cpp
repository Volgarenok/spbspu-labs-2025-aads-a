#include <iostream>
#include <sstream>
#include <limits>
#include "book_system.hpp"

using namespace guseynov;

void parseAndExecute(BookSystem& system, const std::string& commandLine)
{
  std::istringstream iss(commandLine);
  std::string command;
  iss >> command;
  if (command == "help") {
    system.help();
  } else if (command == "addbook") {
    std::string bookName;
    iss >> bookName;
    system.addbook(bookName);
  } else if (command == "add") {
    std::string bookName;
    std::string key;
    std::string record;
    iss >> bookName >> key;
    std::getline(iss, record);
    size_t firstQuote = record.find('"');
    size_t lastQuote = record.rfind('"');
    if (firstQuote != std::string::npos && lastQuote != std::string::npos && firstQuote != lastQuote) {
      std::string recordText = record.substr(firstQuote + 1, lastQuote - firstQuote - 1);
      List< std::string > colors;
      std::string color;
      while (iss >> color) {
        colors.push_back(color);
      }
      system.add(bookName, key, recordText, colors);
    } else {
      std::cout << "Error: Record must be enclosed in quotes\n";
    }
  } else if (command == "rewrite") {
    std::string bookName;
    std::string key;
    std::string newRecord;
    iss >> bookName >> key;
    std::getline(iss, newRecord);
    size_t firstQuote = newRecord.find('"');
    size_t lastQuote = newRecord.rfind('"');
    if (firstQuote != std::string::npos && lastQuote != std::string::npos && firstQuote != lastQuote) {
      std::string recordText = newRecord.substr(firstQuote + 1, lastQuote - firstQuote - 1);
      system.rewrite(bookName, key, recordText);
    } else {
      std::cout << "Error: Record must be enclosed in quotes\n";
    }
  } else if (command == "find") {
    std::string bookName;
    std::string word;
    iss >> bookName >> word;
    system.find(bookName, word);
  } else if (command == "findbytags") {
    std::string bookName;
    std::string logic;
    iss >> bookName >> logic;
    List< std::string > colors;
    std::string color;
    while (iss >> color) {
      colors.push_back(color);
    }
    system.findbytags(bookName, logic, colors);
  } else if (command == "read") {
    std::string bookName;
    std::string key;
    iss >> bookName >> key;
    system.read(bookName, key);
  } else if (command == "readbook") {
    std::string bookName;
    iss >> bookName;
    system.readbook(bookName);
  } else if (command == "combinebooks") {
    std::string book1;
    std::string book2;
    std::string result;
    iss >> book1 >> book2 >> result;
    system.combinebooks(book1, book2, result);
  } else if (command == "movetags") {
    std::string source;
    std::string target;
    iss >> source >> target;
    List< std::string > colors;
    std::string color;
    while (iss >> color) {
      colors.push_back(color);
    }
    system.movetags(source, target, colors);
  } else if (command == "edittags") {
    std::string bookName;
    std::string key;
    std::string mode;
    iss >> bookName >> key >> mode;
    List< std::string > colors;
    std::string color;
    while (iss >> color) {
      colors.push_back(color);
    }
    system.edittags(bookName, key, mode, colors);
  } else if (command == "deletebytags") {
    std::string bookName;
    std::string logic;
    iss >> bookName >> logic;
    List< std::string > colors;
    std::string color;
    while (iss >> color) {
      colors.push_back(color);
    }
    system.deletebytags(bookName, logic, colors);
  } else if (command == "replacetags") {
    std::string bookName;
    std::string oldColor;
    std::string newColor;
    iss >> bookName >> oldColor >> newColor;
    system.replacetags(bookName, oldColor, newColor);
  } else if (command == "listallwith") {
    std::string key;
    iss >> key;
    system.listallwith(key);
  } else if (command == "deletebook") {
    std::string bookName;
    iss >> bookName;
    system.deletebook(bookName);
  } else if (command == "delete") {
    std::string bookName;
    std::string key;
    iss >> bookName >> key;
    system.deleteRecord(bookName, key);
  } else if (command == "listallwithcolor") {
    std::string color;
    iss >> color;
    system.listallwithcolor(color);
  } else if (command == "statistics") {
    system.statistics();
  } else if (!command.empty()) {
    std::cout << "Unknown command: " << command << "\n";
    std::cout << "Type 'help' for list of commands\n";
  }
}

int main()
{
  BookSystem system;
  std::string commandLine;
  std::cout << "Book System Management Console\n";
  std::cout << "Type 'help' for list of commands or 'exit' to quit\n";
  std::cout << "Valid colors: red, blue, green\n\n";
  while (true) {
    std::cout << "> ";
    std::getline(std::cin, commandLine);
    if (commandLine == "exit") {
      break;
    }
    parseAndExecute(system, commandLine);
  }
  std::cout << "Goodbye!\n";
  return 0;
}
