#include <iostream>
#include <limits.h>
#include <vector>
#include <string>
using namespace std;

class Solution {
public:
    int myAtoi(string str) {
        int64_t ans = 0, flag = 1, i = 0, j;
        while(i < str.size() && str[i] == ' ')  
            i++;
        if(i < str.size() && !isdigit(str[i]) && str[i] != '-' && str[i] != '+')
            return ans;
        j = i;
        while(j < str.size() && !isdigit(str[j])) 
            j++;
        if(j-i > 1)
            return ans;
        else if(str[i] == '-')
            flag = -1;
        else if(str[i] == '+')
            flag = 1;
        while(j < str.size() && isdigit(str[j]))        
        {
            ans = ans*10 + str[j++] - '0';
            if(ans > (int64_t)INT_MAX + 2)
                break;
        }
        ans*=flag;
        if(ans > INT_MAX)
            ans = INT_MAX;
        else if(ans < INT_MIN)
            ans = INT_MIN;
        return ans;
    }
};

int main() {
    return 0;
}