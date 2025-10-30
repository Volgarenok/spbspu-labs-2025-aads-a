#ifndef CROSSREFERENCESYSTEM_HPP
#define CROSSREFERENCESYSTEM_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>
#include <optional>

namespace hismatova
{
  void print_help();
  void print_welcome();


  class CrossReferenceSystem
  {
  private:
    std::set< std::string > documents;
    std::map< std::string, std::map< std::string, std::string > > termToDoc;
    std::map< std::string, std::set< std::string > > docToTerms;

  public:
    bool create_doc(const std::string& doc_id)
    {
      return documents.insert(doc_id).second;
    }

    bool delete_doc(const std::string& doc_id)
    {
      auto doc_it = documents.find(doc_id);
      if (doc_it == documents.end())
      {
        return false;
      }

      documents.erase(doc_it);

      auto doc_terms_it = docToTerms.find(doc_id);
      if (doc_terms_it != docToTerms.end())
      {
        std::for_each(doc_terms_it->second.begin(), doc_terms_it->second.end(),
          [this, doc_id](const std::string& term_id)
          {
            auto term_it = termToDoc.find(term_id);
            if (term_it != termToDoc.end())
            {
              term_it->second.erase(doc_id);
              if (term_it->second.empty())
              {
                termToDoc.erase(term_it);
              }
            }
          });
        docToTerms.erase(doc_terms_it);
      }
      return true;
    }

    bool doc_exists(const std::string& doc_id) const
    {
      return documents.find(doc_id) != documents.end();
    }

    std::vector< std::string > list_docs() const
    {
      return std::vector< std::string >(documents.begin(), documents.end());
    }

    bool add_xref(const std::string& term_id, const std::string& doc_id, const std::string& data)
    {
      if (!doc_exists(doc_id))
      {
        return false;
      }

      auto& term_docs = termToDoc[term_id];
      auto result = term_docs.insert(std::make_pair(doc_id, data));

      if (!result.second)
      {
        return false;
      }

      docToTerms[doc_id].insert(term_id);
      return true;
    }

    bool remove_xref(const std::string& term_id, const std::string& doc_id)
    {
      auto term_it = termToDoc.find(term_id);
      if (term_it == termToDoc.end())
      {
        return false;
      }

      auto doc_ref_it = term_it->second.find(doc_id);
      if (doc_ref_it == term_it->second.end())
      {
        return false;
      }

      term_it->second.erase(doc_ref_it);
      if (term_it->second.empty())
      {
        termToDoc.erase(term_it);
      }

      auto doc_terms_it = docToTerms.find(doc_id);
      if (doc_terms_it != docToTerms.end())
      {
        doc_terms_it->second.erase(term_id);
        if (doc_terms_it->second.empty())
        {
          docToTerms.erase(doc_terms_it);
        }
      }
      return true;
    }

    bool has_xref(const std::string& term_id, const std::string& doc_id) const
    {
      auto term_it = termToDoc.find(term_id);
      if (term_it == termToDoc.end())
      {
        return false;
      }
      return term_it->second.find(doc_id) != term_it->second.end();
    }

    std::optional< std::string > get_xref_data(const std::string& term_id, const std::string& doc_id) const
    {
      auto term_it = termToDoc.find(term_id);
      if (term_it == termToDoc.end())
      {
        return std::nullopt;
      }

      auto doc_it = term_it->second.find(doc_id);
      if (doc_it == term_it->second.end())
      {
        return std::nullopt;
      }

      return doc_it->second;
    }

    std::vector< std::string > list_xrefs_from_term(const std::string& term_id) const
    {
      auto term_it = termToDoc.find(term_id);
      if (term_it == termToDoc.end() || term_it->second.empty())
      {
        return {};
      }

      std::vector< std::string > result;
      std::transform(term_it->second.begin(), term_it->second.end(),
        std::back_inserter(result),
        [](const auto& pair)
        {
          return pair.first;
        });
      return result;
    }

    std::optional< std::vector< std::string > > list_xrefs_to_doc(const std::string& doc_id) const
    {
      if (!doc_exists(doc_id))
      {
        return std::nullopt;
      }

      auto doc_it = docToTerms.find(doc_id);
      if (doc_it == docToTerms.end() || doc_it->second.empty())
      {
        return std::vector< std::string >{};
      }

      return std::vector< std::string >(doc_it->second.begin(), doc_it->second.end());
    }

    size_t count_xrefs_from_term(const std::string& term_id) const
    {
      auto term_it = termToDoc.find(term_id);
      if (term_it == termToDoc.end())
      {
        return 0;
      }
      return term_it->second.size();
    }

