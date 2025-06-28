#include "main_processor.hpp"

#include <iostream>
#include <algorithm.hpp>
#include <set.hpp>
#include <map.hpp>
#include "compare.hpp"
#include "print_content.hpp"

namespace rychkov
{
  struct DependencyVisitor
  {
    std::ostream& out;
    std::string symbol;
    bool search_uses = false;
    bool started = false;
    Set< std::pair< std::string, size_t >, NameCompare > actives;
    std::string current;
    Map< std::string, Set< entities::Variable, NameCompare > > dep_map;
    Set< entities::Variable, NameCompare > result;
    size_t depth = 0;

    bool operator()(const typing::Type& type);
    bool operator()(const entities::Variable& var);
    bool operator()(const entities::Declaration& decl);
    bool operator()(const entities::Literal& literal);
    bool operator()(const entities::CastOperation& cast);
    bool operator()(const DynMemWrapper< entities::Expression >& root);
    bool operator()(const entities::Expression::operand& operand);
    bool operator()(const entities::Expression& root);
    bool operator()(const entities::Body& body);
    template< class Entity >
    bool operator()(const std::vector< Entity >& sequence);

    void prepare_deps(const decltype(dep_map)::mapped_type& level);
  };
}

bool rychkov::MainProcessor::dependencies(ParserContext& context)
{
  std::string filename, symbol;
  if ((last_stage_ != CPARSER) || !(context.in >> filename >> symbol))
  {
    return false;
  }
  const CParser& parser = *parsed_.at(filename).preproc.next->next;
  if (!eol(context.in))
  {
    return false;
  }
  DependencyVisitor visitor = for_each(parser.begin(), parser.end(),
        DependencyVisitor{context.out, std::move(symbol)});
  if (!visitor.started)
  {
    context.out << "no symbol \"" << visitor.symbol << "\" in file \"" << filename << "\"\n";
  }
  else
  {
    context.out << "depend from:\n";
    visitor.prepare_deps(visitor.dep_map[visitor.symbol]);
    for_each(visitor.result.begin(), visitor.result.end(), ContentPrinter{context.out, 1});
  }
  return true;
}
bool rychkov::MainProcessor::uses(ParserContext& context)
{
  std::string filename, symbol;
  if ((last_stage_ != CPARSER) || !(context.in >> filename >> symbol))
  {
    return false;
  }
  const CParser& parser = *parsed_.at(filename).preproc.next->next;
  if (!eol(context.in))
  {
    return false;
  }
  if (!for_each(parser.begin(), parser.end(), DependencyVisitor{context.out, symbol, true}).started)
  {
    context.out << "no symbol \"" << symbol << "\" in file \"" << filename << "\"\n";
  }
  return true;
}

