#include <iostream>
using namespace std;
struct ListNode {
  int val;
  ListNode* next;
  ListNode(int x) {
    val = x;
    next = NULL;
  }
};

class Solution {
 public:
  ListNode* addTwoNumbers(ListNode* L1, ListNode* L2) {
    int a, x;
    a = 0;
    ListNode* L3 = new ListNode(0);
    ListNode* l3 = L3;
    ListNode* l1 = L1;
    ListNode* l2 = L2;
    int n = 0;
    while (l1 != NULL || l2 != NULL || a > 0) {
      x = 0;
      if (l1 != NULL) {
        x += (*l1).val;
      }
      if (l2 != NULL) {
        x += (*l2).val;
      }
      x += a;
      if (x >= 10) {
        x -= 10;
        a = 1;
      } else {
        a = 0;
      }
      if (n == 0) {
        l3->val = x;
      } else {
        ListNode* l = new ListNode(x);
        l3->next = l;
        l3 = l3->next;
      }
      l1 = l1 != NULL ? l1->next : NULL;
      l2 = l2 != NULL ? l2->next : NULL;
      n++;
    }
    return L3;
  }
};

int main() {
  ListNode* L1 = new ListNode(2);
  ListNode* L2 = new ListNode(6);

  ListNode* L11 = new ListNode(4);
  ListNode* L12 = new ListNode(4);
  L1->next = L11;
  L2->next = L12;

  ListNode* L3 = new ListNode(0);
  Solution S;
  cout << L1->next->val << endl;
  L3 = S.addTwoNumbers(L1, L2);
  cout << L3->val;
  cout << 88888 << endl;
  return 0;
}
