#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <random> 
#include <algorithm>

template <typename T>
class BinarySearchTree
{
private:
  BinarySearchTree* parent = nullptr;
  BinarySearchTree* left = nullptr;
  BinarySearchTree* right = nullptr;
  bool hasElement = false;
  int64_t key;
  BinarySearchTree& Min(BinarySearchTree& node) const noexcept {
    BinarySearchTree* min = &node;
    while (min->left != nullptr)
    {
      min = min->left;
    }
    return *min;
  }
  void Insert(BinarySearchTree& head, BinarySearchTree* node) noexcept {
    if (head.key < node->key)
    {
      if (head.right == nullptr)
      {
        head.right = node;
        node->parent = &head;
      }
      else
      {
        Insert(*head.right, node);
      }
    }
    else if (head.key > node->key)
    {
      if (head.left == nullptr)
      {
        head.left = node;
        node->parent = &head;
      }
      else
      {
        Insert(*head.left, node);
      }
    }
    else
    {
      head.data = node->data;
    }
  }
public:
  T data;
  BinarySearchTree(int64_t key, T& data) : key(key), data(data), hasElement(true) {
  }
  BinarySearchTree() = default;
  ~BinarySearchTree() {
    delete left;
    delete right;
  }
  void Insert(int64_t key, T data) {
    if (!hasElement)
    {
      this->key = key;
      this->data = data;
      hasElement = true;
    }
    else {
      BinarySearchTree* node = new BinarySearchTree(key, data);
      Insert(*this, node);
    }
  }
  void Remove(BinarySearchTree& node) {
    uint8_t hasLeftRight = (node.left != nullptr) << 1 |
      (node.right != nullptr);
    //0b00 does`nt have left & right
    //0b01 has right
    //0b10 has left
    //0b11 has both
    if (node.parent == nullptr)
    {
      BinarySearchTree<T>* nodeToDelete = nullptr;
      switch (hasLeftRight)
      {
      case 0b00:
        node.data = 0;
        node.hasElement = false;
        break;

      case 0b01:
        nodeToDelete = node.right;
        node = *node.right;
        node.parent = nullptr;
        nodeToDelete->right = nullptr;
        nodeToDelete->left = nullptr;
        if (node.left!=nullptr)
        {
          node.left->parent = &node;
        }
        if (node.right != nullptr)
        {
          node.right->parent = &node;
        }
        delete nodeToDelete;
        break;
      case 0b10:
        nodeToDelete = node.left;
        node = *node.left;
        node.parent = nullptr;
        nodeToDelete->right = nullptr;
        nodeToDelete->left = nullptr;
        if (node.left != nullptr)
        {
          node.left->parent = &node;
        }
        if (node.right != nullptr)
        {
          node.right->parent = &node;
        }
        delete nodeToDelete;
        break;
      case 0b11:
      {
        BinarySearchTree& successor = Min(*node.right);
        node.data = successor.data;
        int64_t buf = successor.key;
        Remove(successor);
        node.key = buf;
        break;
      }
      default:
        break;
      }
      return;
    }
    switch (hasLeftRight)
    {
    case 0b00:
      if (node.parent->left != nullptr && node.parent->left->key == node.key)
      {
        node.parent->left = nullptr;
      }
      else
      {
        node.parent->right = nullptr;
      }
      break;

    case 0b01:
      if (node.parent->left != nullptr && node.parent->left->key == node.key)
      {
        node.parent->left = node.right;
      }
      else
      {
        node.parent->right = node.right;
      }
      node.right->parent = node.parent;
      break;
    case 0b10:
      if (node.parent->left != nullptr && node.parent->left->key == node.key)
      {
        node.parent->left = node.left;
      }
      else
      {
        node.parent->right = node.left;
      }
      node.left->parent = node.parent;
      break;
    case 0b11:
    {
      BinarySearchTree& successor = Min(*node.right);
      node.key = successor.key;
      node.data = successor.data;
      if (successor.parent->left != nullptr && successor.parent->left->key == successor.key)
      {
        successor.parent->left = successor.right;
      }
      else
      {
        successor.parent->right = successor.right;
      }
      if (successor.right != nullptr)
      {
        successor.right->parent = successor.parent;
      }
      return;
      break;
    }
    default:
      break;
    }
    node.left = nullptr;
    node.right = nullptr;
    delete& node;
  }
  BinarySearchTree* Find(int64_t key) {
    BinarySearchTree* ans = this;
    while (ans != nullptr && key != ans->key)
    {
      ans = ans->key < key ? ans->right : ans->left;
    }
    if (ans != nullptr)
    {
      return ans;
    }
    else
    {
      throw std::runtime_error("key not exist");
      return nullptr;
    }
  }
};

