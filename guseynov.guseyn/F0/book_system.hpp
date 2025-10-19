#ifndef BOOK_SYSTEM_HPP
#define BOOK_SYSTEM_HPP

#include <iostream>
#include <string>
#include <list.h>

namespace guseynov
{
  struct Record
  {
    std::string key;
    std::string text;
    List< std::string > tags;
    Record(const std::string& k = "", const std::string& t = ""):
      key(k),
      text(t)
    {}
    bool operator==(const Record& other) const
    {
      return key == other.key;
    }
  };

  struct Book
  {
    std::string name;
    List< Record > records;
    Book(const std::string& n = ""):
      name(n)
    {}
    bool operator==(const Book& other) const
    {
      return name == other.name;
    }
  };

  class BookSystem
  {
  public:
    void help();
    void addbook(const std::string& bookName);
    void add(const std::string& bookName, const std::string& key, const std::string& record, const List< std::string >& colors);
    void rewrite(const std::string& bookName, const std::string& key, const std::string& newRecord);
    void find(const std::string& bookName, const std::string& word);
    void findbytags(const std::string& bookName, const std::string& logic, const List< std::string >& colors);
    void read(const std::string& bookName, const std::string& key);
    void readbook(const std::string& bookName);
    void combinebooks(const std::string& bookName1, const std::string& bookName2, const std::string& resultBookName);
    void movetags(const std::string& sourceBook, const std::string& targetBook, const List< std::string >& colors);
    void edittags(const std::string& bookName, const std::string& key, const std::string& mode, const List< std::string >& colors);
    void deletebytags(const std::string& bookName, const std::string& logic, const List< std::string >& colors);
    void replacetags(const std::string& bookName, const std::string& oldColor, const std::string& newColor);
    void listallwith(const std::string& key);
    void deletebook(const std::string& bookName);
    void deleteRecord(const std::string& bookName, const std::string& key);
    void listallwithcolor(const std::string& color);
    void statistics();
  private:
    List< Book > books_;
    Book* findBook(const std::string& bookName);
    Record* findRecord(Book* book, const std::string& key);
    bool recordHasAllTags(Record* record, const List< std::string >& tags);
    bool recordHasAnyTag(Record* record, const List< std::string >& tags);
    bool containsWord(const std::string& text, const std::string& word);
    void printRecord(Record* record);
    bool isValidColor(const std::string& color);
  };

  bool BookSystem::isValidColor(const std::string& color)
  {
    return (color == "red" || color == "blue" || color == "green");
  }

  void BookSystem::help()
  {
    std::cout << "Available commands:\n";
    std::cout << "1. addbook <book_name> - create new empty book\n";
    std::cout << "2. add <book_name> <key> \"<record>\" <color_1> [<color_2> ...] - add/update record in book\n";
    std::cout << "3. rewrite <book_name> <key> \"<new_record>\" - change record text without modifying tags\n";
    std::cout << "4. find <book_name> <word> - find records containing word\n";
    std::cout << "5. findbytags <book_name> <logic> <color_1> [<color_2> ...] - find records by tags (AND/OR logic)\n";
    std::cout << "6. read <book_name> <key> - display full record information\n";
    std::cout << "7. readbook <book_name> - display all records in book\n";
    std::cout << "8. combinebooks <book_name_1> <book_name_2> <result_book_name> - combine two books into one\n";
    std::cout << "9. movetags <source_book> <target_book> <color_1> [<color_2> ...] - move records with tags between books\n";
    std::cout << "10. edittags <book_name> <key> <mode> <color_1> [<color_2> ...] - edit record tags (SET/ADD/DEL)\n";
    std::cout << "11. deletebytags <book_name> <logic> <color_1> [<color_2> ...] - delete records by tags\n";
    std::cout << "12. replacetags <book_name> <old_color> <new_color> - replace tags in entire book\n";
    std::cout << "13. listallwith <key> - display books structure with records containing tag\n";
    std::cout << "14. deletebook <book_name> - delete entire book\n";
    std::cout << "15. delete <book_name> <key> - delete specific record\n";
    std::cout << "16. listallwithcolor <color> - display books and records with specified color tag\n";
    std::cout << "17. statistics - show system statistics\n";
    std::cout << "18. help - show this help message\n";
    std::cout << "\nValid colors: red, blue, green\n";
  }

