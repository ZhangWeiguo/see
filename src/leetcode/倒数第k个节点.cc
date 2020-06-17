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

class Solution {
 public:
  ListNode* getKthFromEnd(ListNode* head, int k) {
    ListNode *pre = head, *next = head;
    for (int i = 0; i < k; i++) {
      next = next->next;
    }
    while (next != NULL) {
      pre = pre->next;
      next = next->next;
    }
    return pre;
  }
};