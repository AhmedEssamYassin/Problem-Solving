#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

vector<int> mu;
void preComputeMobius(ll N)
{
    mu.resize(N + 1);
    mu[1] = -1;
    for (int i = 1; i <= N; i++)
    {
        if (mu[i])
        {
            mu[i] = -mu[i];
            for (int j = 2 * i; j <= N; j += i)
                mu[j] += mu[i];
        }
    }
}

static int autoCall = (preComputeMobius(2e5), 0);

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
    int t = 1;
    ll N, p;
    cin >> t;
    while (t--)
    {
        cin >> N >> p;
        vector<ll> a(N + 1);
        for (int i{1}; i <= N; i++)
            cin >> a[i];

        vector<ll> pre(N + 1, 0);
        for (int i{2}; i <= N; i++)
        {
            for (ll j = i; j <= N; j += i)
                pre[i] += a[j];
        }
        ll sum = accumulate(a.begin(), a.end(), 0LL);
        ll cur = sum;
        for (int i{}; i < p; i++)
        {
            ll k;
            cin >> k;
            cur = sum;
            vector<ll> divisors{k};
            for (ll d = 2; d * d <= k; d++)
            {
                if (k % d == 0)
                {
                    ll f1 = d;
                    ll f2 = k / d;
                    divisors.push_back(f1);
                    if (f2 != f1)
                        divisors.push_back(f2);
                }
            }
            for (const ll &d : divisors)
                cur += mu[d] * pre[d];
            cout << cur << " ";
        }
        cout << endl;
    }
    return 0;
}