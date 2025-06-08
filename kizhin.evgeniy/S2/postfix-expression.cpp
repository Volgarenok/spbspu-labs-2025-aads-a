#include "postfix-expression.hpp"
#include <stdexcept>
#include <stack.hpp>

kizhin::PostfixExpression::PostfixExpression(Queue< Token > tokens):
  tokens_(std::move(tokens))
{}

kizhin::PostfixExpression& kizhin::PostfixExpression::operator+=(
    const PostfixExpression& rhs)
{
  appendExpression(rhs, std::addressof(Addition::instance()));
  return *this;
}

kizhin::PostfixExpression& kizhin::PostfixExpression::operator-=(
    const PostfixExpression& rhs)
{
  appendExpression(rhs, std::addressof(Subtraction::instance()));
  return *this;
}

kizhin::PostfixExpression& kizhin::PostfixExpression::operator*=(
    const PostfixExpression& rhs)
{
  appendExpression(rhs, std::addressof(Multiplication::instance()));
  return *this;
}

kizhin::PostfixExpression& kizhin::PostfixExpression::operator/=(
    const PostfixExpression& rhs)
{
  appendExpression(rhs, std::addressof(Division::instance()));
  return *this;
}

size_t kizhin::PostfixExpression::size() const noexcept
{
  return tokens_.size();
}

bool kizhin::PostfixExpression::empty() const noexcept
{
  return tokens_.empty();
}

kizhin::PostfixExpression::number_type kizhin::PostfixExpression::evaluate() const
{
  Queue< Token > tokens = tokens_;
  Stack< number_type > operands;
  while (!tokens.empty()) {
    const Token token = tokens.front();
    tokens.pop();
    if (token.type() == TokenType::number) {
      operands.push(token.number());
    } else if (token.type() == TokenType::operation) {
      if (operands.size() < 2) {
        throw std::logic_error("Not enough operands");
      }
      const number_type right = operands.top();
      operands.pop();
      const number_type left = operands.top();
      operands.pop();
      operands.push(token.operation()->eval(left, right));
    } else {
      throw std::logic_error("Unexpected token type");
    }
  }
  if (operands.size() != 1) {
    throw std::logic_error("Invalid expression");
  }
  return operands.top();
}

void kizhin::PostfixExpression::clear() noexcept
{
  while (!tokens_.empty()) {
    tokens_.pop();
  }
}

void kizhin::PostfixExpression::swap(PostfixExpression& rhs) noexcept
{
  tokens_.swap(rhs.tokens_);
}

void kizhin::PostfixExpression::appendExpression(const PostfixExpression& rhs,
    const BinaryOperation* operation)
{
  Queue< Token > newTokens = tokens_;
  Queue< Token > rhsTokens = rhs.tokens_;
  while (!rhsTokens.empty()) {
    newTokens.push(rhsTokens.front());
    rhsTokens.pop();
  }
  newTokens.emplace(operation);
  using std::swap;
  swap(newTokens, tokens_);
}

