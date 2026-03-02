#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

int LIS(vector<ll> const &vc)
{
    int n = vc.size();
    const ll INF = LLONG_MAX;
    vector<ll> dp(n + 1, INF);
    dp[0] = -INF;

    for (int i = 0; i < n; i++)
    {
        int l = upper_bound(dp.begin(), dp.end(), vc[i]) - dp.begin();
        if (dp[l - 1] < vc[i] && vc[i] < dp[l])
            dp[l] = vc[i];
    }

    int ans = 0;
    for (int l = 0; l <= n; l++)
    {
        if (dp[l] < INF)
            ans = l;
    }
    return ans;
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
        int n;
        cin >> n;
        vector<ll> vc(n);
        for (int i{}; i < n; i++)
            cin >> vc[i];
        cout << LIS(vc);
    }
    return 0;
}
