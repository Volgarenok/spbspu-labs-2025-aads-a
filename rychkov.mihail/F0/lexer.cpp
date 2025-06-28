#include "lexer.hpp"

#include <iostream>
#include <cctype>
#include <utility>
#include "cparser.hpp"
#include "print_content.hpp"

rychkov::Lexer::Lexer(std::unique_ptr< CParser > cparser):
  next{std::move(cparser)},
  type_keywords_{
        {"const", CParser::CONST},
        {"volatile", CParser::VOLATILE},
        {"signed", CParser::SIGNED},
        {"unsigned", CParser::UNSIGNED},
        {"long", CParser::LONG}
      },
  keywords_{
        {"typedef", &CParser::parse_typedef},
        {"struct", &CParser::parse_struct},
        {"return", &CParser::parse_return},
        {"if", &CParser::parse_if},
        {"while", &CParser::parse_while}
      }
{}

const rychkov::Set< std::vector< rychkov::Operator >, rychkov::NameCompare > rychkov::Lexer::cases{
      {
        {Operator::UNARY, Operator::ARITHMETIC, "+", false, true, false, 2},
        {Operator::BINARY, Operator::ARITHMETIC, "+", false, false, false, 4}
      },
      {
        {Operator::UNARY, Operator::INCREMENT, "++", true, false, false, 1},
        {Operator::UNARY, Operator::INCREMENT, "++", true, true, true, 2},
      },
      {
        {Operator::UNARY, Operator::ARITHMETIC, "-", false, true, false, 2},
        {Operator::BINARY, Operator::ARITHMETIC, "-", false, false, false, 4}
      },
      {
        {Operator::UNARY, Operator::INCREMENT, "--", true, false, false, 1},
        {Operator::UNARY, Operator::INCREMENT, "--", true, true, true, 2},
      },
      {{Operator::BINARY, Operator::FIELD_ACCESS, ".", true, false, true, 1}},
      {{Operator::BINARY, Operator::FIELD_ACCESS, "->", false, false, true, 1}},
      {{Operator::UNARY, Operator::LOGIC, "!", false, true, false, 2}},
      {{Operator::UNARY, Operator::BIT, "~", false, true, false, 2}},
      {
        {Operator::UNARY, Operator::DEREFERENCE, "*", false, true, true, 2},
        {Operator::BINARY, Operator::ARITHMETIC, "*", false, false, false, 3}
      },
      {{Operator::BINARY, Operator::ARITHMETIC, "/", false, false, false, 3}},
      {{Operator::BINARY, Operator::MODULUS, "%", false, false, false, 3}},
      {{Operator::BINARY, Operator::BIT, "<<", false, false, false, 5}},
      {{Operator::BINARY, Operator::BIT, ">>", false, false, false, 5}},
      {{Operator::BINARY, Operator::COMPARE, ">", false, false, false, 6}},
      {{Operator::BINARY, Operator::COMPARE, ">=", false, false, false, 6}},
      {{Operator::BINARY, Operator::COMPARE, "<", false, false, false, 6}},
      {{Operator::BINARY, Operator::COMPARE, "<=", false, false, false, 6}},
      {{Operator::BINARY, Operator::COMPARE, "==", false, false, false, 7}},
      {{Operator::BINARY, Operator::COMPARE, "!=", false, false, false, 7}},
      {
        {Operator::UNARY, Operator::ADDRESSOF, "&", true, true, false, 2},
        {Operator::BINARY, Operator::BIT, "&", false, false, false, 8}
      },
      {{Operator::BINARY, Operator::BIT, "^", false, false, false, 9}},
      {{Operator::BINARY, Operator::BIT, "|", false, false, false, 10}},
      {{Operator::BINARY, Operator::LOGIC, "&&", false, false, false, 11}},
      {{Operator::BINARY, Operator::LOGIC, "||", false, false, false, 12}},
      {{Operator::BINARY, Operator::ARITHMETIC, "+=", true, true, true, 14}},
      {{Operator::BINARY, Operator::ARITHMETIC, "-=", true, true, true, 14}},
      {{Operator::BINARY, Operator::ARITHMETIC, "*=", true, true, true, 14}},
      {{Operator::BINARY, Operator::ARITHMETIC, "/=", true, true, true, 14}},
      {{Operator::BINARY, Operator::MODULUS, "%=", true, true, true, 14}},
      {{Operator::BINARY, Operator::BIT, "<<=", true, true, true, 14}},
      {{Operator::BINARY, Operator::BIT, ">>=", true, true, true, 14}},
      {{Operator::BINARY, Operator::BIT, "&=", true, true, true, 14}},
      {{Operator::BINARY, Operator::BIT, "^=", true, true, true, 14}},
      {{Operator::BINARY, Operator::BIT, "|=", true, true, true, 14}},
      {{Operator::BINARY, Operator::ASSIGN, "=", true, true, true, 14}}
    };

