#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll mod = 1e9 + 7;
inline ll mult64(const ll &a, const ll &b)
{
    return __int128_t(a) * b % mod;
}

inline ll add64(const ll &a, const ll &b)
{
    ll res = a + b;
    if (res >= mod)
        res -= mod;
    return res;
}

inline ll sub64(const ll &a, const ll &b)
{
    ll res = a - b;
    if (res < 0)
        res += mod;
    if (res >= mod)
        res -= mod;
    return res;
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
            res = mult64(res, N);
        N = mult64(N, N);
        power >>= 1;
    }
    return res;
}

bool isPrime(ll N)
{
    if (N < 2)
        return false;
    if (N < 4)
        return true;
    if (N % 2 == 0 || N % 3 == 0)
        return false;
    int k = 5;
    for (int i = 0; i < k; i++)
    {
        ll a = 2 + rand() % (N - 3);
        if (modPow(a, N - 1, N) != 1)
            return false;
    }
    return true;
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
