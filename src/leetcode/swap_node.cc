#include <iostream>
#include <vector>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
      if (head == NULL || head->next == NULL) return head;
        ListNode *n0, *n1, *root, *pre, *next;
        root = head;
        pre = NULL;
        n0 = root;
        k = 0;
        while (n0 != NULL) {
          n1 = n0->next;
          if (n1 != NULL) {
            next = n1->next;
            n1->next = n0;
            n0->next = next;
            if (pre != NULL) {
              pre->next = n1;
            } else {
              root = n1;
            }
            pre = n0;
            n0 = next;
          } else {
            next = NULL;
            n0 = n1;
          }
        }
      return root;
    }
};

int main() {

}
