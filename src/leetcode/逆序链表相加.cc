#include<vector>
using namespace std;

 struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};


ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode *L1 = l1, *L2 = l2;
    int left_val = 0;
    ListNode *new_node = NULL, *root_node = NULL;
    while (L1 || L2 || left_val > 0) {
        ListNode *this_node = new ListNode(left_val);
        if (L1) {
            this_node->val += L1->val;
            L1 = L1->next;
        }
        if (L2) {
            this_node->val += L2->val;
            L2 = L2->next;
        }
        if (this_node->val >= 10) {
            left_val = this_node->val / 10;
            this_node->val %= 10;
        } else {
            left_val = 0;
        }
        if (new_node) {
            new_node->next = this_node;
            new_node = this_node;
        } else {
            new_node = this_node;
            root_node = this_node;
        }
    }
    return root_node; 