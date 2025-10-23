#include "commands.hpp"
#include <fstream>
#include <stdexcept>

namespace bocharov
{
  void printHelp(std::ostream& out)
  {
    out << "Available commands:\n"
        << "createdict <dictname> - create empty dictionary\n"
        << "deletedict <dictname> - delete dictionary\n"
        << "printdict <dictname> - print dictionary content\n"
        << "gettranslationsln <dictname> <sln> - get translations for slang word\n"
        << "gettranslationru <dictname> <ru> - get slang words for russian word\n"
        << "deletesln <dictname> <sln> - delete slang word\n"
        << "addsln <dictname> <sln> <N> <ru1> ... <ruN> - add slang word with translations\n"
        << "addru <dictname> <sln> <N> <ru1> ... <ruN> - add translations to existing slang\n"
        << "deleteru <dictname> <sln> <N> <ru1> ... <ruN> - delete translations\n"
        << "writedicts <filename> <N> <dictname1> ... <dictnameN> - write dictionaries to file\n"
        << "union <newdict> <N> <dictname1> ... <dictnameN> - union of dictionaries\n"
        << "mostcommon <N> <K> <dictname1> ... <dictnameK> - print most common words\n"
        << "rewritefile <filename> <N> <dictname1> ... <dictnameN> - rewrite file with dictionaries\n"
        << "--help - show help\n";
  }

  void createDict(std::istream& in, dict_dict_t& dicts)
  {
    std::string dictname;
    in >> dictname;
    if (dicts.find(dictname) != dicts.end())
    {
      throw std::invalid_argument("Dictionary already exists");
    }
    dicts.insert(std::make_pair(dictname, dict_t()));
  }

  void deleteDict(std::istream& in, dict_dict_t& dicts)
  {
    std::string dictname;
    in >> dictname;
    if (dicts.erase(dictname) == 0)
    {
      throw std::invalid_argument("Dictionary not found");
    }
  }

  void printDict(std::istream& in, std::ostream& out, const dict_dict_t& dicts)
  {
    std::string dictname;
    in >> dictname;
    const dict_t& dict = dicts.at(dictname);

    for (auto it = dict.begin(); it != dict.end(); ++it)
    {
      out << it->first << ":";
      const list_t& translations = it->second;
      bool first = true;
      for (auto tr = translations.begin(); tr != translations.end(); ++tr)
      {
        if (!first) out << ",";
        out << " " << *tr;
        first = false;
      }
      out << "\n";
    }
  }

  void getTranslationSln(std::istream& in, std::ostream& out, const dict_dict_t& dicts)
  {
    std::string dictname, sln;
    in >> dictname >> sln;
    const dict_t& dict = dicts.at(dictname);
    const list_t& translations = dict.at(sln);

    out << sln << ":";
    bool first = true;
    for (auto tr = translations.begin(); tr != translations.end(); ++tr)
    {
      if (!first) out << ",";
      out << " " << *tr;
      first = false;
    }
    out << "\n";
  }

  void getTranslationRu(std::istream& in, std::ostream& out, const dict_dict_t& dicts)
  {
    std::string dictname, ru;
    in >> dictname >> ru;
    const dict_t& dict = dicts.at(dictname);
    list_t slangWords;

    for (auto it = dict.begin(); it != dict.end(); ++it)
    {
      const list_t& translations = it->second;
      for (auto tr = translations.begin(); tr != translations.end(); ++tr)
      {
        if (*tr == ru)
        {
          slangWords.push_back(it->first);
          break;
        }
      }
    }

    out << ru << ":";
    bool first = true;
    for (auto sln = slangWords.begin(); sln != slangWords.end(); ++sln)
    {
      if (!first) out << ",";
      out << " " << *sln;
      first = false;
    }
    out << "\n";
  }

  void deleteSln(std::istream& in, dict_dict_t& dicts)
  {
    std::string dictname, sln;
    in >> dictname >> sln;
    dict_t& dict = dicts.at(dictname);
    if (dict.erase(sln) == 0)
    {
      throw std::invalid_argument("Slang word not found");
    }
  }

