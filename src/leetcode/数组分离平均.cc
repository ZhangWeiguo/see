#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
using namespace std;

int get_max(vector<int> &T, int max_value) {
    vector<int> max_content(max_value + 1, 0);
    for (int i = 1; i <= T.size(); i++) {
        for (int j = max_value; j >= 1; j--) {
            if (j >= T[i - 1]) {
                max_content[j] = max(max_content[j], max_content[j - T[i - 1]] + T[i - 1]);
            }
        }
    }
    return max_content[max_value];
}

int split_average(vector<int> &T) {
    int sum = 0;
    for (auto &it: T) {
        sum += it;
    }
    int sum_half_exp = sum/2;
    int sum_half_pre = get_max(T, sum_half_exp);
    return sum - 2 * sum_half_exp;
}


int main(int argc, char **argv) {
    cout << "all_num:" << argc << endl;
    for (int i = 0; i < argc; i++) {
        cout << argv[i] << ": " << strlen(argv[i]) << endl;
    }
    return 0;
}