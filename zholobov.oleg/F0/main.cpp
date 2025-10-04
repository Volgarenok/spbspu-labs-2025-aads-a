#include <cstring>
#include <functional>
#include <iostream>

#include <array.hpp>
#include <tree/tree.hpp>

#include "commands.hpp"
#include "utils.hpp"

int main(int argc, char* argv[])
{
  if (argc > 2) {
    std::cerr << "<INCORRECT ARGUMENTS>\n";
    return 1;
  }
  if (argc == 2) {
    if (std::strcmp(argv[1], "--help") == 0) {
      zholobov::printHelp(std::cout);
      std::cout << '\n';
      return 0;
    }
  }

  zholobov::Dictionaries dictionaries;

  using Handler = std::function< void(zholobov::Dictionaries&, const zholobov::Array< zholobov::Word >&) >;
  zholobov::Tree< zholobov::Word, Handler > keywords;

  keywords["dict-create"] = zholobov::cmdDictCreate;
  keywords["dict-remove"] = zholobov::cmdDictRemove;
  keywords["dict-import"] = zholobov::cmdDictImport;
  keywords["dict-export"] = zholobov::cmdDictExport;
  keywords["dict-count"] = zholobov::cmdDictCount;
  keywords["dict-list"] = zholobov::cmdDictList;
  keywords["dict-print-words"] = zholobov::cmdDictPrintWords;
  keywords["dict-print-translations"] = zholobov::cmdDictPrintTranslations;
  keywords["dict-clear"] = zholobov::cmdDictClear;
  keywords["add-word"] = zholobov::cmdAddWord;
  keywords["remove-word"] = zholobov::cmdRemoveWord;
  keywords["count-words"] = zholobov::cmdCountWords;
  keywords["add-translation"] = zholobov::cmdAddTranslation;
  keywords["remove-translation"] = zholobov::cmdRemoveTranslation;
  keywords["change-word"] = zholobov::cmdChangeWord;
  keywords["translate-word"] = zholobov::cmdTranslateWord;
  keywords["union"] = zholobov::cmdUnion;
  keywords["intersect"] = zholobov::cmdIntersect;
  keywords["rare"] = zholobov::cmdRare;

  std::string line;
  while (std::getline(std::cin, line)) {
    zholobov::Array< std::string > tokens = zholobov::splitTokens(line);
    if (tokens.empty()) {
      continue;
    }

    const std::string& cmd = tokens[0];
    try {
      keywords.at(cmd)(dictionaries, tokens);
    } catch (const zholobov::InvalidParams&) {
      std::cout << "<ERROR IN PARAMETERS>\n";
    } catch (const std::out_of_range&) {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