  Book* BookSystem::findBook(const std::string& bookName)
  {
    for (auto it = books_.begin(); it != books_.end(); ++it) {
      if (it->name == bookName) {
        return &(*it);
      }
    }
    return nullptr;
  }

  Record* BookSystem::findRecord(Book* book, const std::string& key)
  {
    for (auto it = book->records.begin(); it != book->records.end(); ++it) {
      if (it->key == key) {
        return &(*it);
      }
    }
    return nullptr;
  }

  bool BookSystem::recordHasAllTags(Record* record, const List< std::string >& tags)
  {
    for (auto tagIt = tags.begin(); tagIt != tags.end(); ++tagIt) {
      bool found = false;
      for (auto recordTagIt = record->tags.begin(); recordTagIt != record->tags.end(); ++recordTagIt) {
        if (*recordTagIt == *tagIt) {
          found = true;
          break;
        }
      }
      if (!found) {
        return false;
      }
    }
    return true;
  }

  bool BookSystem::recordHasAnyTag(Record* record, const List< std::string >& tags)
  {
    for (auto tagIt = tags.begin(); tagIt != tags.end(); ++tagIt) {
      for (auto recordTagIt = record->tags.begin(); recordTagIt != record->tags.end(); ++recordTagIt) {
        if (*recordTagIt == *tagIt) {
          return true;
        }
      }
    }
    return false;
  }

  bool BookSystem::containsWord(const std::string& text, const std::string& word)
  {
    size_t pos = 0;
    while ((pos = text.find(word, pos)) != std::string::npos) {
      if ((pos == 0 || text[pos - 1] == ' ') &&
          (pos + word.length() == text.length() || text[pos + word.length()] == ' ')) {
        return true;
      }
      pos += word.length();
    }
    return false;
  }

  void BookSystem::printRecord(Record* record)
  {
    std::cout << "Key: " << record->key << "\n";
    std::cout << "Text: " << record->text << "\n";
    std::cout << "Tags: ";
    for (auto it = record->tags.begin(); it != record->tags.end(); ++it) {
      std::cout << *it << " ";
    }
    std::cout << "\n";
  }

  void BookSystem::addbook(const std::string& bookName)
  {
    if (findBook(bookName) != nullptr) {
      std::cout << "Book '" << bookName << "' already exists.\n";
      return;
    }
    Book newBook(bookName);
    books_.push_back(newBook);
    std::cout << "Book '" << bookName << "' created successfully.\n";
  }

  void BookSystem::add(const std::string& bookName, const std::string& key, const std::string& record, const List< std::string >& colors)
  {
    Book* book = findBook(bookName);
    if (!book) {
      std::cout << "Book '" << bookName << "' not found.\n";
      return;
    }
    if (colors.empty()) {
      std::cout << "Error: At least one color tag is required.\n";
      return;
    }
    for (auto it = colors.begin(); it != colors.end(); ++it) {
      if (!isValidColor(*it)) {
        std::cout << "Error: Invalid color '" << *it << "'. Valid colors are: red, blue, green.\n";
        return;
      }
    }
    Record* existingRecord = findRecord(book, key);
    if (existingRecord) {
      existingRecord->text = record;
      existingRecord->tags.clear();
      for (auto it = colors.begin(); it != colors.end(); ++it) {
        existingRecord->tags.push_back(*it);
      }
      std::cout << "Record '" << key << "' updated in book '" << bookName << "'.\n";
    } else {
      Record newRecord(key, record);
      for (auto it = colors.begin(); it != colors.end(); ++it) {
        newRecord.tags.push_back(*it);
      }
      book->records.push_back(newRecord);
      std::cout << "Record '" << key << "' added to book '" << bookName << "'.\n";
    }
  }

  void BookSystem::rewrite(const std::string& bookName, const std::string& key, const std::string& newRecord)
  {
    Book* book = findBook(bookName);
    if (!book) {
      std::cout << "Book '" << bookName << "' not found.\n";
      return;
    }
    Record* record = findRecord(book, key);
    if (!record) {
      std::cout << "Record '" << key << "' not found in book '" << bookName << "'.\n";
      return;
    }
    record->text = newRecord;
    std::cout << "Record '" << key << "' updated in book '" << bookName << "'.\n";
  }

