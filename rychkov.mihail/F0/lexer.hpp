#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#include <memory>
#include <set.hpp>
#include <map.hpp>
#include <variant.hpp>

#include "content.hpp"
#include "compare.hpp"
#include "log.hpp"
#include "cparser.hpp"

namespace rychkov
{
  class Lexer
  {
  public:
    static const Set< std::vector< Operator >, NameCompare > cases;
    std::unique_ptr< CParser > next;

    Lexer(std::unique_ptr< CParser > cparser = nullptr);

    void append_name(CParseContext& context, std::string name);
    void append_number(CParseContext& context, std::string name);
    void append_string_literal(CParseContext& context, std::string name);
    void append_char_literal(CParseContext& context, std::string name);
    void append(CParseContext& context, char c);
    void flush(CParseContext& context);

  private:
    using operator_value = const std::vector< Operator >*;

    Map< std::string, CParser::TypeKeyword > type_keywords_;
    Map< std::string, void(CParser::*)(CParseContext&) > keywords_;

    Variant< Monostate, operator_value, entities::Literal > buf_;

    void append_new(CParseContext& context, char c);
    void append_operator(CParseContext& context, char c);
  };
}

#endif
