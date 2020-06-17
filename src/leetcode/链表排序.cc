#include<iostream>
using namespace std;

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int v) {
    val = v;
    next = NULL;
  }
};

ListNode* merge(ListNode *L1, ListNode *L2) {
    ListNode *root = NULL, *new_node = NULL;
    while (L1 && L2) {
        ListNode *next_node;
        if (L1->val < L2->val) {
            next_node = L1;
            L1 = L1->next;
        } else {
            next_node = L2;
            L2 = L2->next;
        }
        if (!root) {
            root = next_node;
            new_node = next_node;
        } else {
            new_node->next = next_node;
            new_node = next_node;
        }
    }
    if (root) {
        if (L1) new_node->next = L1;
        else if (L2) new_node->next = L2;
    } else {
        if (L1) root = L1;
        else if (L2) root = L2;        
    }
    return root;
}

// 快慢指针求中点

ListNode* sortList(ListNode* head) {
    if (head == NULL || head->next == NULL) return head;
    ListNode *pre = head, *next = head, *before = head;
    while(next != NULL && next->next != NULL) {
        before = pre;
        pre = pre->next;
        next = next->next->next;
    }
    sortList(pre);
    before->next = NULL;
    sortList(head);
    ListNode *new_node = merge(head, pre->next);
    return new_node;
}