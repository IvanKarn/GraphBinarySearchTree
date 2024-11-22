
#include <iostream>

template <typename T>
class BinarySearchTree
{
private:
  BinarySearchTree* parent = nullptr;
  BinarySearchTree* left = nullptr;
  BinarySearchTree* right = nullptr;
  int64_t key;
public:
  T data;
  BinarySearchTree(int64_t key, T data) : key(key), data(data) {
  }
  void Insert(BinarySearchTree node) {
    if (node.key < key)
    {
      if (node.right == nullptr)
      {
        node.right = this;
        parent = &node;
      }
      else
      {
        Insert(*node.right);
      }
    }
    else
    {
      if (node.left == nullptr)
      {
        node.left = this;
        parent = &node;
      }
      else
      {
        Insert(*node.left);
      }
    }
  }

};

int main()
{
  std::cout << "Hello World!\n";
}
