#include<iostream>
#include<unordered_set>
#include<unordered_map>
#include<string>
#include<vector>
using namespace std;

int find_way(int begin, int end, unordered_map<int, unordered_set<int>> &conn, vector<int> walked, unordered_set<int> walked_set, int curr_min_steps) {
    int last_point = walked[walked.size() - 1];
    if (last_point == end) {
        for (auto &it: walked) {
            cout << it << " ";
        }
        cout << endl;
        return walked.size();
    }
    unordered_set<int> &choose = conn[last_point];
    if (choose.count(end) > 0) return walked.size() + 1;
    int min_steps = conn.size() + 1;
    if (walked.size() + 1 >= curr_min_steps) return curr_min_steps;
    for (auto &it: choose) {
        if (walked_set.count(it) == 0) {
            vector<int> new_walked = walked;
            unordered_set<int> new_walked_set = walked_set;
            new_walked.emplace_back(it);
            new_walked_set.insert(it);
            min_steps = min(min_steps, find_way(begin, end, conn, new_walked, new_walked_set, min_steps));
        }
    }
    return min_steps;
}

int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
    unordered_set<string> word_set(wordList.begin(), wordList.end());
    word_set.insert(beginWord);
    if (word_set.count(endWord) == 0) return 0;
    // word_set.insert(endWord);
    int i = 0, word_size = beginWord.size();
    unordered_map<string, int> word_map;
    unordered_map<int, string> word_map_inv;
    unordered_map<int, unordered_set<int>> word_conn_map;
    for (auto &it: word_set) {
        word_map_inv[i] = it;
        word_conn_map[i] = unordered_set<int>();
        word_map[it] = i++;
    }
    for (i = 0; i < word_map.size(); i++) {
        for (int j = 0; j < i; j++) {
            int x = 0;
            for (int k = 0; k < word_size; k++) {
                if (word_map_inv[i][k] == word_map_inv[j][k]) x++;
            }
            if (x==(word_size-1)) {
                word_conn_map[i].insert(j);
                word_conn_map[j].insert(i);
            }
        }
    }
    for (auto &it: word_map) {
        cout << it.first << ": " << it.second << endl;
    }
    for (auto &it: word_conn_map) {
        cout << it.first << ": ";
        for (auto &jt: it.second) {
            cout << jt << " ";
        }
        cout << endl;
    }
    vector<int> new_walked = {word_map[beginWord]};
    unordered_set<int> new_walked_set = {word_map[beginWord]};
    int min_steps = find_way(word_map[beginWord], word_map[endWord], word_conn_map, new_walked, new_walked_set, word_map.size() + 1);
    if (min_steps == word_map.size() + 1) min_steps = 0;
    return min_steps;
}

int ladderLengthB(string beginWord, string endWord, vector<string>& wordList) {
    unordered_set<string> word_set(wordList.begin(), wordList.end());
    word_set.insert(beginWord);
    if (word_set.count(endWord) == 0) return 0;
    int i = 0, word_size = beginWord.size();
    unordered_map<string, unordered_set<string>> word_conn_map;
    for (auto &it: word_set) {
        word_conn_map[it] = unordered_set<string>();
    }
    for (auto &it: word_set) {
        for (int j = 0; j < word_size; j++) {
            string new_s = it;
            for (char x = 'a'; x <= 'z'; x++) {
                if (it[j] != x) {
                    new_s[j] = x;
                    if (word_set.count(new_s) > 0) {
                        word_conn_map[it].insert(new_s);
                        word_conn_map[new_s].insert(it);
                    }
                }
            }
        }
    }
    int steps = 1;
    unordered_set<string> can_walked = {beginWord};
    while (true) {
        if (can_walked.size() == word_conn_map.size()) break;
        steps++;
        unordered_set<string> tmp_walked(can_walked.begin(), can_walked.end());
        int add_size = 0;
        for (auto &it: tmp_walked) {
            auto x = word_conn_map[it];
            for (auto &jt: x) {
                if (can_walked.count(jt) == 0) {
                    can_walked.insert(jt);
                    add_size++;
                    if (can_walked.count(endWord) > 0) {
                        return steps;
                    }
                }
            }
        }
        if (add_size == 0) break;
    }
    return 0;
}

int main() {
    vector<string> word_list = {"hot","dot","dog","lot","log","cog"};
    string begin = "hit", end = "cog";
    cout << ladderLengthB(begin, end, word_list) << endl;
}

