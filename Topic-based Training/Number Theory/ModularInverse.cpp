#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

inline ll mult64(const ll &a, const ll &b, const ll &mod)
{
    return __int128_t(a) * b % mod;
}

ll modPow(ll N, ll power, ll mod)
{
    if (N % mod == 0 || N == 0)
        return 0;
    if (N == 1 || power == 0)
        return 1;
    ll res{1};
    while (power)
    {
        if (power & 1)
            res = mult64(res, N, mod);
        N = mult64(N, N, mod);
        power >>= 1;
    }
    return res;
}

ll modInverse(ll N, ll mod) // N and mod must be co-primes, i.e., gcd (N, mod) = 1
{
    // If the mod is guaranteed to be prime, then the mod inverse is just:
    // return modPow(N, mod - 2, mod);
    return 1 < N ? mod - modInverse(mod % N, N) * mod / N : 1;
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
    }
    return 0;
}
