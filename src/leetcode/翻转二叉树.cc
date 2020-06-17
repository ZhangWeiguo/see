#include<iostream>
using namespace std;

struct TreeNode{
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) {
    val = x;
    left = NULL;
    right = NULL;
  }
};

void reverse(TreeNode *t) {
  if (t == NULL) return;
  TreeNode *left = t->left;
  TreeNode *right = t->right;
  t->left = right;
  t->right = left;
  reverse(left);
  reverse(right);
}

int depth(TreeNode *t) {
  if (t == NULL) return 0;
  TreeNode *left = t->left;
  TreeNode *right = t->right;
  int left_depth = 0, right_depth = 0;
  if (left != NULL) left_depth = depth(left);
  if(right != NULL) right_depth = depth(right);
  return max(left_depth + 1, right_depth + 1);
}

void print(TreeNode *t) {
  if (t == NULL) return;
  cout << t->val << ":";
  if (t->left != NULL) print(t->left);
  if (t->right != NULL) print(t->right);
}


int main() {
  TreeNode t0(4);
  TreeNode t1(2);
  TreeNode t2(7);
  TreeNode t3(1);
  TreeNode t4(3);
  TreeNode t5(6);
  TreeNode t6(9);
  t0.left = &t1;
  t0.right = &t2;
  t1.left = &t3;
  t1.right = &t4;
  t2.left = &t5;
  t2.right = &t6;
  print(&t0);
  cout << endl;
  reverse(&t0);
  print(&t0);
  return 0;
}