
#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;


struct TreeNode {
    char val;
    int leaves_num;
    unordered_map<char, TreeNode*> childs;
    TreeNode() {
        val = '0';
        leaves_num = 1;
        unordered_map<char, TreeNode*>().swap(childs);
    }
    TreeNode(char x) {
        val = x;
        leaves_num = 1;
        unordered_map<char, TreeNode*>().swap(childs);
    }
    TreeNode* SearchChild(char x) {
        auto it = childs.find(x);
        if (it != childs.end()) return it->second;
        return NULL;
    }
    TreeNode* AddChild(char x) {
        auto it = childs.find(x);
        if (it == childs.end()) {
            TreeNode *new_node = new TreeNode(x);
            childs[x] = new_node;
            return childs[x];
        } else {
            it->second->leaves_num++;
            return it->second;
        }
    }
    void AddString(string xs) {
        TreeNode *root = NULL;
        for (int i = 0; i < xs.size(); i++) {
            char x = xs[i];
            if (i == 0) {
                root = AddChild(x);
            } else {
                root = root->AddChild(x);
            }
        }
    }
};


TreeNode* build_tree(vector<string> &words) {
    TreeNode *root = new TreeNode();
    for (string &it: words) {
        root->AddString(it);
    }
    return root;
}

string find(TreeNode* root, string search_word) {
    string return_s = "";
    TreeNode *first = root;
    for (int i = 0; i < search_word.size(); i++) {
        if (first == NULL) return return_s;
        char x = search_word[i];
        auto it = first->SearchChild(x);
        if (it != NULL) {
            return_s.push_back(x);
        }
        first = it;
    }
    if (first != NULL) {
        while(first) {
            int max_num = 0;
            TreeNode *new_root = NULL;
            for (auto &it: first->childs) {
                if (it.second->leaves_num > max_num) {
                    max_num = it.second->leaves_num;
                    new_root = it.second;
                }
            }
            first = new_root;
            if (first) return_s.push_back(first->val);
        }
    }
    return return_s;
}



int main() {
    vector<string> words = {
        "tencentmusic", "tencent",
        "tencentmusic","tencentmusic",
        "tencentqq", "tencentmusic",
        "tencentqq", "tencentqq"};
    TreeNode *root = build_tree(words);
    cout << find(root, "tencent") << endl;
    return 0;
}