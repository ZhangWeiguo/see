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

void view(TreeNode* root, vector<int>& v, int deep) {
  if (v.size() <= deep) {
    v.push_back(root->val);
  }
  if (root->right != NULL) {
    view(root->right, v, deep + 1);
  }
  if (root->left != NULL) {
    view(root->left, v, deep + 1);
  }
}

class Solution {
 public:
  vector<int> rightSideView(TreeNode* root) {
    vector<int> v;
    if (root != NULL) {
      view(root, v, 0);
    }
    return v;
  }
};