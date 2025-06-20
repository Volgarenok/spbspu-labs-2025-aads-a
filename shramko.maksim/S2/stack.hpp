#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <string>
#include <utility>

namespace shramko
{
  class StackString
  {
  public:
    StackString() noexcept;
    ~StackString();
    StackString(const StackString&) = delete;
    StackString(StackString&&) = delete;
    StackString& operator=(const StackString&) = delete;
    StackString& operator=(StackString&&) = delete;

    void push(const std::string& data);
    void push(std::string&& data);
    void pop();
    size_t size() const noexcept;
    bool empty() const noexcept;
    std::string& top();
    const std::string& top() const;
    void swap(StackString& other) noexcept;

  private:
    struct Node
    {
      std::string data;
      Node* next;
      explicit Node(const std::string& d, Node* n = nullptr);
      explicit Node(std::string&& d, Node* n = nullptr);
    };
    Node* head_;
    size_t size_;
  };

  class StackLongLong
  {
  public:
    StackLongLong() noexcept;
    ~StackLongLong();
    StackLongLong(const StackLongLong&) = delete;
    StackLongLong(StackLongLong&&) = delete;
    StackLongLong& operator=(const StackLongLong&) = delete;
    StackLongLong& operator=(StackLongLong&&) = delete;

    void push(long long data);
    void pop();
    size_t size() const noexcept;
    bool empty() const noexcept;
    long long& top();
    const long long& top() const;
    void swap(StackLongLong& other) noexcept;

  private:
    struct Node
    {
      long long data;
      Node* next;
      explicit Node(long long d, Node* n = nullptr);
    };
    Node* head_;
    size_t size_;
  };
}

#endif
