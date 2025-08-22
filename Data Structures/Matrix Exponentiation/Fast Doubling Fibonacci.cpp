#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

inline ll add64(const ll &a, const ll &b, const ll &mod)
{
    __int128_t res = __int128_t(a) + b;
    if (res >= mod)
        res -= mod;
    return res;
}

inline ll sub64(const ll &a, const ll &b, const ll &mod)
{
    __int128_t res = __int128_t(a) - b;
    if (res < 0)
        res += mod;
    if (res >= mod)
        res -= mod;
    return res;
}

inline ll mult64(const ll &a, const ll &b, const ll &mod)
{
    return __int128_t(a) * b % mod;
}

// Fast Doubling method
pair<ll, ll> getNthFib(ll N, ll mod)
{
    ll a = 0, b = 1; // (F(0), F(1))
    if (N == 0)
        return {a, b};

    int msb = __lg(N);
    for (int i = msb; i >= 0; i--)
    {
        ll c = mult64(a, sub64(mult64(b, 2, mod), a, mod), mod); // F(2k)
        ll d = add64(mult64(a, a, mod), mult64(b, b, mod), mod); // F(2k + 1)
        if ((N >> i) & 1)
        {
            a = d;
            b = add64(c, d, mod);
        }
        else
        {
            a = c;
            b = d;
        }
    }
    return {a, b};
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
    // cin >> t;
    while (t--)
    {
        cin >> N;
        cout << getNthFib(N, 1e9 + 7).first;
    }
    return 0;
}