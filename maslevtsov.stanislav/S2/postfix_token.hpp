#ifndef POSTFIX_TOKEN_HPP
#define POSTFIX_TOKEN_HPP

#include <string>
#include "queue.hpp"

namespace maslevtsov {
  class PostfixToken
  {
  public:
    PostfixToken() noexcept = default;
    PostfixToken(const PostfixToken& rhs) = default;
    PostfixToken(PostfixToken&& rhs) noexcept = default;
    PostfixToken(const std::string& infix_token);
    ~PostfixToken() = default;

    PostfixToken& operator=(const PostfixToken& rhs) = default;
    PostfixToken& operator=(PostfixToken&& rhs) noexcept = default;

    PostfixToken operator+(const PostfixToken& other) const;
    PostfixToken operator-(const PostfixToken& other) const;
    PostfixToken operator*(const PostfixToken& other) const;
    PostfixToken operator/(const PostfixToken& other) const;
    PostfixToken operator%(const PostfixToken& other) const;
    long long operator()() const;

  private:
    Queue< std::string > token_;

    PostfixToken& math_operator_impl(const PostfixToken& other, const std::string& operation);
  };
}

#endif
