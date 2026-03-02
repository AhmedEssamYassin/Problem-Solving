#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
Push DP, where we update future states based on the current state
Pull DP, where we calculate the current state based on past states
*/

ll pushDP(const vector<ll> &h, int K)
{
    int N = h.size();
    vector<ll> dp(N + 1, LLONG_MAX);
    dp[0] = 0; // We are already here
    // Push DP
    for (int i = 0; i < N; i++)
    {
        for (int j = 1; j <= K; j++)
        {
            if (i + j < N)
                dp[i + j] = min(dp[i + j], dp[i] + abs(h[i] - h[i + j]));
        }
    }
    return dp[N - 1];
}

ll pullDP(const vector<ll> &h, int K)
{
    int N = h.size();
    vector<ll> dp(N + 1, LLONG_MAX);
    dp[0] = 0; // We are already here
    // Pull DP
    for (int i = 0; i < N; i++)
    {
        for (int j = 1; j <= K; j++)
        {
            if (i - j >= 0)
                dp[i] = min(dp[i], dp[i - j] + abs(h[i] - h[i - j]));
        }
    }
    return dp[N - 1];
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
        int N, K;
        cin >> N >> K;
        vector<ll> h(N);
        for (int i = 0; i < N; i++)
            cin >> h[i];
        assert(pushDP(h, K) == pullDP(h, K));
        cout << pushDP(h, K);
    }
    return 0;
}
