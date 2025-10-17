#ifndef ARRAY_HPP
#define ARRAY_HPP
#include <cstddef>
#include <string>

namespace smirnov
{
  class Array
  {
  public:
    Array();
    Array(const Array & other);
    Array & operator=(const Array & other);
    ~Array();
    void push_back(const std::string & value);
    std::string & operator[](std::size_t index);
    const std::string & operator[](std::size_t index) const;
    std::size_t size() const;
    bool empty() const;
    void clear();
  private:
    std::string * data_;
    std::size_t size_;
    std::size_t capacity_;
    void reserve(std::size_t new_cap);
  };
}
#endif
