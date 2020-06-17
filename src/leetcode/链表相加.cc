#include<iostream>
#include<vector>
using namespace std;

struct Node {
  Node(int x) {
    next = NULL;
    val = x;
  }
  int val;
  Node *next;
};

void delete_node(Node *root) {
  Node *node = root;
  while (node != NULL) {
    cout << node->val << " ";
    Node *tmp = node;
    node = node->next;
    delete tmp;
  }
  cout << endl;
}


int add(Node *node1, Node *node2, int index1, int index2, Node *new_node) {
  cout << "TMP: " << index1 << ": " << index2 << endl;
  Node *next_node = new Node(0);
  int x;
  if (index1 == 0 && index2 == 0) {
    new_node->val = node1->val + node2->val;
    delete next_node;
  } else if (index1 == index2) {
    x = add(node1->next, node2->next, index1 - 1, index2 - 1, next_node);
    new_node->val = node1->val + node2->val + x;
    new_node->next = next_node;
  } else {
    if (index1 > index2) {
      x = add(node1->next, node2, index1 - 1, index2, next_node);
      new_node->val = node1->val + x;
      new_node->next = next_node;
    } else {
      x = add(node1, node2->next, index1, index2 - 1, next_node);
      new_node->val = node2->val + x;
      new_node->next = next_node;
    }
  }
  int left = 0;
  while(new_node->val >= 10) {
    new_node->val = new_node->val % 10;
    left = new_node->val / 10;
  }
  cout << "Output:" << new_node->val << ": " << x << endl; 
  return left;
}

int main() {
  int n1, n2;
  cin >> n1 >> n2;
  Node *root1 = NULL, *next1 = NULL, *root2 = NULL, *next2 = NULL;
  for (int i = 0; i < n1 + n2; i++) {
    int x;
    cin >> x;
    Node *new_node = new Node(x);
    if (i < n1) {
      if (i == 0) {
        root1 = new_node;
        next1 = new_node;
      } else {
        next1->next = new_node;
        next1 = new_node;
      }
    } else {
      if (i == n1) {
        root2 = new_node;
        next2 = new_node;
      } else {
        next2->next = new_node;
        next2 = new_node;
      }
    }
  }
  Node *new_node = new Node(0);
  int x = add(root1, root2, n1 - 1, n2 - 1, new_node);
  if (x > 0) {
    while (x > 0) {
      int last_x = x % 10;
      Node *last_node = new Node(last_x);
      last_node->next = new_node;
      x = x / 10;
      new_node = last_node;
    }
  }
  delete_node(root1);
  delete_node(root2);
  delete_node(new_node);
  return 0;
}