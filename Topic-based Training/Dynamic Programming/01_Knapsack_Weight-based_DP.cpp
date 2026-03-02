#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
DP Style: 0/1 (Take or Leave) Knapsack (Weight-based)

State:
    dp[i][j] = maximum value using first `i` items and total weight exactly `j`

Transition:
    dp[i][j] = dp[i - 1][j] // Leave
    dp[i][j] = max(dp[i][j], dp[i - 1][j - w[i - 1]] + v[i - 1])  // Take the i-th item (if j - w[i-1] >= 0)

Final Answer:
    dp[n][maxW] = maximum value with total weight ≤ maxW

*/

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
        int n, maxW;
        cin >> n >> maxW;
        vector<ll> w(n), v(n);
        for (int i{}; i < n; i++)
            cin >> w[i] >> v[i];

        vector<vector<ll>> dp(n + 1, vector<ll>(maxW + 1));
        // dp[i][j] = max value using first `i` items and total weight exactly `j`
        for (int i{1}; i <= n; i++)
        {
            for (int j = 0; j <= maxW; j++)
            {
                // Leave
                dp[i][j] = dp[i - 1][j];
                // Take
                if (j - w[i - 1] >= 0)
                    dp[i][j] = max(dp[i][j], dp[i - 1][j - w[i - 1]] + v[i - 1]);
            }
        }
        cout << dp[n][maxW];
    }
    return 0;
}
