// 2025.08.27 小红书（REDStar 推荐）一面
// CF1512C A-B Palindrome https://www.luogu.com.cn/problem/CF1512C

#include <iostream>
#include <cassert>
using namespace std;

string solve(string s, int a, int b) {
    int n = s.size();
    for (int i = 0; i < n; i++) {
        if (s[i] == '?' && (s[n - i - 1] == '0' || s[n - i - 1] == '1')) {
            s[i] = s[n - i - 1];
        }
        if (s[i] != s[n - i - 1] && s[n - i - 1] != '?') {
            return "-1";
        }
        if (s[i] == '0') {
            a--;
        } else if (s[i] == '1') {
            b--;
        }
    }

    if (n & 1 && s[n / 2] == '?') {
        if (a & 1) {
            s[n / 2] = '0';
            a--;
        } else if (b & 1) {
            s[n / 2] = '1';
            b--;
        } else {
            return "-1";
        }
    }

    int l = 0;
    int r = n - 1;
    while (l < r) {
        if (s[l] == '?') {
            if (a > 0) {
                s[l] = '0';
                s[r] = '0';
                a -= 2;
            } else if (b > 0) {
                s[l] = '1';
                s[r] = '1';
                b -= 2;
            } else {
                break;
            }
        }
        l++;
        r--;
    }

    if (a != 0 || b != 0 || l < r) {
        return "-1";
    }
    return s;
}

// int main() {
//     int t;
//     cin >> t;
//     while (t--) {
//         int a, b;
//         string s;
//         cin >> a >> b;
//         cin >> s;
//         cout << solve(s, a, b) << "\n";
//     }
    
//     return 0;
// }

void test() {
    assert(solve("01?????0", 4, 4) == "01011010");
    assert(solve("??????", 3, 3) == "-1");
    assert(solve("?", 1, 0) == "0");
    assert(solve("0101", 2, 2) == "-1");
    assert(solve("01?0", 2, 2) == "0110");
    assert(solve("0", 0, 1) == "-1");
    assert(solve("1?1", 0, 3) == "111");
    assert(solve("?00?", 2, 2) == "1001");
    assert(solve("??010?0", 4, 3) == "0101010");
}

int main() {
    test();
    return 0;
}
