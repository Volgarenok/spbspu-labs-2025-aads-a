#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>
#include <utility>
#include "map.hpp"

namespace rychkov
{
  struct ParserContext
  {
    std::istream& in;
    std::ostream& out;
    std::ostream& err;

    bool eol();
  };
  template< class Proc >
  class Parser
  {
  public:
    using parser_processor = Proc;
    using call_signature = bool(parser_processor::*)(ParserContext&);
    using map_type = rychkov::Map< std::string, call_signature >;

    parser_processor processor;
    ParserContext context;

    template< class P = parser_processor, class M = map_type >
    Parser(ParserContext parse_context, P proc, M&& call_map):
      processor(std::forward< P >(proc)),
      context(parse_context),
      call_map_(std::forward< M >(call_map))
    {}

    bool available() const noexcept
    {
      return !call_map_.empty() && !context.in.eof() && !context.in.bad();
    }
    bool run()
    {
      if (available() && context.in.fail())
      {
        context.in.clear();
      }
      std::string command;
      if (context.in >> command)
      {
        typename map_type::const_iterator call_p = call_map_.find(command);
        try
        {
          if ((call_p != call_map_.end()) && ((processor.*(call_p->second))(context)))
          {
            return available();
          }
        }
        catch (...)
        {}
        context.out << "<INVALID COMMAND>\n";
        context.in.clear(context.in.rdstate() & ~std::ios::failbit);
        context.in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
      return available();
    }
  private:
    const map_type call_map_;
  };
}

#endif