  void BookSystem::find(const std::string& bookName, const std::string& word)
  {
    Book* book = findBook(bookName);
    if (!book) {
      std::cout << "Book '" << bookName << "' not found.\n";
      return;
    }
    bool found = false;
    for (auto it = book->records.begin(); it != book->records.end(); ++it) {
      if (containsWord(it->text, word)) {
        if (!found) {
          std::cout << "Records containing word '" << word << "':\n";
          found = true;
        }
        printRecord(&(*it));
        std::cout << "---\n";
      }
    }
    if (!found) {
      std::cout << "No records containing word '" << word << "' found.\n";
    }
  }

  void BookSystem::findbytags(const std::string& bookName, const std::string& logic, const List< std::string >& colors)
  {
    Book* book = findBook(bookName);
    if (!book) {
      std::cout << "Book '" << bookName << "' not found.\n";
      return;
    }
    bool found = false;
    for (auto it = book->records.begin(); it != book->records.end(); ++it) {
      bool match = false;
      if (logic == "AND") {
        match = recordHasAllTags(&(*it), colors);
      } else if (logic == "OR") {
        match = recordHasAnyTag(&(*it), colors);
      } else {
        std::cout << "Invalid logic operator. Use AND or OR.\n";
        return;
      }
      if (match) {
        if (!found) {
          std::cout << "Records found with tags (logic: " << logic << "):\n";
          found = true;
        }
        printRecord(&(*it));
        std::cout << "---\n";
      }
    }
    if (!found) {
      std::cout << "No records found with specified tags.\n";
    }
  }

  void BookSystem::read(const std::string& bookName, const std::string& key)
  {
    Book* book = findBook(bookName);
    if (!book) {
      std::cout << "Book '" << bookName << "' not found.\n";
      return;
    }
    Record* record = findRecord(book, key);
    if (!record) {
      std::cout << "Record '" << key << "' not found in book '" << bookName << "'.\n";
      return;
    }
    std::cout << "Record details:\n";
    printRecord(record);
  }

  void BookSystem::readbook(const std::string& bookName)
  {
    Book* book = findBook(bookName);
    if (!book) {
      std::cout << "Book '" << bookName << "' not found.\n";
      return;
    }
    if (book->records.empty()) {
      std::cout << "Book '" << bookName << "' is empty.\n";
      return;
    }
    std::cout << "All records in book '" << bookName << "':\n";
    for (auto it = book->records.begin(); it != book->records.end(); ++it) {
      std::cout << "Key: " << it->key << " | Tags: ";
      for (auto tagIt = it->tags.begin(); tagIt != it->tags.end(); ++tagIt) {
        std::cout << *tagIt << " ";
      }
      std::cout << "\n";
    }
  }

  void BookSystem::combinebooks(const std::string& bookName1, const std::string& bookName2, const std::string& resultBookName)
  {
    Book* book1 = findBook(bookName1);
    Book* book2 = findBook(bookName2);
    if (!book1 || !book2) {
      std::cout << "One or both books not found.\n";
      return;
    }
    if (findBook(resultBookName)) {
      std::cout << "Result book '" << resultBookName << "' already exists.\n";
      return;
    }
    Book resultBook(resultBookName);
    books_.push_back(resultBook);
    Book* newBook = findBook(resultBookName);
    for (auto it = book1->records.begin(); it != book1->records.end(); ++it) {
      Record newRecord = *it;
      newBook->records.push_back(newRecord);
    }
    for (auto it = book2->records.begin(); it != book2->records.end(); ++it) {
      Record* existingRecord = findRecord(newBook, it->key);
      if (existingRecord) {
        std::cout << "Conflict detected for key '" << it->key << "'. Choose option:\n";
        std::cout << "1. Overwrite with record from " << bookName2 << "\n";
        std::cout << "2. Rename key to '" << it->key << "_2'\n";
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        if (choice == 1) {
          *existingRecord = *it;
          std::cout << "Record overwritten.\n";
        } else if (choice == 2) {
          Record newRecord = *it;
          newRecord.key = it->key + "_2";
          newBook->records.push_back(newRecord);
          std::cout << "Record added with new key '" << newRecord.key << "'.\n";
        } else {
          std::cout << "Invalid choice. Record skipped.\n";
        }
      } else {
        Record newRecord = *it;
        newBook->records.push_back(newRecord);
      }
    }
    std::cout << "Books combined successfully into '" << resultBookName << "'.\n";
  }

