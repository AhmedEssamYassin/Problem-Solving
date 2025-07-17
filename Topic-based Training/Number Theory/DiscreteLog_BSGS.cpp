#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Returns minimum x for which a ^ x = b (mod m)
ll solve(ll a, ll b, ll m)
{
    a %= m, b %= m;
    ll k = 1, add = 0, g;
    while ((g = gcd(a, m)) > 1)
    {
        if (b == k)
            return add;
        if (b % g)
            return -1;
        b /= g, m /= g, add++;
        k = (k * a / g) % m;
    }

    ll n = sqrt(m) + 1;
    ll an = 1;
    for (ll i = 1; i <= n; i++)
        an = (an * a) % m;

    unordered_map<ll, ll> vals;
    for (ll q = 0, cur = b; q <= n; q++)
    {
        vals[cur] = q;
        cur = (cur * a) % m;
    }

    for (ll p = 1, cur = k; p <= n; p++)
    {
        cur = (cur * an) % m;
        if (vals.count(cur))
            return n * p - vals[cur] + add;
    }
    return -1;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
    int t = 1;
    ll N;
    cin >> t;
    while (t--)
    {
        ll a, b, m;
        cin >> a >> b >> m;
        // Handle special case when m = 1
        if (m == 1)
        {
            cout << 0 << endl; // Any number mod 1 = 0, so a^0 = 1 ≡ 0 (mod 1)
            continue;
        }

        // Handle special case when a = 0
        if (a == 0)
        {
            if (b % m == 1)
                cout << 0 << endl; // 0^0 = 1 (by convention)
            else if (b % m == 0)
                cout << 1 << endl; // 0^1 = 0
            else
                cout << -1 << endl; // 0^k can never equal non-zero b (except b = 1 when k=0)
            continue;
        }
        cout << solve(a, b, m) << endl;
    }
    return 0;
}
