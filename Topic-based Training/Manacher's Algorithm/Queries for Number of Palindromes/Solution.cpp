#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Manacher's algorithm to precompute palindrome radii
vector<int> manacherTransform(const string &s)
{
    string t = "#";
    for (const char &c : s)
    {
        t += c;
        t += "#";
    }
    int n = t.size();
    vector<int> P(n, 0);
    int center = 0, right = 0;
    for (int i = 0; i < n; i++)
    {
        int mirror = 2 * center - i;
        if (i < right)
            P[i] = min(right - i, P[mirror]);
        while (i - 1 - P[i] >= 0 && i + 1 + P[i] < n &&
               t[i - 1 - P[i]] == t[i + 1 + P[i]])
            P[i]++;
        if (i + P[i] > right)
        {
            center = i;
            right = i + P[i];
        }
    }
    return P;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif
    int t = 1;
    // cin >> t;
    while (t--)
    {
        string str;
        cin >> str;
        int n = str.length();
        vector<int> P = manacherTransform(str);

        // Checks if a str[l ... r] is palindrome in O(1)
        auto isPalindrome = [&](int l, int r) -> bool
        {
            int L = l * 2 + 1;
            int R = r * 2 + 1;
            int center = (L + R) / 2;
            int length = R - L;
            return P[center] >= (length / 2);
        };
        vector<vector<int>> dp(n + 1, vector<int>(n + 1));
        for (int i = n - 1; i >= 0; i--)
        {
            dp[i][i] = 1;
            for (int j = i + 1; j < n; j++)
                dp[i][j] = dp[i + 1][j] + dp[i][j - 1] - dp[i + 1][j - 1] + isPalindrome(i, j);
        }

        int q;
        cin >> q;
        while (q--)
        {
            int l, r;
            cin >> l >> r;
            l--;
            r--;
            cout << dp[l][r] << endl;
        }
    }
    return 0;
}