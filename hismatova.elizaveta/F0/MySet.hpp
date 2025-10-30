#ifndef MYSET_HPP
#define MYSET_HPP
#include <iostream>
#include <algorithm>
#include "MyVector.h"

namespace hismatova
{
  template< typename T >
  struct Node
  {
    T _data;
    Node* _left;
    Node* _right;

    Node():
      _data(T()),
      _left(nullptr),
      _right(nullptr)
    {};
    Node(const T& data):
      _data(data),
      _left(nullptr),
      _right(nullptr)
    {};

    Node*& getLeftTree()
    {
      return _left;
    }
    Node*& getRigthTree()
    {
      return _right;
    }
    T getData() const
    {
      return _data;
    }
  };

  template< typename T >
  class MySetIterator;

  template< typename T >
  class MySetConstIterator;

  template< typename T >
  class MySet
  {
  private:
    Node< T >* _root;
    size_t _size;

    bool insertKey(Node< T >*& root, const T& key);
    bool containData(Node< T >* root, const T& key) const;
    void copyTree(Node< T >*& newNode, const Node< T >* otherNode);
    void printTree(const Node< T >* root) const;
    void deleteTree(Node< T >*& root);
    bool deleteKey(Node< T >*& root, const T& key);
    Node< T >* findMin(Node< T >* node) const;

  public:
    using iterator = MySetIterator< T >;
    using const_iterator = MySetConstIterator< T >;

    MySet();
    MySet(const MySet< T >& other);
    ~MySet();
    MySet< T >& operator=(const MySet< T >& other);

    bool insert(const T& key);
    bool contains(const T& key) const;
    bool erase(const T& key);
    iterator erase(iterator position);
    void clear();

    bool empty() const;
    size_t size() const;

    iterator find(const T& key);
    const_iterator find(const T& key) const;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    void print() const;
    Node< T >* getRoot() const;
  };

  template< typename T >
  class MySetIterator
  {
  private:
    Node< T >* current;
    Vector< Node< T >* > stack;

    void pushLeft(Node< T >* node)
    {
      while (node != nullptr)
      {
        stack.PushBack(node);
        node = node->_left;
      }
    }

  public:
    friend class MySet< T >;

    MySetIterator(Node< T >* root = nullptr)
    {
      pushLeft(root);
      if (stack.Size() > 0)
      {
        current = stack[stack.Size() - 1];
        stack.PopBack();
      }
      else
      {
        current = nullptr;
      }
    }

    MySetIterator(Node< T >* curr, const Vector< Node< T >* >& stk):
      current(curr),
      stack(stk)
    {}

    T& operator*() const
    {
      return current->_data;
    }

    T* operator->() const
    {
      return &(current->_data);
    }

    MySetIterator& operator++()
    {
      if (current->_right != nullptr)
      {
        pushLeft(current->_right);
      }

      if (stack.Size() > 0)
      {
        current = stack[stack.Size() - 1];
        stack.PopBack();
      }
      else
      {
        current = nullptr;
      }
      return *this;
    }

    MySetIterator operator++(int)
    {
      MySetIterator temp = *this;
      ++(*this);
      return temp;
    }

    bool operator==(const MySetIterator& other) const
    {
      return current == other.current;
    }

    bool operator!=(const MySetIterator& other) const
    {
      return !(*this == other);
    }
  };

  template< typename T >
  class MySetConstIterator
  {
  private:
    const Node< T >* current;
    Vector< const Node< T >* > stack;

    void pushLeft(const Node< T >* node)
    {
      while (node != nullptr)
      {
        stack.PushBack(node);
        node = node->_left;
      }
    }

  public:
    friend class MySet< T >;

    MySetConstIterator(const Node< T >* root = nullptr)
    {
      pushLeft(root);
      if (stack.Size() > 0)
      {
        current = stack[stack.Size() - 1];
        stack.PopBack();
      }
      else
      {
        current = nullptr;
      }
    }

    MySetConstIterator(const Node< T >* curr, const Vector< const Node< T >* >& stk):
      current(curr),
      stack(stk)
    {}

    const T& operator*() const
    {
      return current->_data;
    }

    const T* operator->() const
    {
      return &(current->_data);
    }

    MySetConstIterator& operator++()
    {
      if (current->_right != nullptr)
      {
        pushLeft(current->_right);
      }

      if (stack.Size() > 0)
      {
        current = stack[stack.Size() - 1];
        stack.PopBack();
      }
      else
      {
        current = nullptr;
      }
      return *this;
    }

    MySetConstIterator operator++(int)
    {
      MySetConstIterator temp = *this;
      ++(*this);
      return temp;
    }

    bool operator==(const MySetConstIterator& other) const
    {
      return current == other.current;
    }

    bool operator!=(const MySetConstIterator& other) const
    {
      return !(*this == other);
    }
  };

  template< typename T >
  MySet< T >::MySet():
    _root(nullptr),
    _size(0)
  {}

  template< typename T >
  MySet< T >::MySet(const MySet< T >& other):
    _root(nullptr),
    _size(0)
  {
    copyTree(_root, other._root);
    _size = other._size;
  }

  template< typename T >
  MySet< T >& MySet< T >::operator=(const MySet< T >& other)
  {
    if (this != &other)
    {
      clear();
      copyTree(_root, other._root);
      _size = other._size;
    }
    return *this;
  }

  template< typename T >
  MySet< T >::~MySet()
  {
    clear();
  }

  template< typename T >
  void MySet< T >::copyTree(Node< T >*& newNode, const Node< T >* otherNode)
  {
    if (otherNode == nullptr)
    {
      newNode = nullptr;
      return;
    }

    newNode = new Node< T >(otherNode->_data);
    copyTree(newNode->_left, otherNode->_left);
    copyTree(newNode->_right, otherNode->_right);
  }