std::chrono::duration<double> testInsertTime(int count);
std::chrono::duration<double> testFindTime(int count);
std::chrono::duration<double> testRemoveTime(int count);
int main()
{
  int start = 100;
  int step = 1000;
  int stop = 100000;
  std::cout << '[';
  for (size_t i = start; i < stop; i+=step)
  {
    std::cout << testInsertTime(i).count() << ", ";
  }
  std::cout << testInsertTime(stop).count() << ']';
  std::cout << '\n';
  std::cout << '[';
  for (size_t i = start; i < stop; i += step)
  {
    std::cout << testFindTime(i).count() << ", ";
  }
  std::cout << testFindTime(stop).count() << ']';
  std::cout << '\n';
  std::cout << '[';
  for (size_t i = start; i < stop; i += step)
  {
    std::cout << testRemoveTime(i).count() << ", ";
  }
  std::cout << testRemoveTime(stop).count() << ']';
  std::cout << '\n';
  std::cout << '[';
  for (size_t i = start; i < stop; i += step)
  {
    std::cout << i << ", ";
  }
  std::cout << stop << ']';
  std::cout << '\n';
}

std::chrono::duration<double> testInsertTime(int count) {
  BinarySearchTree<int>* tree;
  std::vector<int> nums;
  for (int i = 0; i < count; i++)
  {
    nums.push_back(i);
  }
  std::random_device rd;
  std::chrono::duration<double> res(0);
  auto start{ std::chrono::steady_clock::now() };
  auto end{ std::chrono::steady_clock::now() };
  std::chrono::duration<double> elapsed_seconds{ end - start };
  for (int test_n = 0; test_n < 100; test_n++)
  {
    std::shuffle(nums.begin(), nums.end(), rd);
    tree = new BinarySearchTree<int>();
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < count; i++)
    {
      tree->Insert(nums[i], nums[i]);
    }
    end = std::chrono::steady_clock::now();
    delete(tree);
    elapsed_seconds = end - start;
    res += elapsed_seconds;
  }
  
  res /= 100 * count;
  return res;
}

std::chrono::duration<double> testFindTime(int count) {
  BinarySearchTree<int>* tree;
  std::vector<int> nums;
  for (int i = 0; i < count; i++)
  {
    nums.push_back(i);
  }
  std::random_device rd;
  std::shuffle(nums.begin(), nums.end(), rd);
  std::chrono::duration<double> res(0);
  auto start{ std::chrono::steady_clock::now() };
  auto end{ std::chrono::steady_clock::now() };
  std::chrono::duration<double> elapsed_seconds{ end - start };
  for (int test_n = 0; test_n < 100; test_n++)
  {
    std::shuffle(nums.begin(), nums.end(), rd);
    tree = new BinarySearchTree<int>();
    for (int i = 0; i < count; i++)
    {
      tree->Insert(nums[i], nums[i]);
    }
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < count; i++)
    {
      tree->Find(i)->data;
    }
    end = std::chrono::steady_clock::now();
    delete(tree);
    elapsed_seconds = end - start;
    res += elapsed_seconds;
  }

  res /= 100*count;
  return res;
}

std::chrono::duration<double> testRemoveTime(int count) {
  BinarySearchTree<int>* tree;
  std::vector<int> nums;
  for (int i = 0; i < count; i++)
  {
    nums.push_back(i);
  }
  std::random_device rd;
  
  std::chrono::duration<double> res(0);
  auto start{ std::chrono::steady_clock::now() };
  auto end{ std::chrono::steady_clock::now() };
  std::chrono::duration<double> elapsed_seconds{ end - start };
  BinarySearchTree<int>* found = nullptr;
  for (int test_n = 0; test_n < 100; test_n++)
  {
    std::shuffle(nums.begin(), nums.end(), rd);
    tree = new BinarySearchTree<int>();
    for (int i = 0; i < count; i++)
    {
      tree->Insert(nums[i], nums[i]);
    }
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < count; i++)
    {
      found = tree->Find(i);
      if (tree!=nullptr)
      {
        tree->Remove(*tree->Find(i));

      }
      
    }
    end = std::chrono::steady_clock::now();
    delete(tree);
    elapsed_seconds = end - start;
    res += elapsed_seconds;
  }

  res /= 100 * count;
  return res;
}