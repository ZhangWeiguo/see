#include<iostream>
#include<vector>
#include<map>
#include<bits/shared_ptr.h>
using namespace std;

/*
链接：https://www.nowcoder.com/questionTerminal/70e00e490b454006976c1fdf47f155d9
来源：牛客网
有一棵无穷大的满二叉树，其结点按根结点一层一层地从左往右依次编号，根结点编号为1。现在有两个结点a，b。
请设计一个算法，求出a和b点的最近公共祖先的编号。
给定两个int a,b。为给定结点的编号。请返回a和b的最近公共祖先的编号。注意这里结点本身也可认为是其祖先。
*/
class LCA {
public:
    int getLCA(int a, int b) {
        if (a > b) return getLCA(a/2, b);
        else if (a < b) return getLCA(a, b/2);
        else return a;
    }
};

/*
给定一棵二叉树以及这棵树上的两个节点 o1 和 o2，请找到 o1 和 o2 的最近公共祖先节点。 
链接：https://www.nowcoder.com/questionTerminal/c75deef6d4bf40249c785f240dad4247
来源：牛客网

第一行输入两个整数 n 和 root，n 表示二叉树的总节点个数，root 表示二叉树的根节点。
以下 n 行每行三个整数 fa，lch，rch，表示 fa 的左儿子为 lch，右儿子为 rch。
(如果 lch 为 0 则表示 fa 没有左儿子，rch同理)
最后一行为节点 o1 和 o2。
*/

struct TreeNode{
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x): val(x){
    left = NULL;
    right = NULL;
  }
  TreeNode(TreeNode &node) {
    val = node.val;
    left = node.left;
    right = node.right;
  }
};

TreeNode* find_parent(TreeNode *root, TreeNode *left_node, TreeNode *right_node) {
  if (left_node->val == right_node->val) return left_node;
  if (root == NULL) return NULL;
  if (root->val == left_node->val || root->val == right_node->val) return root;
  auto left = find_parent(root->left, left_node, right_node);
  auto right = find_parent(root->right, left_node, right_node);
  if (left != NULL && right != NULL) return root;
  else if (left != NULL) return left;
  else if (right != NULL) return right;
  else return NULL;
}

int main() {
  map<int, shared_ptr<TreeNode> > node_dict;
  int root_val, node_num;
  cin >> node_num >> root_val;
  node_dict[root_val] = make_shared<TreeNode>(root_val);
  for (int i = 0; i < node_num; i++) {
    int val, left_val, right_val;
    cin >> val >> left_val >> right_val;
    if (node_dict.count(val) == 0) {
      node_dict[val] = make_shared<TreeNode>(val);
    }
    if (left_val > 0 && node_dict.count(left_val) == 0) {
      node_dict[left_val] = make_shared<TreeNode>(left_val);
    }
    if (right_val > 0 && node_dict.count(right_val) == 0) {
      node_dict[right_val] = make_shared<TreeNode>(right_val);
    }
    if (left_val > 0) node_dict[val]->left = node_dict[left_val].get();
    if (right_val > 0) node_dict[val]->right = node_dict[right_val].get();
  }
  int val1, val2;
  cin >> val1 >> val2;
  auto new_node = find_parent(
    node_dict[root_val].get(),
    node_dict[val1].get(),
    node_dict[val2].get());
  cout << new_node->val << endl;
  return 0;
}