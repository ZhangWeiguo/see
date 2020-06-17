#include <iostream>

using namespace std;
struct TreeNode {
  int val;
  TreeNode* left;
  TreeNode* right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

bool isValid(TreeNode* root, int lv, int rv, bool left, bool right) {
  bool r = true;
  if (root->left != NULL) {
    if (root->left->val >= root->val) {
      r = false;
    }
    if (left) {
      if (root->left->val >= lv) {
        r = false;
      }
    }
    if (right) {
      if (root->left->val <= rv) {
        r = false;
      }
    }
    if (r) {
      r = isValid(root->left, max(root->val, lv), rv, true, right);
    }
  }
  if (root->right != NULL) {
    if (root->right->val <= root->val) {
      r = false;
    }
    if (left) {
      if (root->right->val >= lv) {
        r = false;
      }
    }
    if (right) {
      if (root->right->val <= rv) {
        r = false;
      }
    }
    if (r) {
      r = isValid(root->right, lv, min(root->val, rv), left, true);
    }
  }
  return r;
}

class Solution {
 public:
  bool isValidBST(TreeNode* root) {
    if (root == NULL) return true;
    return isValid(root, root->val, root->val, false, false);
  }
};
