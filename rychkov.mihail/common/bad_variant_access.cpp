#include "variant/class/access.hpp"

rychkov::bad_variant_access::bad_variant_access() noexcept:
  data_("bad variant access")
{}
const char* rychkov::bad_variant_access::what() const noexcept
{
  return data_;
}
rychkov::bad_variant_access::bad_variant_access(const char* message) noexcept:
  data_(message)
{}

void rychkov::details::throw_bad_variant_access(const char* message)
{
  throw bad_variant_access(message);
}
void rychkov::details::throw_bad_variant_access(bool valueless)
{
  if (valueless)
  {
    throw_bad_variant_access("get(): variant is valueless");
  }
  else
  {
    throw_bad_variant_access("get(): variant active index don't matches requested type");
  }
}
