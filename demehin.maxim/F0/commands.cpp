#include "commands.hpp"
#include <fstream>

namespace
{
  void printList(std::ostream& out, const demehin::list_t& lst)
  {
    bool isFirst = true;
    for (auto&& translation: lst)
    {
      if (!isFirst)
      {
        out << ",";
      }
      out << " " << translation;
      isFirst = false;
    }
  }

  demehin::tree_t unionTwo(const demehin::tree_t& dict1, const demehin::tree_t& dict2)
  {
    demehin::tree_t res = dict1;
    for (auto&& key: dict2)
    {
      res.insert(key);
    }
    return res;
  }

  demehin::tree_t intersectTwo(const demehin::tree_t& dict1, const demehin::tree_t& dict2)
  {
    demehin::tree_t res;
    for (auto&& key: dict1)
    {
      if (dict2.find(key.first) != dict2.end())
      {
        res.insert(key);
      }
    }
    return res;
  }

  demehin::tree_t complementTwo(const demehin::tree_t& dict1, const demehin::tree_t& dict2)
  {
    demehin::tree_t res = dict1;
    for (auto&& key: dict2)
    {
      res.erase(key.first);
    }
    return res;
  }

  void writeDict(std::ostream& out, const std::string& dict_name, const demehin::tree_t& dict)
  {
    out << dict_name << "\n";
    for (auto&& key: dict)
    {
      out << key.first;
      for (auto&& ru_word: key.second)
      {
        out << " " << ru_word;
      }
      out << "\n";
    }
    out << "\n";
  }

  void processWriting(std::istream& in, const demehin::dict_t& dicts, std::ios_base::openmode mode)
  {
    std::string filename;
    size_t dicts_cnt;
    in >> filename >> dicts_cnt;

    std::ofstream file(filename, mode);
    if (!file)
    {
      throw std::logic_error("incorrect file");
    }

    for (size_t i = 0; i < dicts_cnt; i++)
    {
      std::string dict_name;
      in >> dict_name;
      auto it = dicts.find(dict_name);
      if (it != dicts.end())
      {
        writeDict(file, dict_name, dicts.at(dict_name));
      }
    }
  }
}

void demehin::printHelp(std::ostream& out)
{
  out << "Commands system:\n";
  out << "1. createdict < dictname > - create empty dictionary\n";
  out << "2. deletedict < dictname > - delete excisting dictionary\n";
  out << "3. printdict < dictname > - print dict content\n";
  out << "4. gettranslationeng < dictname > < eng > - print translations of word eng\n";
  out << "5. gettranslationru < dictname > < ru > - print translations of word ru\n";
  out << "6. deleteeng < dictname > < eng > - delete word eng from dictionary\n";
  out << "7. addeng < dictname > < eng > < N > < ru-1 > ... < ru-n > - add word eng with its N translations\n";
  out << "8. addru < dictname > < eng > < N > < ru-1 > ... < ru-n > - add N translations to eng\n";
  out << "9. deleteru < dictname > < eng > < N > < ru-1 > ... < ru-n > - delete N translations of eng\n";
  out << "10. union < newdict > < N > < dictname-1 > ... < dictname-n > - union of N dictionaries\n";
  out << "11. complement < newdict > < N > < dictname-1 > ... < dictname-n > - complemention of N dictionaries\n";
  out << "12. intersect < newdict > < N > < dictname-1 > ... < dictname-n > - intersection of N dictionaries\n";
  out << "13. writedicts < filename > < N > < dictname-1 > ... < dictname-n > - save N dictionaries to file\n>";
  out << "14. rewritefile < filename > < N > < dictname-1 > ... < dictname-n > - rewrite file with N dictionaries\n";
  out << "15. mostcommon < N > < K > < dictname-1 > ... < dictname-n > - print N most common words in K dictionaries\n";
}

void demehin::rewriteFile(std::istream& in, const dict_t& dicts)
{
  processWriting(in, dicts, std::ios::out);
}

void demehin::writeToFile(std::istream& in, const dict_t& dicts)
{
  processWriting(in, dicts, std::ios::app);
}

void demehin::createDict(std::istream& in, dict_t& dicts)
{
  tree_t dict;
  std::string dict_name;
  in >> dict_name;
  if (dicts.find(dict_name) == dicts.end())
  {
    dicts.insert(std::make_pair(dict_name, dict));
  }
  else
  {
    throw std::logic_error("existent dictionary");
  }
}

void demehin::deleteDict(std::istream& in, dict_t& dicts)
{
  std::string dict_name;
  in >> dict_name;
  if (dicts.find(dict_name) == dicts.end())
  {
    throw std::logic_error("non-existent dictionary");
  }

  dicts.erase(dict_name);
}

void demehin::printDict(std::istream& in, std::ostream& out, const dict_t& dicts)
{
  std::string dict_name;
  in >> dict_name;

  tree_t dict = dicts.at(dict_name);

  for (auto&& key: dict)
  {
    out << key.first << ":";
    printList(out, key.second);
    out << "\n";
  }
}

