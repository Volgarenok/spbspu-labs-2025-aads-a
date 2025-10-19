#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>
#include <stack>
#include <queue>
#include "iterator.hpp"
#include "cIterator.hpp"

namespace guseynov
{
  template < typename Key, typename Value, typename Compare >
  class Tree
  {
  public:
    using Iterator_t = Iterator< Key, Value, Compare >;
    using ConstIterator_t = ConstIterator< Key, Value, Compare >;

    Tree():
      fakeRoot_(new Node_t(Key(), Value(), nullptr)),
      size_(0)
    {}

    template< typename InputIt >
    Tree(InputIt first, InputIt last):
      Tree()
    {
      for (InputIt it = first; it != last; it++)
      {
        insert(*it);
      }
    }

    explicit Tree(std::initializer_list< std::pair< Key, Value > > iList):
      Tree(iList.begin(), iList.end())
    {}

    Tree(const Tree& other):
      Tree(other.cbegin(), other.cend())
    {}

    Tree(Tree&& other) noexcept:
      fakeRoot_(std::exchange(other.fakeRoot_, new Node_t(Key(), Value(), nullptr))),
      size_(std::exchange(other.size_, 0))
    {}

    ~Tree()
    {
      clear();
      delete fakeRoot_;
    }

    Tree& operator=(const Tree& other)
    {
      if (this != std::addressof(other))
      {
        Tree< Key, Value, Compare> temp(other);
        swap(temp);
      }
      return *this;
    }

    Tree& operator=(Tree&& other) noexcept
    {
      if (this != std::addressof(other))
      {
        Tree< Key, Value, Compare > temp(std::move(other));
        swap(temp);
      }
      return *this;
    }

    Iterator_t begin() const noexcept
    {
      if (empty())
      {
        return end();
      }
      return Iterator_t(findMin(getRoot()));
    }

    ConstIterator_t cbegin() const noexcept
    {
      if (empty())
      {
        return cend();
      }
      return ConstIterator_t(findMin(getRoot()));
    }

    Iterator_t end() const noexcept
    {
      return Iterator_t(fakeRoot_);
    }

    ConstIterator_t cend() const noexcept
    {
      return ConstIterator_t(fakeRoot_);
    }

    void push(const Key& k, const Value& v)
    {
      if (count(k))
      {
        Node_t* existing = find(getRoot(), k);
        existing->data.second = v;
        return;
      }
      Node_t* oldRoot = getRoot();
      size_t oldSize = size_;
      try
      {
        Node_t* newRoot = insert(getRoot(), k, v, fakeRoot_);
        setRoot(newRoot);
        size_ = oldSize + 1;
      }
      catch (...)
      {
        setRoot(oldRoot);
        size_ = oldSize;
        throw;
      }
    }

    const Value& get(const Key& k) const
    {
      const Node_t* node = find(getRoot(), k);
      if (!node)
      {
        throw std::out_of_range("Key not found");
      }
      return node->data.second;
    }

    void drop(const Key& k)
    {
      Node_t* node = find(getRoot(), k);
      if (!node)
      {
        throw std::out_of_range("Key not found");
      }
      setRoot(remove(getRoot(), k));
      size_--;
    }

    size_t size() const noexcept
    {
      return size_;
    }

    bool empty() const noexcept
    {
      return size_ == 0;
    }

    void clear() noexcept
    {
      clear(getRoot());
      setRoot(nullptr);
      size_ = 0;
    }

    void swap(Tree& other) noexcept
    {
      std::swap(fakeRoot_, other.fakeRoot_);
      std::swap(size_, other.size_);
    }

    Iterator_t insert(const std::pair< const Key, Value >& value)
    {
      push(value.first, value.second);
      return Iterator_t(find(getRoot(), value.first));
    }

    Iterator_t insert(std::pair< const Key, Value >&& value)
    {
      push(std::move(value.first), std::move(value.second));
      return Iterator_t(find(getRoot(), value.first));
    }

