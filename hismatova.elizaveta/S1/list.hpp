#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <numeric>
#include <utility>

namespace hismatova
{
  template < typename T >
  class List
  {
  private:
    struct Node
    {
      T data;
      Node* next;
      Node(const T& val, Node* nextNode = nullptr):
        data(val),
        next(nextNode)
      {}
    };
    Node* fake;
    size_t list_size;

  public:
    class iterator
    {
    public:
      using value_type = T;
      using difference_type = std::ptrdiff_t;
      using pointer = T*;
      using reference = T&;
      using iterator_category = std::forward_iterator_tag;
      Node* current;
      explicit iterator(Node* node):
        current(node)
      {}
      T& operator*()
      {
        return current->data;
      }
      T* operator->()
      {
        return &current->data;
      }
      iterator& operator++()
      {
        current = current->next;
        return *this;
      }
      bool operator!=(const iterator& other) const
      {
        return current != other.current;
      }
    };

    List():
      list_size(0)
    {
      fake = new Node(T());
      fake->next = fake;
    }

    List(std::initializer_list< T > ilist):
      List()
    {
      for (const auto& val: ilist)
      {
        push_back(val);
      }
    }

    List(const List& other):
      List()
    {
      for (auto it = other.begin(); it != other.end(); ++it)
      {
        push_back(*it);
      }
    }

    List(List&& other) noexcept:
      fake(other.fake),
      list_size(other.list_size)
    {
      other.fake = nullptr;
      other.list_size = 0;
    }

    ~List()
    {
      clear();
      delete fake;
    }

    List& operator=(const List& other)
    {
      if (this != std::addressof(other))
      {
        clear();
        for (auto it = other.begin(); it != other.end(); ++it)
        {
          push_back(*it);
        }
      }
      return *this;
    }

    List& operator=(List&& other) noexcept
    {
      if (this != std::addressof(other))
      {
        clear();
        delete fake;
        fake = other.fake;
        list_size = other.list_size;
        other.fake = nullptr;
        other.list_size = 0;
      }
      return *this;
    }

    bool operator==(const List& other) const
    {
      if (list_size != other.list_size)
      {
        return false;
      }
      auto it1 = begin(), it2 = other.begin();
      while (it1 != end())
      {
        if (*it1 != *it2)
        {
          return false;
        }
        ++it1; ++it2;
      }
      return true;
    }

    bool operator!=(const List& other) const
    {
      return !(*this == other);
    }

    bool operator<(const List& other) const
    {
      auto it1 = begin(), it2 = other.begin();
      while (it1 != end() && it2 != other.end())
      {
        if (*it1 < *it2)
        {
          return true;
        }
        if (*it2 < *it1)
        {
          return false;
        }
        ++it1; ++it2;
      }
      return list_size < other.list_size;
    }

    bool operator>(const List& other) const
    {
      return other < *this;
    }

    bool operator<=(const List& other) const
    {
      return !(*this > other);
    }

    bool operator>=(const List& other) const
    {
      return !(*this < other);
    }

    iterator begin() const
    {
      return iterator(fake->next);
    }

    iterator end() const
    {
      return iterator(fake);
    }

    T& front()
    {
      return fake->next->data;
    }

    const T& front() const
    {
      return fake->next->data;
    }

    T& back()
    {
      Node* last = fake;
      while (last->next != fake)
      {
        last = last->next;
      }
      return last->data;
    }

    const T& back() const
    {
      Node* last = fake;
      while (last->next != fake)
      {
        last = last->next;
      }
      return last->data;
    }

    bool empty() const
    {
      return list_size == 0;
    }

    size_t size() const
    {
      return list_size;
    }

    void push_back(const T& value)
    {
      Node* newNode = new Node(value, fake);
      Node* last = fake;
      while (last->next != fake)
      {
        last = last->next;
      }
      last->next = newNode;
      ++list_size;
    }

    void pop_front()
    {
      if (empty())
      {
        return;
      }
      Node* temp = fake->next;
      fake->next = temp->next;
      delete temp;
      --list_size;
    }

    void clear()
    {
      while (!empty())
      {
        pop_front();
      }
    }

    void swap(List& other)
    {
      std::swap(fake, other.fake);
      std::swap(list_size, other.list_size);
    }

    void reverse()
    {
      if (list_size < 2)
      {
        return;
      }
      Node* prev = fake;
      Node* curr = fake->next;
      Node* next;
      while (curr != fake)
      {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
      }
      fake->next = prev;
    }

    void remove(const T& value)
    {
      Node* prev = fake;
      Node* curr = fake->next;
      while (curr != fake)
      {
        if (curr->data == value)
        {
          prev->next = curr->next;
          delete curr;
          curr = prev->next;
          --list_size;
        }
        else
        {
          prev = curr;
          curr = curr->next;
        }
      }
    }

    void remove_if(bool (*predicate)(const T&))
    {
      Node* prev = fake;
      Node* curr = fake->next;
      while (curr != fake)
      {
        if (predicate(curr->data))
        {
          prev->next = curr->next;
          delete curr;
          curr = prev->next;
          --list_size;
        }
        else
        {
          prev = curr;
          curr = curr->next;
        }
      }
    }

    void splice(iterator pos, List& other)
    {
      if (other.empty())
      {
        return;
      }
      Node* last = fake;
      while (last->next != fake)
      {
        last = last->next;
      }
      last->next = other.fake->next;
      other.fake->next = other.fake;
      list_size += other.list_size;
      other.list_size = 0;
    }

    void assign(size_t count, const T& value)
    {
      List temp;
      for (size_t i = 0; i < count; ++i)
      {
        temp.push_back(value);
      }
      swap(temp);
    }

    void assign(std::initializer_list< T > ilist)
    {
      List temp;
      for (const auto& val: ilist)
      {
        temp.push_back(val);
      }
      swap(temp);
    }

    void insert(iterator pos, const T& value)
    {
      Node* newNode = new Node(value);
      Node* current = fake;
      while (current != pos.current)
      {
        current = current->next;
      }
      newNode->next = current->next;
      current->next = newNode;
      ++list_size;
    }

    void erase(iterator pos)
    {
      Node* current = fake;
      while (current->next != pos.current)
      {
        current = current->next;
      }
      Node* temp = current->next;
      current->next = temp->next;
      delete temp;
      --list_size;
    }
  };
}
#endif
