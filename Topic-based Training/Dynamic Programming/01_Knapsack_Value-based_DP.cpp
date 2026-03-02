#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
DP Style: 0/1 (Take or Leave) Knapsack (Value-Based DP)

State:
    dp[i][j] = minimum weight using first `i` items and total value exactly `j`

Transition:
    dp[i][j] = dp[i - 1][j]                                   // Leave item i
    if (j - v[i - 1] >= 0)
        dp[i][j] = min(dp[i][j], dp[i - 1][j - v[i - 1]] + w[i - 1])  // Take item i

Initialization:
    dp[0][0] = 0 (0 weight for 0 value), all other dp[0][j > 0] = INF

Final Answer:
    max(j) such that dp[n][j] <= maxW = maximum achievable value within weight limit
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
        ll valueSum{};
        for (int i{}; i < n; i++)
        {
            cin >> w[i] >> v[i];
            valueSum += v[i];
        }

        vector<vector<ll>> dp(n + 1, vector<ll>(valueSum + 1, INT_MAX));
        // dp[i][j] = minimum weight using first `i` items and total value exactly `j`
        dp[0][0] = 0;
        for (int i{1}; i <= n; i++)
        {
            for (int j = 0; j <= valueSum; j++)
            {
                // Leave
                dp[i][j] = dp[i - 1][j];
                // Take
                if (j - v[i - 1] >= 0)
                    dp[i][j] = min(dp[i][j], dp[i - 1][j - v[i - 1]] + w[i - 1]);
            }
        }
        ll ans = 0;
        for (int j = valueSum; j >= 0; j--)
        {
            if (dp[n][j] <= maxW)
            {
                ans = j;
                break;
            }
        }
        cout << ans << endl;
    }
    return 0;
}