  void BookSystem::movetags(const std::string& sourceBook, const std::string& targetBook, const List< std::string >& colors)
  {
    Book* srcBook = findBook(sourceBook);
    Book* tgtBook = findBook(targetBook);
    if (!srcBook || !tgtBook) {
      std::cout << "One or both books not found.\n";
      return;
    }
    List< std::string > keysToMove;
    for (auto it = srcBook->records.begin(); it != srcBook->records.end(); ++it) {
      if (recordHasAnyTag(&(*it), colors)) {
        keysToMove.push_back(it->key);
      }
    }
    for (auto keyIt = keysToMove.begin(); keyIt != keysToMove.end(); ++keyIt) {
      Record* record = findRecord(srcBook, *keyIt);
      if (record) {
        tgtBook->records.push_back(*record);
        deleteRecord(sourceBook, *keyIt);
      }
    }
    std::cout << "Moved " << keysToMove.size() << " records from '" << sourceBook << "' to '" << targetBook << "'.\n";
  }

  void BookSystem::edittags(const std::string& bookName, const std::string& key, const std::string& mode, const List< std::string >& colors)
  {
    Book* book = findBook(bookName);
    if (!book) {
      std::cout << "Book '" << bookName << "' not found.\n";
      return;
    }
    Record* record = findRecord(book, key);
    if (!record) {
      std::cout << "Record '" << key << "' not found in book '" << bookName << "'.\n";
      return;
    }
    if (mode == "SET" || mode == "ADD") {
      for (auto it = colors.begin(); it != colors.end(); ++it) {
        if (!isValidColor(*it)) {
          std::cout << "Error: Invalid color '" << *it << "'. Valid colors are: red, blue, green.\n";
          return;
        }
      }
    }
    if (mode == "SET") {
      record->tags.clear();
      for (auto it = colors.begin(); it != colors.end(); ++it) {
        record->tags.push_back(*it);
      }
    } else if (mode == "ADD") {
      for (auto it = colors.begin(); it != colors.end(); ++it) {
        bool exists = false;
        for (auto tagIt = record->tags.begin(); tagIt != record->tags.end(); ++tagIt) {
          if (*tagIt == *it) {
            exists = true;
            break;
          }
        }
        if (!exists) {
          record->tags.push_back(*it);
        }
      }
    } else if (mode == "DEL") {
      List< std::string > newTags;
      for (auto tagIt = record->tags.begin(); tagIt != record->tags.end(); ++tagIt) {
        bool shouldKeep = true;
        for (auto colorIt = colors.begin(); colorIt != colors.end(); ++colorIt) {
          if (*tagIt == *colorIt) {
            shouldKeep = false;
            break;
          }
        }
        if (shouldKeep) {
          newTags.push_back(*tagIt);
        }
      }
      record->tags = newTags;
    } else {
      std::cout << "Invalid mode. Use SET, ADD, or DEL.\n";
      return;
    }
    std::cout << "Tags updated for record '" << key << "'.\n";
  }

  void BookSystem::deletebytags(const std::string& bookName, const std::string& logic, const List< std::string >& colors)
  {
    Book* book = findBook(bookName);
    if (!book) {
      std::cout << "Book '" << bookName << "' not found.\n";
      return;
    }
    List< std::string > keysToDelete;
    for (auto it = book->records.begin(); it != book->records.end(); ++it) {
      bool match = false;
      if (logic == "AND") {
        match = recordHasAllTags(&(*it), colors);
      } else if (logic == "OR") {
        match = recordHasAnyTag(&(*it), colors);
      } else {
        std::cout << "Invalid logic operator. Use AND or OR.\n";
        return;
      }
      if (match) {
        keysToDelete.push_back(it->key);
      }
    }
    for (auto keyIt = keysToDelete.begin(); keyIt != keysToDelete.end(); ++keyIt) {
      deleteRecord(bookName, *keyIt);
    }
    std::cout << "Deleted " << keysToDelete.size() << " records from book '" << bookName << "'.\n";
  }

  void BookSystem::replacetags(const std::string& bookName, const std::string& oldColor, const std::string& newColor)
  {
    Book* book = findBook(bookName);
    if (!book) {
      std::cout << "Book '" << bookName << "' not found.\n";
      return;
    }
    int replacedCount = 0;
    for (auto it = book->records.begin(); it != book->records.end(); ++it) {
      for (auto tagIt = it->tags.begin(); tagIt != it->tags.end(); ++tagIt) {
        if (*tagIt == oldColor) {
          *tagIt = newColor;
          replacedCount++;
        }
      }
    }
    std::cout << "Replaced " << replacedCount << " occurrences of tag '" << oldColor << "' with '" << newColor << "'.\n";
  }