  template< typename T >
  void MySet< T >::deleteTree(Node< T >*& root)
  {
    if (root == nullptr)
    {
      return;
    }

    deleteTree(root->_left);
    deleteTree(root->_right);
    delete root;
    root = nullptr;
  }

  template< typename T >
  bool MySet< T >::insert(const T& key)
  {
    if (insertKey(_root, key))
    {
      _size++;
      return true;
    }
    return false;
  }

  template< typename T >
  bool MySet< T >::insertKey(Node< T >*& root, const T& key)
  {
    if (root == nullptr)
    {
      root = new Node< T >(key);
      return true;
    }

    if (key < root->getData())
    {
      return insertKey(root->getLeftTree(), key);
    }
    else if (key > root->getData())
    {
      return insertKey(root->getRigthTree(), key);
    }

    return false;
  }

  template< typename T >
  bool MySet< T >::contains(const T& key) const
  {
    return containData(_root, key);
  }

  template< typename T >
  bool MySet< T >::containData(Node< T >* root, const T& key) const
  {
    if (root == nullptr)
    {
      return false;
    }

    if (key == root->getData())
    {
      return true;
    }

    if (key < root->getData())
    {
      return containData(root->getLeftTree(), key);
    }
    else
    {
      return containData(root->getRigthTree(), key);
    }
  }

  template< typename T >
  bool MySet< T >::erase(const T& key)
  {
    if (deleteKey(_root, key))
    {
      _size--;
      return true;
    }
    return false;
  }

  template< typename T >
  typename MySet< T >::iterator MySet< T >::erase(iterator position)
  {
    if (position == end())
    {
      return end();
    }

    T key = *position;
    iterator next = position;
    ++next;

    if (erase(key))
    {
      return next;
    }

    return end();
  }

  template< typename T >
  bool MySet< T >::deleteKey(Node< T >*& root, const T& key)
  {
    if (root == nullptr)
    {
      return false;
    }

    if (key < root->getData())
    {
      return deleteKey(root->getLeftTree(), key);
    }
    else if (key > root->getData())
    {
      return deleteKey(root->getRigthTree(), key);
    }
    else
    {
      if (root->_left == nullptr && root->_right == nullptr)
      {
        delete root;
        root = nullptr;
        return true;
      }
      else if (root->_left == nullptr)
      {
        Node< T >* temp = root;
        root = root->_right;
        delete temp;
        return true;
      }
      else if (root->_right == nullptr)
      {
        Node< T >* temp = root;
        root = root->_left;
        delete temp;
        return true;
      }
      else
      {
        Node< T >* temp = findMin(root->_right);
        root->_data = temp->_data;
        return deleteKey(root->getRigthTree(), temp->_data);
      }
    }
  }

  template< typename T > 
  Node< T >* MySet< T >::findMin(Node< T >* node) const
  {
    while (node != nullptr && node->_left != nullptr)
    {
      node = node->_left;
    }
    return node;
  }

  template< typename T >
  void MySet< T >::clear()
  {
    deleteTree(_root);
    _root = nullptr;
    _size = 0;
  }

  template< typename T >
  bool MySet< T >::empty() const
  {
    return _root == nullptr;
  }

  template< typename T >
  size_t MySet< T >::size() const
  {
    return _size;
  }

  template< typename T >
  typename MySet< T >::iterator MySet< T >::find(const T& key)
  {
    Node< T >* current = _root;
    Vector< Node< T >* > searchStack;

    while (current != nullptr || searchStack.Size() > 0)
    {
      while (current != nullptr)
      {
        searchStack.PushBack(current);
        current = current->_left;
      }

      current = searchStack[searchStack.Size() - 1];
      searchStack.PopBack();

      if (current->_data == key)
      {
        return iterator(current, searchStack);
      }

      current = current->_right;
    }

    return end();
  }

  template< typename T >
  typename MySet< T >::const_iterator MySet< T >::find(const T& key) const
  {
    const Node< T >* current = _root;
    Vector< const Node< T >* > searchStack;

    while (current != nullptr || searchStack.Size() > 0)
    {
      while (current != nullptr)
      {
        searchStack.PushBack(current);
        current = current->_left;
      }

      current = searchStack[searchStack.Size() - 1];
      searchStack.PopBack();

      if (current->_data == key)
      {
        return const_iterator(current, searchStack);
      }

      current = current->_right;
    }

    return end();
  }

  template< typename T >
  typename MySet< T >::iterator MySet< T >::begin()
  {
    return iterator(_root);
  }

  template< typename T >
  typename MySet< T >::iterator MySet< T >::end()
  {
    return iterator(nullptr);
  }

  template< typename T >
  typename MySet< T >::const_iterator MySet< T >::begin() const
  {
    return const_iterator(_root);
  }

  template< typename T >
  typename MySet< T >::const_iterator MySet< T >::end() const
  {
    return const_iterator(nullptr);
  }

  template< typename T >
  typename MySet< T >::const_iterator MySet< T >::cbegin() const
  {
    return const_iterator(_root);
  }

  template< typename T >
  typename MySet< T >::const_iterator MySet< T >::cend() const
  {
    return const_iterator(nullptr);
  }

  template< typename T >
  void MySet< T >::print() const
  {
    printTree(_root);
    std::cout << std::endl;
  }

  template< typename T >
  void MySet< T >::printTree(const Node< T >* root) const
  {
    if (root == nullptr)
    {
      return;
    }
    printTree(root->_left);
    std::cout << root->getData() << " ";
    printTree(root->_right);
  }

  template< typename T >
  Node< T >* MySet< T >::getRoot() const
  {
    return _root;
  }
}
#endif
