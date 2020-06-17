#include<iostream>
#include<vector>
#include<unordered_set>
using namespace std;

struct TreeNode {
  int val;
  TreeNode* left;
  TreeNode* right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
 public:
  TreeNode* reConstructBinaryTree(vector<int> pre, vector<int> vin) {
    if (pre.size() > 0) {
      TreeNode *root = new TreeNode(pre[0]);
      vector<int> left_pre, left_vin, right_pre, right_vin;
      unordered_set<int> left_set, right_set;
      bool left = true;
      for (int i = 0; i < vin.size(); i++) {
        if (vin[i] == pre[0]) {
          left = false;
          continue;
        }
        if (left) {left_vin.emplace_back(vin[i]); left_set.insert(vin[i]);}
        else {right_vin.emplace_back(vin[i]); right_set.insert(vin[i]);}
      }
      for (int i = 1; i < pre.size(); i++) {
        if (left_set.count(pre[i]) > 0) left_pre.emplace_back(pre[i]);
        else right_pre.emplace_back(pre[i]);
      }
      TreeNode *left_node = reConstructBinaryTree(left_pre, left_vin);
      TreeNode *right_node = reConstructBinaryTree(right_pre, right_vin);
      root->left = left_node;
      root->right = right_node;
      return root;
    } else {
      return NULL;
    }
  }


  TreeNode* buildTree(vector<int>& pre, vector<int>& vin) {
    return buildTree(pre, vin, 0, 0, pre.size() - 1);
  }

  TreeNode* buildTree(vector<int>& pre, vector<int>& vin, int root_index, int start, int end) {
    if (root_index >= pre.size() || start > end) return NULL;
    TreeNode *root = new TreeNode(pre[root_index]);
    if (start == end) return root;
    int i = 0;
    while (start + i <= end) {
      if (vin[start+i] == pre[root_index]) {
        break;
      }
      i++;
    }
    TreeNode *left = buildTree(pre, vin, root_index + 1, start, start + i - 1);
    TreeNode *right = buildTree(pre, vin, root_index + 1 + i, start + i + 1, end);
    root->left = left;
    root->right = right;
    return root;
  }
};

int main() {
  vector<int> pre, vin;
  pre.push_back(10);
  TreeNode* T;
  Solution S;
  T = S.reConstructBinaryTree(pre, vin);
  cout << T->val << endl;
  return 0;
}
