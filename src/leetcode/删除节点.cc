#include<iostream>
#include<vector>
using namespace std;

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

ListNode* removeNthFromEndB(ListNode* head, int n) {
  ListNode *first = head, *second = head, *tmp = NULL;
  for (int i = 0; i < n; i++) {
    second = second->next;
  }
  while(second != NULL) {
    tmp = first;
    first = first->next;
    second = second->next;
  }
  if (first != second) {
    if (tmp == NULL) {
      tmp = first->next;
      delete first;
      return tmp;
    } else {
      tmp->next = first->next;
      delete first;
      return head;
    }
  } else {
    delete first;
    return NULL;
  }
}

ListNode* removeNthFromEndA(ListNode* head, int n) {
   vector<ListNode*> nodes;
   while (head != NULL) {
     nodes.emplace_back(head);
     head = head->next;
   }
   int index = nodes.size() - n;
   ListNode *tmp = nodes[index];
   if (index - 1 >= 0) {
      nodes[index - 1]->next = tmp->next;
      delete tmp;
      return nodes[0];
   } else {
     ListNode *x = tmp->next;
     delete tmp;
     return x;
   }
}

int main() {

  return 0;
}