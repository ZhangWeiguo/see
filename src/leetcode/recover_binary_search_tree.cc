#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
struct TreeNode {
  int val;
  TreeNode* left;
  TreeNode* right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

void Valid(TreeNode* root, vector<TreeNode*>& L) {
  if (root->left != NULL) Valid(root->left, L);
  if (root != NULL) L.push_back(root);
  if (root->right != NULL) Valid(root->right, L);
}

void compare(vector<TreeNode*> L) {
  int x, k;
  for (int i = 0; i < L.size() - 1; i++) {
    x = L[i]->val;
    k = i;
    for (int j = i + 1; j < L.size(); j++) {
      if (L[j]->val < x) {
        x = L[j]->val;
        k = j;
      }
    }
    if (k != i) {
      L[k]->val = L[i]->val;
      L[i]->val = x;
      return;
    }
  }
}

class Solution {
 public:
  void recoverTree(TreeNode* root) {
    vector<TreeNode*> L;
    Valid(root, L);
    compare(L);
  }
};