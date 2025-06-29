#include "commands.hpp"
#include <string>
#include <fstream>
#include <hash_dyn_array.hpp>
#include <AVLtree.hpp>
#include <dynamic_array.hpp>

namespace
{
  using tree_of_words = tkach::AvlTree< std::string, tkach::List< std::string > >;
  using tree_of_dict = tkach::AvlTree< std::string, tree_of_words >;

  bool findTranslation(const tkach::List< std::string >& list, const std::string& translation)
  {
    if (list.empty())
    {
      return false;
    }
    for (auto it = list.cbegin(); it != list.cend(); ++it)
    {
      if (*it == translation)
      {
        return true;
      }
    }
    return false;
  }

  tkach::List< std::string > mergeTranslations(const tkach::List< std::string >& list1, const tkach::List< std::string >& list2)
  {
    tkach::AvlTree< std::string, std::string > tree_translations;
    for (auto it = list1.cbegin(); it != list1.cend(); ++it)
    {
      tree_translations[*it];
    }
    if (std::addressof(list1) != std::addressof(list2))
    {
      for (auto it = list2.cbegin(); it != list2.cend(); ++it)
      {
        tree_translations[*it];
      }
    }
    tkach::List< std::string > translations;
    for (auto it = tree_translations.cbegin(); it != tree_translations.cend(); ++it)
    {
      translations.pushBack(it->first);
    }
    return translations;
  }

  tkach::List< std::string > getCommonElements(const tkach::List< std::string >& list1, const tkach::List< std::string >& list2)
  {
    tkach::List< std::string > common_list;
    for (auto it = list1.cbegin(); it != list1.cend(); ++it)
    {
      if (findTranslation(list2, *it) && !findTranslation(common_list, *it))
      {
        common_list.pushBack(*it);
      }
    }
    return common_list;
  }

  void writeDictToFile(std::ostream& out, const std::string& dict_name, const tree_of_words& dict)
  {
    out << dict_name << "\n";
    for (auto it = dict.cbegin(); it != dict.cend(); ++it)
    {
      out << it->first;
      for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); ++it2)
      {
        out << " " << *it2;
      }
      out << "\n";
    }
  }

  tkach::AvlTree< std::string, tkach::List< std::string > > mergeDicts(const tkach::List< const tree_of_words* >& source_dicts)
  {
    size_t size = source_dicts.size();
    if (size == 0)
    {
      throw std::logic_error("EMPTY");
    }
    tree_of_words temp_dict;
    for (auto it = source_dicts.cbegin(); it != source_dicts.cend(); ++it)
    {
      const tree_of_words& now_dict = *(*it);
      for (auto it = now_dict.cbegin(); it != now_dict.cend(); ++it)
      {
        std::string word = it->first;
        tkach::List< std::string > translations = it->second;
        temp_dict[word] = mergeTranslations(translations, temp_dict[word]);
      }
    }
    return temp_dict;
  }

  void exportDictionaries(std::istream& in, const tree_of_dict& avltree, std::ios_base::openmode mode)
  {
    std::string filename;
    int count_of_dicts = 0;
    if (!(in >> filename) || filename.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    if (!(in >> count_of_dicts) || (count_of_dicts < 0))
    {
      throw std::logic_error("<INVALID NUMBER>");
    }
    std::ofstream outFile(filename, mode);
    if (!outFile.is_open())
    {
      throw std::logic_error("<EXPORT FAILED>");
    }
    if (mode == std::ios_base::app)
    {
      outFile << "\n";
    }
    if (count_of_dicts == 0)
    {
      if (avltree.empty())
      {
        return;
      }
      for (auto it = avltree.cbegin(); it != avltree.cend(); ++it)
      {
        writeDictToFile(outFile, it->first, it->second);
        outFile << "\n";
      }
    }
    else
    {
      tkach::HashDynArray< std::string > current_dict_name(count_of_dicts);
      for (size_t i = 0; i < static_cast< size_t >(count_of_dicts); ++i)
      {
        if (!(in >> current_dict_name[i]) || current_dict_name.empty())
        {
          throw std::logic_error("<INVALID ARGUMENTS>");
        }
        auto it = avltree.find(current_dict_name[i]);
        if (it == avltree.cend())
        {
          throw std::logic_error("<INVALID DICTIONARY>");
        }
      }
      for (size_t i = 0; i < static_cast< size_t >(count_of_dicts); ++i)
      {
        auto it = avltree.find(current_dict_name[i]);
        writeDictToFile(outFile, it->first, it->second);
        outFile << "\n";
      }
    }
  }
}

