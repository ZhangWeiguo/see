#include<iostream>
#include<vector>
using namespace std;

// 从begin跳到end 每天只能向前或者向后跳N步 最少需要多少天
int dfs_search(int index, int begin, int end, int days, int curr_min_days) {
    cout << index << ": " << end << ": " << days << ": " << curr_min_days << endl;
    if (index + days + 1 == end || index - days - 1 == end) {
        return days + 1;
    }
    // 最多跳 2 * N 天
    if (days >= 2 * abs(end - begin)) return 2 * days;
    if (days >= curr_min_days && curr_min_days != -1) return curr_min_days;
    int forward_days = dfs_search(index + days + 1, begin, end, days + 1, curr_min_days);
    if (curr_min_days == -1) curr_min_days = forward_days;
    else if (forward_days < curr_min_days) curr_min_days = forward_days;
    int afterward_days = dfs_search(index - days - 1, begin, end, days + 1, curr_min_days);
    if (forward_days > afterward_days) return afterward_days;
    else return forward_days;
}

int min_days(int begin, int end) {
    return dfs_search(begin, begin, end, 0, -1);
}

int main() {
    cout << min_days(0, 6) << endl;
    return 0;
}