  void BookSystem::listallwith(const std::string& key)
  {
    bool found = false;
    for (auto bookIt = books_.begin(); bookIt != books_.end(); ++bookIt) {
      for (auto recordIt = bookIt->records.begin(); recordIt != bookIt->records.end(); ++recordIt) {
        for (auto tagIt = recordIt->tags.begin(); tagIt != recordIt->tags.end(); ++tagIt) {
          if (*tagIt == key) {
            if (!found) {
              std::cout << "Books and records with tag '" << key << "':\n";
              found = true;
            }
            std::cout << "Book: " << bookIt->name << " | Record: " << recordIt->key << "\n";
            break;
          }
        }
      }
    }
    if (!found) {
      std::cout << "No books or records found with tag '" << key << "'.\n";
    }
  }

  void BookSystem::deletebook(const std::string& bookName)
  {
    for (auto it = books_.begin(); it != books_.end(); ++it) {
      if (it->name == bookName) {
        books_.remove(*it);
        std::cout << "Book '" << bookName << "' deleted successfully.\n";
        return;
      }
    }
    std::cout << "Book '" << bookName << "' not found.\n";
  }

  void BookSystem::deleteRecord(const std::string& bookName, const std::string& key)
  {
    Book* book = findBook(bookName);
    if (!book) {
      std::cout << "Book '" << bookName << "' not found.\n";
      return;
    }
    for (auto it = book->records.begin(); it != book->records.end(); ++it) {
      if (it->key == key) {
        book->records.remove(*it);
        std::cout << "Record '" << key << "' deleted from book '" << bookName << "'.\n";
        return;
      }
    }
    std::cout << "Record '" << key << "' not found in book '" << bookName << "'.\n";
  }

  void BookSystem::listallwithcolor(const std::string& color)
  {
    bool found = false;
    for (auto bookIt = books_.begin(); bookIt != books_.end(); ++bookIt) {
      for (auto recordIt = bookIt->records.begin(); recordIt != bookIt->records.end(); ++recordIt) {
        for (auto tagIt = recordIt->tags.begin(); tagIt != recordIt->tags.end(); ++tagIt) {
          if (*tagIt == color) {
            if (!found) {
              std::cout << "Books and records with color '" << color << "':\n";
              found = true;
            }
            std::cout << "Book: " << bookIt->name << " | Record: " << recordIt->key << " | Tags: ";
            for (auto tIt = recordIt->tags.begin(); tIt != recordIt->tags.end(); ++tIt) {
              std::cout << *tIt << " ";
            }
            std::cout << "\n";
            break;
          }
        }
      }
    }
    if (!found) {
      std::cout << "No books or records found with color '" << color << "'.\n";
    }
  }

  void BookSystem::statistics()
  {
    std::cout << "=== System Statistics ===\n";
    std::cout << "Total books: " << books_.size() << "\n";
    int totalRecords = 0;
    int totalTags = 0;
    int redCount = 0;
    int blueCount = 0;
    int greenCount = 0;
    for (auto bookIt = books_.begin(); bookIt != books_.end(); ++bookIt) {
      totalRecords += bookIt->records.size();
      std::cout << "  Book '" << bookIt->name << "': " << bookIt->records.size() << " records\n";
      for (auto recordIt = bookIt->records.begin(); recordIt != bookIt->records.end(); ++recordIt) {
        totalTags += recordIt->tags.size();
        for (auto tagIt = recordIt->tags.begin(); tagIt != recordIt->tags.end(); ++tagIt) {
          if (*tagIt == "red") {
            redCount++;
          } else if (*tagIt == "blue") {
            blueCount++;
          } else if (*tagIt == "green") {
            greenCount++;
          }
        }
      }
    }
    std::cout << "Total records: " << totalRecords << "\n";
    std::cout << "Total tags: " << totalTags << "\n";
    std::cout << "Tag distribution:\n";
    std::cout << "  red: " << redCount << "\n";
    std::cout << "  blue: " << blueCount << "\n";
    std::cout << "  green: " << greenCount << "\n";
    if (totalRecords > 0) {
      std::cout << "Average tags per record: " << static_cast< double >(totalTags) / totalRecords << "\n";
    }
  }
}

#endif