    std::optional< size_t > count_xrefs_to_doc(const std::string& doc_id) const
    {
      if (!doc_exists(doc_id))
      {
        return std::nullopt;
      }

      auto doc_it = docToTerms.find(doc_id);
      if (doc_it == docToTerms.end())
      {
        return 0;
      }
      return doc_it->second.size();
    }

    bool purge_term(const std::string& term_id)
    {
      auto term_it = termToDoc.find(term_id);
      if (term_it == termToDoc.end())
      {
        return false;
      }

      std::for_each(term_it->second.begin(), term_it->second.end(),
        [this, term_id](const std::pair< std::string, std::string >& doc_ref)
        {
          auto doc_it = docToTerms.find(doc_ref.first);
          if (doc_it != docToTerms.end())
          {
            doc_it->second.erase(term_id);
            if (doc_it->second.empty())
            {
              docToTerms.erase(doc_it);
            }
          }
        });

      termToDoc.erase(term_it);
      return true;
    }

    void purge_dead_xrefs()
    {
      std::vector< std::string > terms_to_remove;

      for (auto term_it = termToDoc.begin(); term_it != termToDoc.end(); term_it++)
      {
        std::vector< std::string > docs_to_remove;

        std::for_each(term_it->second.begin(), term_it->second.end(),
          [this, &docs_to_remove](const std::pair< std::string, std::string >& doc_ref)
          {
            if (!doc_exists(doc_ref.first))
            {
              docs_to_remove.push_back(doc_ref.first);
            }
          });

        std::for_each(docs_to_remove.begin(), docs_to_remove.end(),
          [this, &term_it](const std::string& doc_id)
          {
            term_it->second.erase(doc_id);
          });

        if (term_it->second.empty())
        {
          terms_to_remove.push_back(term_it->first);
        }
      }

      std::for_each(terms_to_remove.begin(), terms_to_remove.end(),
        [this](const std::string& term_id)
        {
          termToDoc.erase(term_id);
        });

      for (auto doc_it = docToTerms.begin(); doc_it != docToTerms.end(); )
      {
        if (!doc_exists(doc_it->first))
        {
          doc_it = docToTerms.erase(doc_it);
        }
        else
        {
          doc_it++;
        }
      }
    }

    std::optional< bool > term_integrity_check(const std::string& term_id) const
    {
      auto term_it = termToDoc.find(term_id);
      if (term_it == termToDoc.end())
      {
        return std::nullopt;
      }

      bool integrity_ok = std::all_of(term_it->second.begin(), term_it->second.end(),
        [this](const std::pair< std::string, std::string >& doc_ref)
        {
          return doc_exists(doc_ref.first);
        });

      return integrity_ok;
    }

    std::optional< bool > doc_integrity_check(const std::string& doc_id) const
    {
      if (!doc_exists(doc_id))
      {
        return std::nullopt;
      }

      auto doc_terms_it = docToTerms.find(doc_id);
      if (doc_terms_it == docToTerms.end())
      {
        return true;
      }

      bool integrity_ok = std::all_of(doc_terms_it->second.begin(), doc_terms_it->second.end(),
        [this, doc_id](const std::string& term_id)
        {
          auto term_it = termToDoc.find(term_id);
          return term_it != termToDoc.end() &&
            term_it->second.find(doc_id) != term_it->second.end();
        });

      return integrity_ok;
    }

    bool save_to_file(const std::string& filename) const
    {
      std::ofstream file(filename);
      if (!file.is_open())
      {
        return false;
      }

      file << "DOCUMENTS:" << std::endl;
      std::for_each(documents.begin(), documents.end(),
        [&file](const std::string& doc_id)
        {
          file << doc_id << std::endl;
        });
\
      file << "REFERENCES:" << std::endl;
      std::for_each(termToDoc.begin(), termToDoc.end(),
        [this, &file](const std::pair< std::string, std::map< std::string, std::string > >& term)
        {
          std::for_each(term.second.begin(), term.second.end(),
            [this, &file, &term](const std::pair< std::string, std::string >& doc_ref)
            {
              file << term.first << "|" << doc_ref.first << "|" << doc_ref.second << std::endl;
            });
        });

      return true;
    }