    template< typename InputIt >
    void insert(InputIt first, InputIt last)
    {
      for (InputIt it = first; it != last; ++it)
      {
        push(it->first, it->second);
      }
    }

    Iterator_t erase(Iterator_t pos) noexcept
    {
      if (pos == end() || pos.current_ == nullptr)
      {
        return end();
      }
      Node_t* nextNode = findNextNode(pos.current_);
      setRoot(remove(getRoot(), pos.current_->data.first));
      size_--;
      if (nextNode != nullptr)
      {
        return Iterator_t(nextNode);
      }
      else
      {
        return end();
      }
    }

    size_t erase(const Key& key) noexcept
    {
      Iterator_t it = find(key);
      if (it != end())
       {
         erase(it);
         return 1;
       }
      return 0;
    }

    Iterator_t erase(Iterator_t first, Iterator_t last) noexcept
    {
      while (first != last)
      {
        first = erase(first);
      }
      return last;
    }

    Value& at(const Key& key)
    {
      return const_cast< Value& >(static_cast< const Tree* >(this)->at(key));
    }

    const Value& at(const Key& key) const
    {
      const Node_t* node = find(getRoot(), key);
      if (!node)
      {
        throw std::out_of_range("Key not found");
      }
      return node->data.second;
    }

    Value& operator[](const Key& key)
    {
      Node_t* node = find(getRoot(), key);
      if (!node)
      {
        push(key, Value());
        node = find(getRoot(), key);
      }
      return node->data.second;
    }

    Value& operator[](Key&& key)
    {
      Node_t* node = find(getRoot(), key);
      if (!node)
      {
        push(std::move(key), Value());
        node = find(getRoot(), key);
      }
      return node->data.second;
    }

    size_t count(const Key& key) const noexcept
    {
      if (find(getRoot(), key))
      {
        return 1;
      }
      return 0;
    }

    Iterator_t find(const Key& key) noexcept
    {
      Node_t* node = find(getRoot(), key);
      if (node != nullptr)
      {
        return Iterator_t(node);
      }
      else
      {
        return end();
      }
    }

    ConstIterator_t find(const Key& key) const noexcept
    {
      const Node_t* node = find(getRoot(), key);
      if (node != nullptr)
      {
        return ConstIterator_t(node);
      }
      else
      {
        return cend();
      }
    }

    std::pair< Iterator_t, Iterator_t > equal_range(const Key& key) noexcept
    {
      std::pair< ConstIterator_t, ConstIterator_t > constRange = static_cast< const Tree* >(this)->equal_range(key);
      return { Iterator_t(const_cast< Node_t* >(constRange.first.current_) ), Iterator_t(const_cast< Node_t* >(constRange.second.current_)) };
    }

    std::pair< ConstIterator_t, ConstIterator_t > equal_range(const Key& key) const noexcept
    {
      return { lower_bound(key), upper_bound(key) };
    }

    Iterator_t lower_bound(const Key& key) noexcept
    {
      return Iterator_t(const_cast< Node_t* >(static_cast< const Tree* >(this)->lower_bound(key).current_));
    }

    ConstIterator_t lower_bound(const Key& key) const noexcept
    {
      const Node_t* current = getRoot();
      const Node_t* result = nullptr;
      while (current)
      {
        if (!comp_(current->data.first, key))
        {
          result = current;
          current = current->left;
        }
        else
        {
          current = current->right;
        }
      }
      if (result != nullptr)
      {
        return ConstIterator_t(result);
      }
      else
      {
        return cend();
      }
    }

    Iterator_t upper_bound(const Key& key) noexcept
    {
      return Iterator_t(const_cast< Node_t* >(static_cast< const Tree* >(this)->upper_bound(key).current_));
    }

    ConstIterator_t upper_bound(const Key& key) const noexcept
    {
      const Node_t* current = getRoot();
      const Node_t* result = nullptr;
      while (current)
      {
        if (comp_(key, current->data.first))
        {
          result = current;
          current = current->left;
        }
        else
        {
          current = current->right;
        }
      }
      if (result != nullptr)
      {
        return ConstIterator_t(result);
      }
      else
      {
        return cend();
      }
    }