void tkach::addWord(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree)
{
  std::string dict_name;
  std::string eng_word;
  int num_translations = 0;
  if (!(in >> dict_name) || dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (!(in >> eng_word) || eng_word.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (!(in >> num_translations) || (num_translations <= 0))
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  List< std::string > translations;
  std::string translation;
  for (size_t i = 0; i < static_cast< size_t >(num_translations); ++i)
  {
    if (!(in >> translation))
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    translations.pushBack(translation);
  }
  translations = mergeTranslations(translations, translations);
  auto it = avltree.find(dict_name);
  if (it == avltree.end())
  {
    tree_of_words new_dict;
    new_dict[eng_word] = translations;
    avltree[dict_name] = new_dict;
  }
  else
  {
    tree_of_words& current_dict = it->second;
    current_dict[eng_word] = mergeTranslations(translations, current_dict[eng_word]);
  }
}

void tkach::mergeWords(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree)
{
  std::string dict_name;
  std::string eng_word1;
  std::string eng_word2;
  if (!(in >> dict_name) || dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  auto it = avltree.find(dict_name);
  if (it == avltree.end())
  {
    throw std::logic_error("<INVALID DICTIONARY>");
  }
  if (!(in >> eng_word1) || eng_word1.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  auto it2 = it->second.find(eng_word1);
  if (it2 == it->second.end())
  {
    throw std::logic_error("<INVALID WORD>");
  }
  if (!(in >> eng_word2) || eng_word2.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  auto it3 = it->second.find(eng_word2);
  if (it3 == it->second.end())
  {
    throw std::logic_error("<INVALID WORD>");
  }
  it2->second = mergeTranslations(it2->second, it3->second);
  it3->second = it2->second;
}

void tkach::printCommonTranslations(std::istream& in, std::ostream& out, const tree_of_dict& avltree)
{
  std::string dict_name;
  int number_of_words = 0;
  if (!(in >> dict_name) || dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  auto it = avltree.find(dict_name);
  if (it == avltree.cend())
  {
    throw std::logic_error("<INVALID DICTIONARY>");
  }
  if (!(in >> number_of_words) || number_of_words <= 0)
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  tkach::List< std::string > common_translations;
  for (size_t i = 0; i < static_cast< size_t >(number_of_words); ++i)
  {
    std::string word;
    if (!(in >> word) || word.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    auto word_it = it->second.find(word);
    if (word_it == it->second.cend())
    {
      throw std::logic_error("<INVALID WORD>");
    }
    if (common_translations.empty())
    {
      common_translations = word_it->second;
    }
    else
    {
      common_translations = getCommonElements(common_translations, word_it->second);
    }
  }
  if (common_translations.empty())
  {
    out << "\n";
  }
  else
  {
    out << *common_translations.cbegin();
    for (auto it3 = ++common_translations.cbegin(); it3 != common_translations.cend(); ++it3)
    {
      out << " " << *it3;
    }
    out << "\n";
  }
}

void tkach::doExportOverwrite(std::istream& in, const AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree)
{
  exportDictionaries(in, avltree, std::ios_base::out);
}

void tkach::doExportInEnd(std::istream& in, const AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree)
{
  exportDictionaries(in, avltree, std::ios_base::app);
}

void tkach::printAll(std::ostream& out, const AvlTree< std::string, AvlTree< std::string, List< std::string > > >& data)
{
  if (data.empty())
  {
    throw std::logic_error("<EMPTY>");
  }
  for (auto it = data.cbegin(); it != data.cend(); ++it)
  {
    out << it->first << "\n";
    if (!(it->second.empty()))
    {
      for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); ++it2)
      {
        out << it2->first;
        if (!it2->second.empty())
        {
          for (auto it3 = it2->second.cbegin(); it3 != it2->second.cend(); it3++)
          {
            out << " " << *it3;
          }
        }
        out << "\n";
      }
    }
  }
}

void tkach::removeWord(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree)
{
  std::string dict_name;
  std::string eng_word;
  if (!(in >> dict_name) || dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  auto it = avltree.find(dict_name);
  if (it == avltree.end())
  {
    throw std::logic_error("<INVALID DICTIONARY>");
  }
  if (!(in >> eng_word) || eng_word.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  bool removed = (it->second).erase(eng_word);
  if (!removed)
  {
    throw std::logic_error("<INVALID WORD>");
  }
}

void tkach::addTranslation(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree)
{
  std::string dict_name = "";
  std::string eng_word;
  std::string translation;
  size_t dict_name_specified = 2;
  if (!(in >> dict_name_specified) || (dict_name_specified != 0 && dict_name_specified != 1))
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  auto it = avltree.begin();
  if (dict_name_specified)
  {
    if (!(in >> dict_name) || dict_name.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    it = avltree.find(dict_name);
    if (it == avltree.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
  }
  if (!(in >> eng_word) || eng_word.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (!(in >> translation) || translation.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (dict_name_specified)
  {
    auto it2 = it->second.find(eng_word);
    if (it2 == it->second.end())
    {
      throw std::logic_error("<INVALID WORD>");
    }
    if (!findTranslation(it2->second, translation))
    {
      List< std::string > new_translations_list;
      new_translations_list.pushBack(translation);
      it2->second = mergeTranslations(it2->second, new_translations_list);
    }
  }
  else
  {
    for (auto it = avltree.begin(); it != avltree.end(); ++it)
    {
      auto it2 = it->second.find(eng_word);
      if (it2 != it->second.end())
      {
        if (!findTranslation(it2->second, translation))
        {
          List< std::string > new_translations_list;
          new_translations_list.pushBack(translation);
          it2->second = mergeTranslations(it2->second, new_translations_list);
        }
      }
    }
  }
}

void tkach::removeTranslation(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree)
{
  std::string dict_name = "";
  std::string eng_word;
  std::string translation;
  size_t dict_name_specified = 2;
  if (!(in >> dict_name_specified) || (dict_name_specified != 0 && dict_name_specified != 1))
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  auto it = avltree.begin();
  if (dict_name_specified)
  {
    if (!(in >> dict_name) || dict_name.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    it = avltree.find(dict_name);
    if (it == avltree.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
  }
  if (!(in >> eng_word) || eng_word.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (!(in >> translation) || translation.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (dict_name_specified)
  {
    auto it2 = it->second.find(eng_word);
    if (it2 == it->second.end())
    {
      throw std::logic_error("<INVALID WORD OR TRANSLATION>");
    }
    if (findTranslation(it2->second, translation))
    {
      it2->second.remove(translation);
    }
    else
    {
      throw std::logic_error("<INVALID WORD OR TRANSLATION>");
    }
  }
  else
  {
    for (auto it = avltree.begin(); it != avltree.end(); ++it)
    {
      auto it2 = it->second.find(eng_word);
      if (it2 != it->second.end())
      {
        if (findTranslation(it2->second, translation))
        {
          it2->second.remove(translation);
        }
      }
    }
  }
}

void tkach::clear(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree)
{
  std::string dict_name;
  if (!(in >> dict_name) || dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  auto it = avltree.find(dict_name);
  if (it == avltree.end())
  {
    throw std::logic_error("<INVALID DICTIONARY>");
  }
  it->second.clear();
}

void tkach::printCount(std::istream& in, std::ostream& out, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree)
{
  std::string dict_name;
  if (!(in >> dict_name) || dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  auto it = avltree.find(dict_name);
  if (it == avltree.end())
  {
    throw std::logic_error("<INVALID DICTIONARY>");
  }
  out << it->second.size() << "\n";
}

void tkach::substructDicts(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree)
{
  std::string new_dict_name;
  int number_of_dictionaries = 0;
  if (!(in >> new_dict_name) || new_dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (!(in >> number_of_dictionaries) || (number_of_dictionaries <= 0))
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  HashDynArray< tree_of_words* > source_dicts;
  for (size_t i = 0; i < static_cast< size_t >(number_of_dictionaries); ++i)
  {
    std::string current_dict_name;
    if (!(in >> current_dict_name) || current_dict_name.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    auto it = avltree.find(current_dict_name);
    if (it == avltree.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    source_dicts[i] = &it->second;
  }
  tree_of_words result_dict(*source_dicts[0]);
  for (size_t i = 1; i < static_cast< size_t >(number_of_dictionaries); ++i)
  {
    const tree_of_words* dict = source_dicts[i];
    for (auto it = dict->cbegin(); it != dict->cend(); ++it)
    {
      result_dict.erase(it->first);
    }
  }
  avltree[new_dict_name] = result_dict;
}

void tkach::mergeNumberDicts(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree)
{
  std::string new_dict_name;
  int number_of_dictionaries = 0;
  if (!(in >> new_dict_name) || new_dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (!(in >> number_of_dictionaries) || (number_of_dictionaries <= 0))
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  List< const tree_of_words* > source_dicts;
  for (size_t i = 0; i < static_cast< size_t >(number_of_dictionaries); ++i)
  {
    std::string current_dict_name;
    if (!(in >> current_dict_name) || current_dict_name.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    auto it = avltree.find(current_dict_name);
    if (it == avltree.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    source_dicts.pushBack(&(it->second));
  }
  tree_of_words result_dict = mergeDicts(source_dicts);
  avltree[new_dict_name] = result_dict;
}

void tkach::doCommonPartDicts(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree)
{
  std::string new_dict_name;
  int number_of_dictionaries = 0;
  if (!(in >> new_dict_name) || new_dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (!(in >> number_of_dictionaries) || (number_of_dictionaries <= 0))
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  HashDynArray< tree_of_words* > source_dicts;
  for (size_t i = 0; i < static_cast< size_t >(number_of_dictionaries); ++i)
  {
    std::string current_dict_name;
    if (!(in >> current_dict_name) || current_dict_name.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    auto it = avltree.find(current_dict_name);
    if (it == avltree.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    source_dicts[i] = &it->second;
  }
  tree_of_words result_dict(*source_dicts[0]);
  for (size_t i = 1; i < static_cast< size_t >(number_of_dictionaries); ++i)
  {
    const tree_of_words* dict = source_dicts[i];
    tree_of_words common;
    for (auto it = result_dict.begin(); it != result_dict.end(); ++it)
    {
      auto it_find = dict->find(it->first);
      if (it_find != dict->cend())
      {
        common[it->first] = mergeTranslations(it->second, it_find->second);
      }
    }
    result_dict = common;
  }
  avltree[new_dict_name] = result_dict;
}

void tkach::copyTranslations(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree)
{
  std::string source_dict_name;
  std::string eng_word;
  int number_of_dictionaries = 0;
  if (!(in >> source_dict_name) || source_dict_name.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  auto source_it = avltree.find(source_dict_name);
  if (source_it == avltree.end())
  {
    throw std::logic_error("<INVALID DICTIONARY>");
  }
  if (!(in >> eng_word) || eng_word.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  const tree_of_words& source_dict = source_it->second;
  auto word_it = source_dict.find(eng_word);
  if (word_it == source_dict.cend())
  {
    throw std::logic_error("<INVALID WORD>");
  }
  if (!(in >> number_of_dictionaries) || (number_of_dictionaries <= 0))
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  for (size_t i = 0; i < static_cast< size_t >(number_of_dictionaries); ++i)
  {
    std::string current_dict_name;
    if (!(in >> current_dict_name) || current_dict_name.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    auto it = avltree.find(current_dict_name);
    if (it == avltree.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    tree_of_words& target = it->second;
    target[eng_word] = mergeTranslations(word_it->second, target[eng_word]);
  }
}

void tkach::import(std::istream& in, AvlTree< std::string, AvlTree< std::string, List< std::string > > >& avltree)
{
  tree_of_dict temp(avltree);
  std::string file_name = "";
  int count_of_dict = 0;
  if (!(in >> file_name))
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  std::fstream in2(file_name);
  if (!in2.is_open())
  {
    throw std::logic_error("<INVALID IMPORT>");
  }
  if (!(in >> count_of_dict) || count_of_dict < 0)
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  HashDynArray< std::string > main_name_dict(count_of_dict);
  for (size_t i = 0; i < static_cast< size_t >(count_of_dict); ++i)
  {
    if (!(in >> main_name_dict[i]))
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    auto it = temp.find(main_name_dict[i]);
    if (it == temp.end())
    {
      throw std::logic_error("<INVALID IMPORT>");
    }
  }
  std::string name_of_dict = "";
  while (in2 >> name_of_dict)
  {
    if (name_of_dict.empty())
    {
      continue;
    }
    tree_of_words temp_dict;
    std::string eng_word;
    while (in2 >> eng_word)
    {
      List< std::string > translations;
      std::string translation;
      while (in2 >> translation)
      {
        translations.pushBack(translation);
        if (in2.peek() == '\n')
        {
          in2.get();
          break;
        }
      }
      temp_dict[eng_word] = mergeTranslations(translations, temp_dict[eng_word]);
      if (in2.peek() == '\n')
      {
        in2.get();
        break;
      }
    }
    temp[name_of_dict] = mergeDicts({&temp_dict, &temp[name_of_dict]});
  }
  if (!in2.eof())
  {
    throw std::logic_error("<INVALID IMPORT>");
  }
  if (count_of_dict != 0)
  {
    for (size_t i = 0; i < static_cast< size_t >(count_of_dict); ++i)
    {
      auto it = temp.find(main_name_dict[i]);
      avltree[main_name_dict[i]] = temp[main_name_dict[i]];
    }
  }
  else
  {
    avltree = std::move(temp);
  }
}

void tkach::printTranslations(std::istream& in, std::ostream& out, const tree_of_dict& avltree)
{
  std::string eng_word;
  int number_of_dictionaries = 0;
  if (!(in >> eng_word) || eng_word.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (!(in >> number_of_dictionaries) || (number_of_dictionaries <= 0))
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  AvlTree< std::string, std::string > tree_translations;
  bool word_found = false;
  for (size_t i = 0; i < static_cast< size_t >(number_of_dictionaries); ++i)
  {
    std::string current_dict_name;
    if (!(in >> current_dict_name) || current_dict_name.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    auto it = avltree.find(current_dict_name);
    if (it == avltree.cend())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    auto it2 = it->second.find(eng_word);
    if (it2 != it->second.cend())
    {
      word_found = true;
      for (auto it3 = it2->second.cbegin(); it3 != it2->second.cend(); ++it3)
      {
        tree_translations[*it3];
      }
    }
  }
  if (!word_found)
  {
    throw std::logic_error("<INVALID WORD>");
  }
  else if (tree_translations.empty())
  {
    out << "\n";
  }
  else
  {
    out << tree_translations.begin()->first;
    for (auto it = ++tree_translations.begin(); it != tree_translations.end(); ++it)
    {
      out << " " << it->first;
    }
    out << "\n";
  }
}

void tkach::help(std::ostream& out)
{
  out << "import <file_name> <number_of_dictionaries> <dictionary_name1> ... - Импорт словарей из файла.\n";
  out << "addword <dictionary_name> <english_word> <number_of_translations> <translation1> ...";
  out << "- Добавление слова и его переводов в словарь.\n";
  out << "mergewords <dictionary_name> <english_word1> <english_word2>";
  out << "- Объединение переводов двух английских слов в одном словаре.\n";
  out << "printcommontranslations <dictionary_name> <number_of_words> <word1> ... ";
  out << "- Вывод общих переводов для нескольких слов в одном словаре.\n";
  out << "exportoverwrite <file_name> <number_of_dictionaries> <dictionary_name1> ... ";
  out << "- Экспорт словарей в файл, перезаписывая его.\n";
  out << "exportinend <file_name> <number_of_dictionaries> <dictionary_name1> ... ";
  out << "- Экспорт словарей в файл, добавляя в конец.\n";
  out << "printall - Вывод всех словарей и их содержимого.\n";
  out << "removeword <dictionary_name> <english_word> - Удаление слова из словаря.\n";
  out << "addtranslation <0_or_1> [dictionary_name] <english_word> <translation> ";
  out << "- Добавление перевода к слову.\n";
  out << "removetranslation <0_or_1> [dictionary_name] <english_word> <translation> - Удаление перевода слова.\n";
  out << "clear <dictionary_name> - Очистка словаря от всех слов.\n";
  out << "mergedictionaries <new_dictionary_name> <number_of_dictionaries> <dictionary_name1> ... ";
  out << "- Объединение нескольких словарей в новый.\n";
  out << "subtractedictionaries <new_dictionary_name> <number_of_dictionaries> <dictionary_name1> ... ";
  out << "- Вычитание словарей из первого в новый.\n";
  out << "commonpartdictionaries <new_dictionary_name> <number_of_dictionaries> <dictionary_name1> ... ";
  out << "- Создание нового словаря из общих слов нескольких словарей.\n";
  out << "copytranslations <source_dictionary_name> <english_word> <number_of_dictionaries> <target_dictionary_name1> ... ";
  out << "- Копирование переводов слова из одного словаря в другие.\n";
  out << "printtranslations <english_word> <number_of_dictionaries> <dictionary_name1> ... ";
  out << "- Вывод переводов слова в указанных словарях.\n";
  out << "printengwordswithtraslation <translation> <number_of_dictionaries> <dictionary_name1> ...  ";
  out << "- Вывод английских слов, содержащих этот перевод.\n";
  out << "count <dictionary_name> - Вывод количества слов в словаре.\n";
}

void tkach::printEngWordsWithTraslation(std::istream& in, std::ostream& out, const tree_of_dict& avltree)
{
  std::string translation;
  int number_of_dictionaries = 0;
  if (!(in >> translation) || translation.empty())
  {
    throw std::logic_error("<INVALID ARGUMENTS>");
  }
  if (!(in >> number_of_dictionaries) || (number_of_dictionaries <= 0))
  {
    throw std::logic_error("<INVALID NUMBER>");
  }
  AvlTree< std::string, std::string > tree_word;
  for (size_t i = 0; i < static_cast< size_t >(number_of_dictionaries); ++i)
  {
    std::string current_dict_name;
    if (!(in >> current_dict_name) || current_dict_name.empty())
    {
      throw std::logic_error("<INVALID ARGUMENTS>");
    }
    auto it = avltree.find(current_dict_name);
    if (it == avltree.cend())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); ++it2)
    {
      if (findTranslation(it2->second, translation))
      {
        tree_word[it2->first];
      }
    }
  }
  if (tree_word.empty())
  {
    throw std::logic_error("<NO WORD WITH THIS TRASLATION>");
  }
  else
  {
    out << tree_word.begin()->first;
    for (auto it = ++tree_word.begin(); it != tree_word.end(); ++it)
    {
      out << " " << it->first;
    }
    out << "\n";
  }
}