    bool load_from_file(const std::string& filename)
    {
      std::ifstream file(filename);
      if (!file.is_open())
      {
        return false;
      }

      documents.clear();
      termToDoc.clear();
      docToTerms.clear();

      std::string line;
      bool reading_documents = false;
      bool reading_references = false;

      std::vector< std::string > lines;
      std::copy(std::istream_iterator<std::string>(file),
        std::istream_iterator<std::string>(),
        std::back_inserter(lines));

      std::for_each(lines.begin(), lines.end(), [this, &reading_references, &reading_documents]
      (const std::string& line)
        {
          if (line == "DOCUMENTS:")
          {
            reading_documents = true;
            reading_references = false;
          }
          else if (line == "REFERENCES:")
          {
            reading_documents = false;
            reading_references = true;
          }
          else if (reading_documents && !line.empty())
          {
            documents.insert(line);
          }
          else if (reading_references && !line.empty())
          {
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);

            if (pos1 != std::string::npos && pos2 != std::string::npos)
            {
              std::string term_id = line.substr(0, pos1);
              std::string doc_id = line.substr(pos1 + 1, pos2 - pos1 - 1);
              std::string data = line.substr(pos2 + 1);

              termToDoc[term_id][doc_id] = data;
              docToTerms[doc_id].insert(term_id);
            }
          }
        });
      return true;
    }
  };


  class CommandProcessor
  {
  private:
    CrossReferenceSystem* system;

  public:
    CommandProcessor(CrossReferenceSystem* sys):
      system(sys)
    {}

    void process_command(const std::vector< std::string >& tokens)
    {
      if (tokens.empty() || !system)
      {
        return;
      }

      const std::string_view command = tokens[0];

      if (command == "create_doc" && tokens.size() == 2)
      {
        if (!system->create_doc(tokens[1]))
        {
          std::cout << "<INVALID COMMAND>" << std::endl;
        }
      }

      else if (command == "delete_doc" && tokens.size() == 2)
      {
        if (!system->delete_doc(tokens[1]))
        {
          std::cout << "<INVALID COMMAND>" << std::endl;
        }
      }

      else if (command == "doc_exists" && tokens.size() == 2)
      {
        std::cout << (system->doc_exists(tokens[1]) ? "true": "false") << std::endl;
      }

      else if (command == "list_docs" && tokens.size() == 1)
      {
        auto docs = system->list_docs();
        if (docs.empty())
        {
          std::cout << "<EMPTY>" << std::endl;
        }
        else
        {
          std::for_each(docs.begin(), docs.end(), [](const auto& doc)
            {
              std::cout << doc << std::endl;
            });
        }
      }
      else if (command == "add_xref" && tokens.size() >= 4)
      {
        std::string data;
        if (tokens.size() > 3)
        {
          std::for_each(tokens.begin() + 3, tokens.end(), [&data, i = 3](const auto& token) mutable
          {
            if (i++ > 3)
            {
              data += " ";
            }
            data += token;
          });
        }
        if (!system->add_xref(tokens[1], tokens[2], data))
        {
          std::cout << "<INVALID COMMAND>" << std::endl;
        }
      }

      else if (command == "remove_xref" && tokens.size() == 3)
      {
        if (!system->remove_xref(tokens[1], tokens[2]))
        {
          std::cout << "<INVALID COMMAND>" << std::endl;
        }
      }

      else if (command == "has_xref" && tokens.size() == 3)
      {
        std::cout << (system->has_xref(tokens[1], tokens[2]) ? "true": "false") << std::endl;
      }

      else if (command == "get_xref_data" && tokens.size() == 3)
      {
        auto data = system->get_xref_data(tokens[1], tokens[2]);
        std::cout << (data ? *data: "<EMPTY>") << std::endl;
      }

      else if (command == "list_xrefs_from_term" && tokens.size() == 2)
      {
        auto docs = system->list_xrefs_from_term(tokens[1]);
        if (docs.empty())
        {
          std::cout << "<EMPTY>" << std::endl;
        }
        else
        {
          std::for_each(docs.begin(), docs.end(), [](const auto& doc)
            {
              std::cout << doc << std::endl;
            });
        }
      }
      else if (command == "list_xrefs_to_doc" && tokens.size() == 2)
      {
        auto terms = system->list_xrefs_to_doc(tokens[1]);
        if (!terms)
        {
          std::cout << "<INVALID COMMAND>" << std::endl;
        }
        else if (terms->empty())
        {
          std::cout << "<EMPTY>" << std::endl;
        }
        else
        {
          std::for_each(terms->begin(), terms->end(), [](const auto& term)
            {
              std::cout << term << std::endl;
            });
        }
      }

      else if (command == "count_xrefs_from_term" && tokens.size() == 2)
      {
        std::cout << system->count_xrefs_from_term(tokens[1]) << std::endl;
      }

      else if (command == "count_xrefs_to_doc" && tokens.size() == 2)
      {
        auto count = system->count_xrefs_to_doc(tokens[1]);
        std::cout << (count ? std::to_string(*count): "<INVALID COMMAND>") << std::endl;
      }

      else if (command == "purge_term" && tokens.size() == 2)
      {
        if (!system->purge_term(tokens[1])) std::cout << "<INVALID COMMAND>" << std::endl;
      }

      else if (command == "purge_dead_xrefs" && tokens.size() == 1)
      {
        system->purge_dead_xrefs();
      }

      else if (command == "term_integrity_check" && tokens.size() == 2)
      {
        auto result = system->term_integrity_check(tokens[1]);
        std::cout << (!result ? "<INVALID COMMAND>":
          (*result ? "INTEGRITY OK": "INTEGRITY CORRUPTED")) << std::endl;
      }

      else if (command == "doc_integrity_check" && tokens.size() == 2)
      {
        auto result = system->doc_integrity_check(tokens[1]);
        std::cout << (!result ? "<INVALID COMMAND>":
          (*result ? "INTEGRITY OK": "INTEGRITY CORRUPTED")) << std::endl;
      }

      else if (command == "save" && tokens.size() == 2)
      {
        if (system->save_to_file(tokens[1]))
        {
          std::cout << "Database saved to " << tokens[1] << std::endl;
        }
        else
        {
          std::cout << "Error: Could not save database" << std::endl;
        }
      }
      else if (command == "--help")
      {
        print_help();
      }
      else
      {
        std::cout << "<INVALID COMMAND>" << std::endl;
      }
    }
  };


  class CommandLineParser
  {
  public:
    struct ParsedArgs
    {
      bool should_exit = false;
      bool success = true;
      std::string load_filename;
      std::string check_filename;
      bool show_help = false;
      std::string error_message;
    };

    static ParsedArgs parse(int argc, char* argv[], CrossReferenceSystem& system)
    {
      ParsedArgs result;

      for (int i = 1; i < argc; i++)
      {
        std::string arg = argv[i];

        if (arg == "--load" && i + 1 < argc)
        {
          result.load_filename = argv[++i];
          if (!system.load_from_file(result.load_filename))
          {
            result.error_message = "Error: Could not load database from file " + result.load_filename;
            result.success = false;
          }
          else
          {
            std::cout << "Database loaded from " << result.load_filename << std::endl;
          }
        }
        else if (arg == "--check" && i + 1 < argc)
        {
          result.check_filename = argv[++i];
          CrossReferenceSystem test_system;
          if (test_system.load_from_file(result.check_filename))
          {
            std::cout << "Database file " << result.check_filename << " is valid" << std::endl;
          }
          else
          {
            std::cout << "Database file " << result.check_filename << " is invalid" << std::endl;
          }
          result.should_exit = true;
        }
        else if (arg == "--help")
        {
          result.show_help = true;
          result.should_exit = true;
          print_help();
        }
        else
        {
          result.error_message = "Unknown parameter: " + arg;
          result.success = false;
        }
      }

      return result;
    }
  };


  void print_help()
  {
    std::cout << "Cross-Reference Database System" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  --load <file>    Load database from file" << std::endl;
    std::cout << "  --check <file>   Check database file validity" << std::endl;
    std::cout << "  --help           Show this help" << std::endl;
    std::cout << std::endl;
    std::cout << "Interactive Commands:" << std::endl;
    std::cout << "  create_doc <doc_id>" << std::endl;
    std::cout << "  delete_doc <doc_id>" << std::endl;
    std::cout << "  doc_exists <doc_id>" << std::endl;
    std::cout << "  list_docs" << std::endl;
    std::cout << "  add_xref <term_id> <doc_id> <data>" << std::endl;
    std::cout << "  remove_xref <term_id> <doc_id>" << std::endl;
    std::cout << "  has_xref <term_id> <doc_id>" << std::endl;
    std::cout << "  get_xref_data <term_id> <doc_id>" << std::endl;
    std::cout << "  list_xrefs_from_term <term_id>" << std::endl;
    std::cout << "  list_xrefs_to_doc <doc_id>" << std::endl;
    std::cout << "  count_xrefs_from_term <term_id>" << std::endl;
    std::cout << "  count_xrefs_to_doc <doc_id>" << std::endl;
    std::cout << "  purge_term <term_id>" << std::endl;
    std::cout << "  purge_dead_xrefs" << std::endl;
    std::cout << "  term_integrity_check <term_id>" << std::endl;
    std::cout << "  doc_integrity_check <doc_id>" << std::endl;
    std::cout << "  save <filename>   Save database to file" << std::endl;
    std::cout << "  exit              Exit program" << std::endl;
  }


  void print_welcome()
  {
    std::cout << "Cross-Reference Database System" << std::endl;
    std::cout << "Type commands or 'exit' to quit" << std::endl;
  }
}
#endif
