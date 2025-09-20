#ifndef PRIME_HPP
#define PRIME_HPP

#include <cstddef>

namespace averenkov
{
  bool is_prime(size_t n) noexcept;
  size_t next_prime(size_t n) noexcept;
}

#endif
