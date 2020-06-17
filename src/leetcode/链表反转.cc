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

void print(ListNode *n) {
  ListNode *n1 = n;
  while (n1 != NULL) {
    cout << n1->val << ";";
    n1 = n1->next;
  }
  cout << endl;
}

ListNode* reverse(ListNode *n) {
  if (n == NULL) return n;
  ListNode *pre = n, *next = n->next;
  n->next = NULL;
  while (next != NULL) {
    ListNode *tmp = next->next;
    next->next = pre;
    pre = next;
    next = tmp;
  }
  return pre;
}

int main(int argc, char **argv) {
  ListNode n0(1), n1(2), n2(3), n3(4);
  n0.next = &n1;
  n1.next = &n2;
  n2.next = &n3;
  print(&n0);
  ListNode *nn = reverse(&n0);
  print(nn);
  return 0;
}
