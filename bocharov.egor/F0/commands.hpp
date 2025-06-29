#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <tree.hpp>
#include <list.hpp>

namespace bocharov
{
    using list_t = List<std::string>;
    using dict_t = Tree<std::string, list_t>;
    using dict_dict_t = Tree<std::string, dict_t>;

    void printHelp(std::ostream& out);
    void createDict(std::istream& in, dict_dict_t& dicts);
    void deleteDict(std::istream& in, dict_dict_t& dicts);
    void printDict(std::istream& in, std::ostream& out, const dict_dict_t& dicts);
    void getTranslationSln(std::istream& in, std::ostream& out, const dict_dict_t& dicts);
    void getTranslationRu(std::istream& in, std::ostream& out, const dict_dict_t& dicts);
    void deleteSln(std::istream& in, dict_dict_t& dicts);
    void addSln(std::istream& in, dict_dict_t& dicts);
    void deleteRu(std::istream& in, dict_dict_t& dicts);
    void addRu(std::istream& in, dict_dict_t& dicts);
    void writeDicts(std::istream& in, const dict_dict_t& dicts);
    void makeUnion(std::istream& in, dict_dict_t& dicts);
    void printMostCommon(std::istream& in, std::ostream& out, const dict_dict_t& dicts);
    void rewriteFile(std::istream& in, const dict_dict_t& dicts);
    void rare(std::istream& in, dict_dict_t& dicts);
}

#endif
