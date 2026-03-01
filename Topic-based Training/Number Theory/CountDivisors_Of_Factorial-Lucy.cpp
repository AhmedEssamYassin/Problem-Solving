#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <typename T>
inline T mult64(const T &a, const T &b, T mod)
{
    return (__int128_t)a * b % mod;
}

template <typename T>
T modPow(T N, T power, T mod)
{
    if (N % mod == 0 || N == 0)
        return 0;
    if (N == 1 || power == 0)
        return 1;
    T res{1};
    while (power)
    {
        if (power & 1)
            res = mult64(res, N, mod);
        N = mult64(N, N, mod);
        power >>= 1;
    }
    return res;
}

// Lucy's algorithm, works well for few number of tests at large N
ll countDivisors(ll N, ll mod)
{
    if (N <= 1)
        return 1 % mod;
    ll sq = sqrt(N);
    vector<ll> piS(sq + 1), piL(sq + 1);
    for (ll i = 1; i <= sq; i++)
    {
        piS[i] = i - 1;
        piL[i] = N / i - 1;
    }
    for (ll p = 2; p <= sq; p++)
    {
        if (piS[p] == piS[p - 1])
            continue;
        ll pSq = p * p;
        ll piP1 = piS[p - 1];
        ll maxI = min(sq, N / pSq);
        ll threshold = sq / p;
        for (ll i = 1; i <= min(maxI, threshold); i++)
            piL[i] -= piL[i * p] - piP1;
        for (ll i = max(1LL, threshold + 1); i <= maxI; i++)
            piL[i] -= piS[N / (i * p)] - piP1;
        for (ll i = sq; i >= pSq; i--)
            piS[i] -= piS[i / p] - piP1;
    }
    auto getPi = [&](ll x)
    { return (x <= sq) ? piS[x] : piL[N / x]; };
    ll cnt = 1;
    for (ll p = 2; p <= sq; p++)
    {
        if (piS[p] == piS[p - 1])
            continue;
        ll v = 0;
        ll cur = N;
        while (cur /= p)
            v += cur;
        cnt = mult64(cnt, v + 1, mod);
    }
    for (ll Q = 1; Q <= sq; Q++)
    {
        ll R = N / Q;
        ll L = max((ll)sq, N / (Q + 1));
        if (R > L)
        {
            ll numPrimes = getPi(R) - getPi(L);
            if (numPrimes > 0)
                cnt = mult64(cnt, modPow(Q + 1, numPrimes, mod), mod);
        }
    }
    return cnt;
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
    ll N, M;
    cin >> t;
    while (t--)
    {
        cin >> N >> M;
        cout << countDivisors(N, M) << endl;
    }
    return 0;
}