void demehin::getTranslationEng(std::istream& in, std::ostream& out, const dict_t& dicts)
{
  std::string dict_name;
  in >> dict_name;
  auto dict = dicts.at(dict_name);

  std::string eng_word;
  in >> eng_word;

  auto translations = dict.at(eng_word);
  out << eng_word << ":";
  printList(out, translations);
  out << "\n";
}

void demehin::getTranslationRu(std::istream& in, std::ostream& out, const dict_t& dicts)
{
  std::string dict_name, ru_word;
  in >> dict_name >> ru_word;

  auto dict = dicts.at(dict_name);
  demehin::List< std::string > eng_list;

  for (auto&& key: dict)
  {
    for (auto&& translation: key.second)
    {
      if (translation == ru_word)
      {
        eng_list.push_front(key.first);
        break;
      }
    }
  }

  if (eng_list.empty())
  {
    throw std::logic_error("no such russian word");
  }

  out << ru_word << ":";
  printList(out, eng_list);
  out << "\n";
}

void demehin::deleteEng(std::istream& in, dict_t& dicts)
{
  std::string dict_name, eng_word;
  in >> dict_name >> eng_word;
  auto& dict = dicts.at(dict_name);
  if (dict.erase(eng_word) == 0)
  {
    throw std::logic_error("no such english word");
  }
}

void demehin::addEng(std::istream& in, dict_t& dicts)
{
  std::string dict_name, eng_word;
  size_t translations_cnt;

  in >> dict_name >> eng_word >> translations_cnt;

  auto& dict = dicts.at(dict_name);
  if (dict.count(eng_word) == 1)
  {
    throw std::logic_error("word already exist");
  }

  list_t translations;
  for (size_t i = 0; i < translations_cnt; i++)
  {
    std::string ru_word;
    in >> ru_word;
    translations.push_back(ru_word);
  }
  dict.insert(std::make_pair(eng_word, translations));
}

void demehin::deleteRu(std::istream& in, dict_t& dicts)
{
  std::string dict_name, eng_word;
  size_t translations_cnt;

  in >> dict_name >> eng_word >> translations_cnt;

  auto& dict = dicts.at(dict_name);
  auto& translations = dict.at(eng_word);

  for (size_t i = 0; i < translations_cnt; i++)
  {
    std::string ru_word;
    in >> ru_word;
    translations.remove(ru_word);
  }
}

void demehin::addRu(std::istream& in, dict_t& dicts)
{
  std::string dict_name, eng_word;
  size_t translations_cnt;

  in >> dict_name >> eng_word >> translations_cnt;

  auto& dict = dicts.at(dict_name);
  auto& translations = dict.at(eng_word);

  for (size_t i = 0; i < translations_cnt; i++)
  {
    std::string ru_word;
    in >> ru_word;
    translations.push_back(ru_word);
  }
}

void demehin::makeUnion(std::istream& in, dict_t& dicts)
{
  std::string new_name;
  size_t dicts_cnt;
  in >> new_name >> dicts_cnt;

  tree_t res;
  for (size_t i = 0; i < dicts_cnt; i++)
  {
    std::string dict_name;
    in >> dict_name;
    res = unionTwo(res, dicts.at(dict_name));
  }

  dicts.insert(std::make_pair(new_name, res));
}

void demehin::makeIntersect(std::istream& in, dict_t& dicts)
{
  std::string new_name, first_name;
  size_t dicts_cnt;
  in >> new_name >> dicts_cnt >> first_name;

  tree_t res = dicts.at(first_name);

  for (size_t i = 1; i < dicts_cnt; i++)
  {
    std::string dict_name;
    in >> dict_name;
    res = intersectTwo(res, dicts.at(dict_name));
  }

  dicts.insert(std::make_pair(new_name, res));
}

void demehin::makeComplement(std::istream& in, dict_t& dicts)
{
  std::string new_name, first_name;
  size_t dicts_cnt;
  in >> new_name >> dicts_cnt >> first_name;

  tree_t res = dicts.at(first_name);

  for (size_t i = 1; i < dicts_cnt; i++)
  {
    std::string dict_name;
    in >> dict_name;
    res = complementTwo(res, dicts.at(dict_name));
  }

  dicts.insert(std::make_pair(new_name, res));
}

void demehin::printMostCommons(std::istream& in, std::ostream& out, const dict_t& dicts)
{
  int n, k;
  in >> n >> k;
  if (n <= 0 || k <= 0)
  {
    throw std::logic_error("incorrect count");
  }

  list_t dicts_names;
  std::string dict_name;

  for (int i = 0; i < k; i++)
  {
    in >> dict_name;
    dicts_names.push_back(dict_name);
  }

  Tree< std::string, int > word_frequency;
  for (auto&& name: dicts_names)
  {
    auto unit = dicts.at(name);
    for (auto&& key: unit)
    {
      word_frequency[key.first]++;
    }
  }

  Tree< int, list_t, std::greater< int > > freq_to_words;
  for (auto&& key: word_frequency)
  {
    freq_to_words[key.second].push_back(key.first);
  }

  int cnt = 0;
  for (auto&& key: freq_to_words)
  {
    for (auto&& word: key.second)
    {
      if (cnt++ >= n)
      {
        break;
      }
      out << word << " " << key.first << "\n";
    }
    if (cnt >= n)
    {
      break;
    }
  }
}
