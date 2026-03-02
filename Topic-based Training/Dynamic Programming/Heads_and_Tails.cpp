#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

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
        int N;
        cin >> N;
        vector<long double> p(N);
        for (int i{}; i < N; i++)
            cin >> p[i];

        // dp[i][h] = probability that after tossing first `i` coins, exactly `h` coins landed heads.
        vector<vector<long double>> dp(N + 1, vector<long double>(N + 1, 0.0L));
        dp[0][0] = 1.0L;
        for (int i = 1; i <= N; i++)
        {
            for (int h = 0; h <= i; h++)
            {
                if (h > 0)
                    dp[i][h] += dp[i - 1][h - 1] * p[i - 1];
                dp[i][h] += dp[i - 1][h] * (1 - p[i - 1]);
            }
        }

        long double ans = 0.0L;
        for (int h = (N + 1) / 2; h <= N; h++)
            ans += dp[N][h];

        cout << fixed << setprecision(10) << ans;
    }
    return 0;
}