void rychkov::Lexer::append_name(CParseContext& context, std::string name)
{
  flush(context);
  decltype(type_keywords_)::const_iterator type_keyword_p = type_keywords_.find(name);
  if (type_keyword_p != type_keywords_.end())
  {
    if (next == nullptr)
    {
      context.out << "<keyw> " << name << '\n';
    }
    else
    {
      next->append(context, type_keyword_p->second);
    }
    return;
  }
  decltype(keywords_)::const_iterator keyword_p = keywords_.find(name);
  if (keyword_p != keywords_.cend())
  {
    if (next == nullptr)
    {
      context.out << "<keyw> " << name << '\n';
    }
    else
    {
      ((*next).*(keyword_p->second))(context);
    }
    return;
  }

  if (next == nullptr)
  {
    context.out << "<name> " << name << '\n';
  }
  else
  {
    next->append(context, std::move(name));
  }
}
void rychkov::Lexer::append_number(CParseContext& context, std::string name)
{
  flush(context);
  bool is_int = true;
  entities::Literal lit = {{}, {}, entities::Literal::Number};
  std::string::size_type i = 0;
  for (; i < name.length(); i++)
  {
    if (name[i] == '.')
    {
      if (!is_int)
      {
        log(context, "point duplicates in numeric literal");
        continue;
      }
      is_int = false;
      lit.literal += name[i];
      continue;
    }
    if (name[i] == '\'')
    {
      continue;
    }
    if (!std::isdigit(name[i]))
    {
      break;
    }
    lit.literal += name[i];
  }
  lit.suffix = name.substr(i, name.length() - i);

  if (is_int)
  {
    lit.result_type = {"int", typing::BASIC};
  }
  else
  {
    lit.result_type = {"double", typing::BASIC};
  }

  if ((lit.suffix == "f") || (lit.suffix == "F"))
  {
    lit.result_type.name = "float";
  }
  else if (!is_int)
  {
    if (!lit.suffix.empty())
    {
      log(context, "unknown numeric suffix - " + lit.suffix);
      lit.suffix.clear();
    }
  }
  else if ((lit.suffix == "l") || (lit.suffix == "L"))
  {
    lit.result_type.length_category = typing::LONG;
  }
  else if ((lit.suffix == "ul") || (lit.suffix == "UL"))
  {
    lit.result_type.length_category = typing::LONG;
    lit.result_type.is_unsigned = true;
  }
  else if ((lit.suffix == "ll") || (lit.suffix == "LL"))
  {
    lit.result_type.length_category = typing::LONG_LONG;
  }
  else if ((lit.suffix == "ull") || (lit.suffix == "ULL"))
  {
    lit.result_type.length_category = typing::LONG_LONG;
    lit.result_type.is_unsigned = true;
  }
  else if (!lit.suffix.empty())
  {
    log(context, "unknown numeric suffix - " + lit.suffix);
    lit.suffix.clear();
  }

  if (next == nullptr)
  {
    context.out << "<numb> " << lit << '\n';
  }
  else
  {
    next->append(context, lit);
  }
}
void rychkov::Lexer::append_string_literal(CParseContext& context, std::string name)
{
  flush(context);
  name.erase(name.length() - 1);
  name.erase(0, 1);
  typing::Type type = {{}, typing::ARRAY, {"char", typing::BASIC, nullptr, true}};
  buf_ = entities::Literal{name, {}, entities::Literal::String, std::move(type)};
}
void rychkov::Lexer::append_char_literal(CParseContext& context, std::string name)
{
  flush(context);
  name.erase(name.length() - 1);
  name.erase(0, 1);
  buf_ = entities::Literal{name, {}, entities::Literal::String, {"char", typing::BASIC, nullptr, true}};
}
void rychkov::Lexer::flush(CParseContext& context)
{
  if (holds_alternative< entities::Literal >(buf_))
  {
    entities::Literal& lit = get< entities::Literal >(buf_);
    if (lit.type == entities::Literal::String)
    {
      lit.result_type.array_has_length = true;
      lit.result_type.array_length = lit.literal.length() + 1;
    }

    if (next == nullptr)
    {
      context.out << "<lit>  " << lit << '\n';
    }
    else
    {
      next->append(context, lit);
    }
  }
  else if (holds_alternative< operator_value >(buf_))
  {
    if (next == nullptr)
    {
      context.out << "<oper> " << (*get< operator_value >(buf_))[0].token << '\n';
    }
    else
    {
      next->append(context, *get< operator_value >(buf_));
    }
  }
  buf_.emplace< 0 >();
}
void rychkov::Lexer::append(CParseContext& context, char c)
{
  if (holds_alternative< operator_value >(buf_))
  {
    append_operator(context, c);
    return;
  }
  flush(context);
  append_new(context, c);
}
void rychkov::Lexer::append_new(CParseContext& context, char c)
{
  decltype(cases)::const_iterator oper_p = cases.find(std::string{c});
  if (oper_p != cases.end())
  {
    buf_ = &*oper_p;
  }
  else if (!std::isspace(c))
  {
    if (next == nullptr)
    {
      context.out << "<spec> " << c << '\n';
      return;
    }
    next->append(context, c);
  }
}
void rychkov::Lexer::append_operator(CParseContext& context, char c)
{
  operator_value& oper = get< operator_value >(buf_);
  decltype(cases)::const_iterator oper_p = cases.find((*oper)[0].token + c);
  if (oper_p != cases.cend())
  {
    buf_ = &*oper_p;
    return;
  }
  flush(context);
  append(context, c);
}
