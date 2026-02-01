#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

void getAllFactors(ll N, vector<ll> &factors)
{
    for (ll d = 1; d * d <= N; d++)
    {
        if (N % d == 0)
        {
            ll f1 = d;
            ll f2 = N / d;
            factors.push_back(f1);
            if (f2 != f1)
                factors.push_back(f2);
        }
    }
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
    ll N;
    cin >> t;
    while (t--)
    {
    }
    return 0;
}
