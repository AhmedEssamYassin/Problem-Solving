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
ll getNthFib(ll N, ll mod)
{
    using P = pair<ll, ll>;
    P f = {0, 1}, res = {1, 0};

    while (N > 0)
    {
        auto &[a, b] = f;
        auto &[c, d] = res;

        if (N & 1)
        {
            ll ac = mult64(a, c, mod);
            ll bd = mult64(b, d, mod);
            ll bc = mult64(b, c, mod);
            ll a_plus_b = add64(a, b, mod);

            ll x = add64(ac, bd, mod);
            ll y = add64(bc, mult64(a_plus_b, d, mod), mod);
            res = {x, y};
        }

        // Optimize the doubling step: F(2k) = F(k) * (2 * F(k + 1) - F(k))
        ll two_b = add64(b, b, mod);
        ll two_b_minus_a = (two_b >= a) ? two_b - a : two_b + mod - a;

        ll x = mult64(a, two_b_minus_a, mod);
        ll y = add64(mult64(a, a, mod), mult64(b, b, mod), mod);
        f = {x, y};

        N >>= 1;
    }
    return res.second;
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
        cout << getNthFib(N, 1e9 + 7);
    }
    return 0;
}