#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <string>
#include <utility>

namespace shramko
{
  class QueueString
  {
  public:
    QueueString() noexcept;
    ~QueueString();
    QueueString(const QueueString&) = delete;
    QueueString(QueueString&&) = delete;
    QueueString& operator=(const QueueString&) = delete;
    QueueString& operator=(QueueString&&) = delete;

    void push(const std::string& data);
    void push(std::string&& data);
    void pop();
    size_t size() const noexcept;
    bool empty() const noexcept;
    std::string& front();
    const std::string& front() const;
    void swap(QueueString& other) noexcept;

  private:
    struct Node
    {
      std::string data;
      Node* next;
      explicit Node(const std::string& d, Node* n = nullptr);
      explicit Node(std::string&& d, Node* n = nullptr);
    };
    Node* head_;
    Node* tail_;
    size_t size_;
  };

  class QueueOfQueueString
  {
  public:
    QueueOfQueueString() noexcept;
    ~QueueOfQueueString();
    QueueOfQueueString(const QueueOfQueueString&) = delete;
    QueueOfQueueString(QueueOfQueueString&&) = delete;
    QueueOfQueueString& operator=(const QueueOfQueueString&) = delete;
    QueueOfQueueString& operator=(QueueOfQueueString&&) = delete;

    void push(QueueString& data);
    void pop();
    size_t size() const noexcept;
    bool empty() const noexcept;
    QueueString& front();
    const QueueString& front() const;
    void swap(QueueOfQueueString& other) noexcept;

  private:
    struct Node
    {
      QueueString data;
      Node* next;
      explicit Node(QueueString& d, Node* n = nullptr);
    };
    Node* head_;
    Node* tail_;
    size_t size_;
  };
}

#endif