    template< typename F >
    F& traverse_lnr(F& f) const
    {
      if (empty())
      {
        return f;
      }
      std::stack< const Node_t* > stack;
      const Node_t* current = getRoot();
      while (current != nullptr || !stack.empty())
      {
        while (current != nullptr)
        {
          stack.push(current);
          current = current->left;
        }
        current = stack.top();
        stack.pop();
        f(current->data);
        current = current->right;
      }
      return f;
    }

    template< typename F >
    F& traverse_rnl(F& f) const
    {
      if (empty())
      {
        return f;
      }
      std::stack< const Node_t* > stack;
      const Node_t* current = getRoot();
      while (current != nullptr || !stack.empty())
      {
        while (current != nullptr)
        {
          stack.push(current);
          current = current->right;
        }
        current = stack.top();
        stack.pop();
        f(current->data);
        current = current->left;
      }
      return f;
    }

    template< typename F >
    F& traverse_breadth(F& f) const
    {
      if (empty())
      {
        return f;
      }
      std::queue< const Node_t* > queue;
      queue.push(getRoot());
      while (!queue.empty())
      {
        const Node_t* current = queue.front();
        queue.pop();
        f(current->data);
        if (current->left)
        {
          queue.push(current->left);
        }
        if (current->right)
        {
          queue.push(current->right);
        }
      }
      return f;
    }

  private:
    using Node_t = Node< Key, Value >;

    Node_t* fakeRoot_;
    size_t size_;
    Compare comp_;

    Node_t* getRoot() const noexcept
    {
      return fakeRoot_->left;
    }

    void setRoot(Node_t* newRoot) noexcept
    {
      fakeRoot_->left = newRoot;
      if (newRoot)
      {
        newRoot->parent = fakeRoot_;
      }
    }

    void updateHeight(Node_t* node) noexcept
    {
      if (node == nullptr)
      {
        return;
      }
      int leftHeight = 0;
      if (node->left != nullptr)
      {
        leftHeight = node->left->height;
      }
      int rightHeight = 0;
      if (node->right != nullptr)
      {
        rightHeight = node->right->height;
      }
      node->height = 1 + std::max(leftHeight, rightHeight);
    }

    int getBalance(Node_t* node) noexcept
    {
      if (!node)
      {
        return 0;
      }
      int leftHeight = 0;
      if (node->left)
      {
        leftHeight = node->left->height;
      }
      int rightHeight = 0;
      if (node->right)
      {
        rightHeight = node->right->height;
      }
      return leftHeight - rightHeight;
    }

    Node_t* rotateRight(Node_t* y) noexcept
    {
      Node_t* leftChild = y->left;
      Node_t* leftRightSubtree = leftChild->right;
      leftChild->right = y;
      y->left = leftRightSubtree;
      if (leftRightSubtree)
      {
        leftRightSubtree->parent = y;
      }
      leftChild->parent = y->parent;
      y->parent = leftChild;
      updateHeight(y);
      updateHeight(leftChild);
      return leftChild;
    }

    Node_t* rotateLeft(Node_t* x) noexcept
    {
      Node_t* rightChild = x->right;
      Node_t* rightLeftSubtree = rightChild->left;
      rightChild->left = x;
      x->right = rightLeftSubtree;
      if (rightLeftSubtree)
      {
        rightLeftSubtree->parent = x;
      }
      rightChild->parent = x->parent;
      x->parent = rightChild;
      updateHeight(x);
      updateHeight(rightChild);
      return rightChild;
    }