bool rychkov::DependencyVisitor::operator()(const entities::Variable& var)
{
  if (actives.find(var.name) == actives.end())
  {
    if (search_uses)
    {
      return var.name == symbol;
    }
    dep_map[current].insert(var);
  }
  return false;
}
bool rychkov::DependencyVisitor::operator()(const typing::Type& type)
{
  if (!search_uses)
  {
    return false;
  }
  if (typing::is_function(&type))
  {
    if (operator()(*type.base))
    {
      return true;
    }
    for (const typing::Type& param: type.function_parameters)
    {
      if (operator()(param))
      {
        return true;
      }
    }
    return false;
  }
  return type.base == nullptr ? type.name == symbol : operator()(*type.base);
}
bool rychkov::DependencyVisitor::operator()(const entities::Declaration& decl)
{
  ++depth;
  if (holds_alternative< entities::Statement >(decl.data))
  {
    const entities::Statement& statement = get< entities::Statement >(decl.data);
    if (operator()(statement.conditions))
    {
      CParser::clear_scope(actives, --depth);
      return true;
    }
  }
  else if (holds_alternative< entities::Variable >(decl.data))
  {
    const entities::Variable& var = get< entities::Variable >(decl.data);
    if (depth == 1)
    {
      current = var.name;
      if (var.name == symbol)
      {
        out << var << '\n';
        started = true;
        if (!search_uses)
        {
          actives.emplace(var.name, depth - 1);
          operator()(decl.value);
          CParser::clear_scope(actives, --depth);
          return true;
        }
      }
      else if (!started && search_uses)
      {
        CParser::clear_scope(actives, --depth);
        return false;
      }
    }
    else
    {
      actives.emplace(var.name, depth - 1);
    }
    if (search_uses && operator()(var.type))
    {
      if (--depth == 0)
      {
        out << '\t' << var << '\n';
      }
      return true;
    }
  }
  else if (holds_alternative< entities::Function >(decl.data))
  {
    const entities::Function& func = get< entities::Function >(decl.data);
    for (const std::string& param: func.parameters)
    {
      if (!param.empty())
      {
        actives.emplace(param, depth);
      }
    }
    if (depth == 1)
    {
      current = func.name;
      if (func.name == symbol)
      {
        out << func << '\n';
        started = true;
        if (!search_uses)
        {
          actives.emplace(func.name, depth - 1);
        }
      }
      else if (!started && search_uses)
      {
        CParser::clear_scope(actives, --depth);
        return false;
      }
    }
    else
    {
      actives.emplace(func.name, depth - 1);
    }
    if (search_uses && operator()(func.type))
    {
      if (--depth == 0)
      {
        out << '\t' << func << '\n';
      }
      return true;
    }
  }
  else if (search_uses)
  {
    const std::string* name = nullptr;
    if (holds_alternative< entities::Struct >(decl.data))
    {
      name = &get< entities::Struct >(decl.data).name;
    }
    else if (holds_alternative< entities::Enum >(decl.data))
    {
      name = &get< entities::Enum >(decl.data).name;
    }
    else if (holds_alternative< entities::Union >(decl.data))
    {
      name = &get< entities::Union >(decl.data).name;
    }
    if (name != nullptr)
    {
      if (depth == 1)
      {
        if (*name == symbol)
        {
          out << "struct-like " << *name << '\n';
          started = true;
        }
      }
      else
      {
        actives.emplace(*name, depth - 1);
      }
    }
  }
  if (operator()(decl.value))
  {
    if (depth > 1)
    {
      CParser::clear_scope(actives, --depth);
      return true;
    }
    const std::string* name = nullptr;
    if (search_uses && holds_alternative< entities::Variable >(decl.data))
    {
      const entities::Variable& var = get< entities::Variable >(decl.data);
      out << '\t' << var << '\n';
    }
    else if (search_uses && holds_alternative< entities::Function >(decl.data))
    {
      const entities::Function& func = get< entities::Function >(decl.data);
      out << '\t' << func << '\n';
    }
    else if (holds_alternative< entities::Struct >(decl.data))
    {
      name = &get< entities::Struct >(decl.data).name;
    }
    else if (holds_alternative< entities::Enum >(decl.data))
    {
      name = &get< entities::Enum >(decl.data).name;
    }
    else if (holds_alternative< entities::Union >(decl.data))
    {
      name = &get< entities::Union >(decl.data).name;
    }
    if (name != nullptr)
    {
      out << "\tstruct-like " << *name << '\n';
    }
  }
  CParser::clear_scope(actives, --depth);
  return false;
}
bool rychkov::DependencyVisitor::operator()(const entities::Literal&)
{
  return false;
}
bool rychkov::DependencyVisitor::operator()(const entities::CastOperation& cast)
{
  return operator()(cast.expr);
}
bool rychkov::DependencyVisitor::operator()(const DynMemWrapper< entities::Expression >& root)
{
  return (root != nullptr) && operator()(*root);
}
bool rychkov::DependencyVisitor::operator()(const entities::Expression::operand& operand)
{
  return visit(*this, operand);
}
bool rychkov::DependencyVisitor::operator()(const entities::Expression& root)
{
  for (const entities::Expression::operand& operand: root.operands)
  {
    if (operator()(operand))
    {
      return true;
    }
  }
  return false;
}
bool rychkov::DependencyVisitor::operator()(const entities::Body& body)
{
  return operator()(body.data);
}
template< class Entity >
bool rychkov::DependencyVisitor::operator()(const std::vector< Entity >& sequence)
{
  ++depth;
  for (const Entity& operand: sequence)
  {
    if (operator()(operand))
    {
      CParser::clear_scope(actives, --depth);
      return true;
    }
  }
  CParser::clear_scope(actives, --depth);
  return false;
}
void rychkov::DependencyVisitor::prepare_deps(const decltype(dep_map)::mapped_type& level)
{
  for (const entities::Variable& var: level)
  {
    decltype(result)::iterator existing = result.find(var);
    if (existing == result.end())
    {
      result.insert(var);
      prepare_deps(dep_map[var.name]);
    }
  }
}