  void addSln(std::istream& in, dict_dict_t& dicts)
  {
    std::string dictname, sln;
    size_t n;
    in >> dictname >> sln >> n;

    dict_t& dict = dicts.at(dictname);
    if (dict.find(sln) != dict.end())
    {
      throw std::invalid_argument("Slang word already exists");
    }

    list_t translations;
    for (size_t i = 0; i < n; ++i)
    {
      std::string ru;
      in >> ru;
      translations.push_back(ru);
    }

    dict.insert(std::make_pair(sln, translations));
  }

  void addRu(std::istream& in, dict_dict_t& dicts)
  {
    std::string dictname, sln;
    size_t n;
    in >> dictname >> sln >> n;

    dict_t& dict = dicts.at(dictname);
    list_t& translations = dict.at(sln);

    for (size_t i = 0; i < n; ++i)
    {
      std::string ru;
      in >> ru;
      translations.push_back(ru);
    }
  }

  void deleteRu(std::istream& in, dict_dict_t& dicts)
  {
    std::string dictname, sln;
    size_t n;
    in >> dictname >> sln >> n;

    dict_t& dict = dicts.at(dictname);
    list_t& translations = dict.at(sln);

    for (size_t i = 0; i < n; ++i)
    {
      std::string ru;
      in >> ru;
      translations.remove(ru);
    }
  }

  void writeDicts(std::istream& in, const dict_dict_t& dicts)
  {
    std::string filename;
    size_t n;
    in >> filename >> n;

    std::ofstream file(filename, std::ios::app);
    if (!file)
    {
      throw std::runtime_error("Cannot open file");
    }

    for (size_t i = 0; i < n; ++i)
    {
      std::string dictname;
      in >> dictname;
      const dict_t& dict = dicts.at(dictname);

      file << dictname << "\n";
      for (auto it = dict.begin(); it != dict.end(); ++it)
      {
        file << it->first;
        const list_t& translations = it->second;
        for (auto tr = translations.begin(); tr != translations.end(); ++tr)
        {
          file << " " << *tr;
        }
        file << "\n";
      }
      file << "\n";
    }
  }

  void makeUnion(std::istream& in, dict_dict_t& dicts)
  {
    std::string newDictname;
    size_t n;
    in >> newDictname >> n;

    dict_t unionDict;
    for (size_t i = 0; i < n; ++i)
    {
      std::string dictname;
      in >> dictname;
      const dict_t& dict = dicts.at(dictname);

      for (auto it = dict.begin(); it != dict.end(); ++it)
      {
        if (unionDict.find(it->first) == unionDict.end())
        {
          unionDict.insert(std::make_pair(it->first, it->second));
        }
    }
  }

    dicts.insert(std::make_pair(newDictname, unionDict));
  }

  void printMostCommon(std::istream& in, std::ostream& out, const dict_dict_t& dicts)
  {
    int n, k;
    in >> n >> k;
    if (n <= 0 || k <= 0)
    {
      throw std::invalid_argument("Invalid N or K");
    }


    Tree<std::string, int> frequency;


    for (int i = 0; i < k; ++i)
    {
      std::string dictname;
      in >> dictname;

      const dict_t& dict = dicts.at(dictname);


      for (auto it = dict.begin(); it != dict.end(); ++it)
      {
        const std::string& word = it->first;
        auto freqIt = frequency.find(word);
        if (freqIt == frequency.end())
        {

          frequency.insert(std::make_pair(word, 1));
        }
        else
        {
          int count = freqIt->second;
          frequency.erase(word);
          frequency.insert(std::make_pair(word, count + 1));
        }
      }
    }

    Tree<int, List<std::string>, std::greater<int>> sortedFreq;

    for (auto it = frequency.begin(); it != frequency.end(); ++it)
    {
      const int count = it->second;
      const std::string& word = it->first;

      auto sortedIt = sortedFreq.find(count);
      if (sortedIt == sortedFreq.end())
      {
        List<std::string> newList;
        newList.push_back(word);
        sortedFreq.insert(std::make_pair(count, newList));
      }
      else
      {
        sortedFreq.at(count).push_back(word);
      }
    }

    int printed = 0;
    for (auto freqIt = sortedFreq.begin(); freqIt != sortedFreq.end() && printed < n; ++freqIt)
    {
      const int count = freqIt->first;
      const List<std::string>& words = freqIt->second;

      for (auto wordIt = words.begin(); wordIt != words.end() && printed < n; ++wordIt)
      {
        out << *wordIt << " " << count << "\n";
        printed++;
      }
    }
  }

