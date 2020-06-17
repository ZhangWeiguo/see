#include <iostream>
#include <limits.h>
#include <vector>
#include <math.h>
using namespace std;

class Solution {
public:
    static int reverse(int x) {
        long y = 0, xx = abs(x), tmp = 0;
        vector<int> yy;
        while (xx > 0) {
            tmp = xx % 10;
            yy.emplace_back(tmp);
            xx = xx / 10;
        }
        tmp = 1;
        for (int i = 0;  i < yy.size(); i++) {
            y += yy[yy.size() - 1 - i] * tmp;
            if (y > INT_MAX) return 0;
            tmp *= 10;
        }
        if (x < 0) y *= -1;
        return y;  
    }
};


int main() {
    cout << Solution::reverse(1534236469) << endl;
    cout << Solution::reverse(-321) << endl;
    return 0;
}