#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const auto mod = 1000000007; // 1e9 + 7

template <typename T>
inline T add64(const T &a, const T &b)
{
    ll res = (ll)a + b;
    if (res >= mod)
        res -= mod;
    return res;
}

template <typename T>
inline T sub64(const T &a, const T &b)
{
    ll res = (ll)a - b;
    if (res < 0)
        res += mod;
    if (res >= mod)
        res -= mod;
    return res;
}

template <typename T>
inline T mult32(const T &a, const T &b)
{
    return (long long)(a)*b % mod;
}

template <typename T>
inline T mult64(const T &a, const T &b)
{
    return __int128_t(a) * b % mod;
}

template <typename T>
using MulFuncT = T (*)(const T &, const T &);

template <typename T>
MulFuncT<T> getModMultFunc()
{
    return mod < (1LL << 31) ? mult32<T> : mult64<T>;
}
#define modMult(a, b) getModMultFunc<std::decay_t<decltype(a)>>()(a, b)

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
