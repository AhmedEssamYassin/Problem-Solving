#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <typename T>
inline T add64(const T &a, const T &b, const T &mod)
{
    ll res = (ll)a + b;
    if (res >= mod)
        res -= mod;
    return res;
}

template <typename T>
inline T sub64(const T &a, const T &b, const T &mod)
{
    ll res = (ll)a - b;
    if (res < 0)
        res += mod;
    if (res >= mod)
        res -= mod;
    return res;
}

template <typename T>
inline T mult64(const T &a, const T &b, const T &mod)
{
    return __int128_t(a) * b % mod;
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