    Node_t* insert(Node_t* node, const Key& k, const Value& v, Node_t* parent)
    {
      if (!node)
      {
        return new Node_t(k, v, parent);
      }
      if (comp_(k, node->data.first))
      {
        node->left = insert(node->left, k, v, node);
      }
      else if (comp_(node->data.first, k))
      {
        node->right = insert(node->right, k, v, node);
      }
      else
      {
        node->data.second = v;
        return node;
      }
      updateHeight(node);
      int balance = getBalance(node);
      if (balance > 1 && comp_(k, node->left->data.first))
      {
        return rotateRight(node);
      }
      if (balance < -1 && comp_(node->right->data.first, k))
      {
        return rotateLeft(node);
      }
      if (balance > 1 && comp_(node->left->data.first, k))
      {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
      }
      if (balance < -1 && comp_(k, node->right->data.first))
      {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
      }
      return node;
    }

    Node_t* find(Node_t* node, const Key& k) const noexcept
    {
      while (node)
      {
        if (comp_(k, node->data.first))
        {
          node = node->left;
        }
        else if (comp_(node->data.first, k))
        {
          node = node->right;
        }
        else
        {
          return node;
        }
      }
      return nullptr;
    }

    Node_t* findMin(Node_t* node) const noexcept
    {
      if (!node)
      {
        return nullptr;
      }
      while (node->left)
      {
        node = node->left;
      }
      return node;
    }

    Node_t* findMax(Node_t* node) const noexcept
    {
      if (!node)
      {
        return nullptr;
      }
      while (node->right)
      {
        node = node->right;
      }
      return node;
    }

    Node_t* remove(Node_t* node, const Key& k) noexcept
    {
      if (!node)
      {
        return nullptr;
      }
      if (comp_(k, node->data.first))
      {
        node->left = remove(node->left, k);
      }
      else if (comp_(node->data.first, k))
      {
        node->right = remove(node->right, k);
      }
      else
      {
        if (!node->left || !node->right)
        {
          Node_t* temp = node->left;
          if (!temp)
          {
            temp = node->right;
          }
          if (!temp)
          {
            temp = node;
            node = nullptr;
          }
          else
          {
            *node = *temp;
          }
          delete temp;
        }
        else
        {
          Node_t* temp = findMin(node->right);
          node->data = temp->data;
          node->right = remove(node->right, temp->data.first);
        }
      }
      if (!node)
      {
        return nullptr;
      }
      updateHeight(node);
      int balance = getBalance(node);
      if (balance > 1 && getBalance(node->left) >= 0)
      {
        return rotateRight(node);
      }
      if (balance > 1 && getBalance(node->left) < 0)
      {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
      }
      if (balance < -1 && getBalance(node->right) <= 0)
      {
        return rotateLeft(node);
      }
      if (balance < -1 && getBalance(node->right) > 0)
      {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
      }
      return node;
    }

    void clear(Node_t* node) noexcept
    {
      if (node)
      {
        clear(node->left);
        clear(node->right);
        delete node;
      }
    }

    Node_t* copyTree(Node_t* node, Node_t* parent)
    {
      if (!node)
      {
        return nullptr;
      }
      Node_t* left_copy = nullptr;
      Node_t* right_copy = nullptr;
      Node_t* newNode = nullptr;
      try
      {
        newNode = new Node_t(node->data.first, node->data.second, parent);
        left_copy = copyTree(node->left, nullptr);
        right_copy = copyTree(node->right, nullptr);
        newNode->left = left_copy;
        newNode->right = right_copy;
        if (left_copy)
        {
          left_copy->parent = newNode;
        }
        if (right_copy)
        {
          right_copy->parent = newNode;
        }
        newNode->height = node->height;
        return newNode;
      }
      catch (...)
      {
        clear(left_copy);
        clear(right_copy);
        delete newNode;
        throw;
      }
    }

    Node_t* findNextNode(Node_t* node) const noexcept
    {
      if (node == nullptr)
      {
        return nullptr;
      }
      if (node->right != nullptr)
      {
        node = node->right;
        while (node->left != nullptr)
        {
          node = node->left;
        }
        return node;
      }
      while (node->parent != nullptr)
      {
        if (node == node->parent->left)
        {
          return node->parent;
        }
        node = node->parent;
      }
      return nullptr;
    }
  };
}

#endif
