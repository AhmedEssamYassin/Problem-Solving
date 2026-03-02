#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
DP Style: Longest Common Subsequence (LCS)

State:
    dp[i][j] = length of LCS between first `i` characters of string `a`
                         and first `j` characters of string `b`

Transition:
    if (a[i - 1] == b[j - 1])
        dp[i][j] = dp[i - 1][j - 1] + 1                     // Characters match
    else
        dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])          // Skip one character

Initialization:
    dp[0][j] = 0 for all j (empty `a` vs `b`)
    dp[i][0] = 0 for all i (empty `b` vs `a`)

Final Answer:
    dp[n][m] = length of LCS between full strings `a` (length `n`) and `b` (length `m`)
*/

string LCS(const string &S, const string &T)
{
    int n = S.length();
    int m = T.length();
    vector<vector<ll>> dp(n + 1, vector<ll>(m + 1));
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            if (S[i - 1] == T[j - 1]) // Match
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else // Mismatch
                dp[i][j] = max(dp[i][j - 1], dp[i - 1][j]);
        }
    }
    // Backtrack to find LCS string
    string lcs;
    int i = n, j = m;
    while (i > 0 && j > 0)
    {
        if (S[i - 1] == T[j - 1])
        {
            lcs += S[i - 1];
            i--;
            j--;
        }
        else if (dp[i - 1][j] > dp[i][j - 1])
            i--;
        else
            j--;
    }
    reverse(lcs.begin(), lcs.end());
    return lcs;
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
        string S, T;
        cin >> S >> T;
        cout << LCS(S, T);
    }
    return 0;
}