  void rewriteFile(std::istream& in, const dict_dict_t& dicts)
  {
    std::string filename;
    size_t n;
    in >> filename >> n;

    std::ofstream file(filename);
    if (!file)
    {
      throw std::runtime_error("Cannot open file");
    }

    for (size_t i = 0; i < n; ++i)
    {
      std::string dictname;
      in >> dictname;
      const dict_t& dict = dicts.at(dictname);

      file << dictname << "\n";
      for (auto it = dict.begin(); it != dict.end(); ++it)
      {
        file << it->first;
        const list_t& translations = it->second;
        for (auto tr = translations.begin(); tr != translations.end(); ++tr)
        {
          file << " " << *tr;
        }
        file << "\n";
      }
      file << "\n";
    }
  }

  void rare(std::istream & in, dict_dict_t & dicts)
  {
    int K;
    in >> K;
    std::string newDictName;
    in >> newDictName;
    size_t n;
    in >> n;


    if (K < 0 || n <= 0 || K >= static_cast<int>(n))
    {
      throw std::invalid_argument("Invalid K or N");
    }


    List< std::string > dictNames;
    for (size_t i = 0; i < n; ++i)
    {
      std::string name;
      in >> name;
      dictNames.push_back(name);
    }

    for (auto it = dictNames.begin(); it != dictNames.end(); ++it)
    {
      if (dicts.find(*it) == dicts.end())
      {
        throw std::invalid_argument("Dictionary not found");
      }
    }


    Tree<std::string, std::pair<int, List<std::string>>> temp;


    for (auto dictIt = dictNames.begin(); dictIt != dictNames.end(); ++dictIt)
    {
      const dict_t& dict = dicts.at(*dictIt);
      for (auto wordIt = dict.begin(); wordIt != dict.end(); ++wordIt)
      {
        const std::string& sln = wordIt->first;
        const list_t& translations = wordIt->second;

        auto tempIt = temp.find(sln);
        if (tempIt == temp.end())
        {
            temp.insert(std::make_pair(sln, std::make_pair(1, translations)));
        }
        else
        {
          tempIt->second.first++;
          for (auto trIt = translations.begin(); trIt != translations.end(); ++trIt)
          {
            tempIt->second.second.push_back(*trIt);
          }
        }
      }
    }


    dict_t newDict;
    for (auto tempIt = temp.begin(); tempIt != temp.end(); ++tempIt)
    {
      const std::string& sln = tempIt->first;
      int count = tempIt->second.first;
      const list_t& translations = tempIt->second.second;

      if (count <= K)
      {
        Tree<std::string, bool> uniqueTree;
        list_t uniqueTranslations;

        for (auto trIt = translations.begin(); trIt != translations.end(); ++trIt)
        {
          const std::string& tr = *trIt;
          if (uniqueTree.find(tr) == uniqueTree.end())
          {
            uniqueTree.insert(std::make_pair(tr, true));
            uniqueTranslations.push_back(tr);
          }
        }
        newDict.insert(std::make_pair(sln, uniqueTranslations));
      }
    }


    if (dicts.find(newDictName) != dicts.end())
    {
      throw std::invalid_argument("Dictionary already exists");
    }
    dicts.insert(std::make_pair(newDictName, newDict));
  }
}
