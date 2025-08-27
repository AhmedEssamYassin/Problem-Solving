#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <typename T>
inline T mult64(const T &a, const T &b, const T &mod)
{
    return __int128_t(a) * b % mod;
}

template <typename T>
inline T add64(const T &a, const T &b, const T &mod)
{
    __int128_t res = __int128_t(a) + b;
    if (res >= mod)
        res -= mod;
    return res;
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

template <typename T>
bool isPrime(T N)
{
    if (N < 2 || N % 6 % 4 != 1)
        return (N | 1) == 3;
    T d = N - 1;
    int s{};
    while (~s & 1)
        d >>= 1, ++s;
    for (const T &a : {2, 3, 5, 7, 11, 17, 19, 325, 9375, 28178, 450775, 9780504, 1795265022})
    {
        T p = modPow(a % N, d, N), i = s;
        while (p != 1 && p != N - 1 && a % N && i--)
            p = mult64(p, p, N);
        if (p != N - 1 && i != s)
            return false;
    }
    return true;
}

template <typename T>
void primeFactorize(T N, map<T, T> &primeFactors) // Use a vector if generating ALL factors
{
    for (ll p = 2; p * p <= N; p++)
    {
        while (N % p == 0)
            primeFactors[p]++, N /= p;
    }
    if (N > 1)
        primeFactors[N]++;
}

template <typename T>
T binPow(T N, T power)
{
    T res = 1;
    while (power)
    {
        if (power & 1)
            res *= N;
        N *= N;
        power >>= 1;
    }
    return res;
}

// Euler Totient Function
template <typename T>
T phi(T N)
{
    if (isPrime(N))
        return (N - 1);
    map<T, T> pf;
    primeFactorize(N, pf);
    T ans = 1;
    for (auto &[p, exp] : pf) // O(log² N)
        ans *= (binPow(p, exp) - binPow(p, exp - 1));
    return ans;
}

ll normalize(__int128_t x, ll m)
{
    if (x < m)
        return x;
    return (m + x % m);
}

template <typename T>
T Exp(T N, T power, T mod)
{
    T res{1};
    while (power)
    {
        if (power & 1)
            res = normalize((__int128_t)res * N, mod);

        N = normalize((__int128_t)N * N, mod);
        power >>= 1;
    }
    return res;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
    // freopen("calc.in", "r", stdin);
    // freopen("calc.out", "w", stdout);
    int t = 1;
    ll N, k;
    // cin >> t;
    while (t--)
    {
        cin >> N >> k;
        if (N == 0)
            return cout << add64<ll>(2 % k, 1, k), 0;

        ll w[] = {0, 2, 2, N}; // 0 is padding
        function<ll(ll, ll, ll)> solve = [&](ll l, ll r, ll m) -> ll
        {
            if (l == r + 1 || m == 1)
                return 1;

            ll power = solve(l + 1, r, phi(m));
            ll res = Exp(w[l], power, m);
            return res; // Don't take mod here
        };
        cout << add64(solve(1, 3, k) % k, 1LL, k);
    }
    return 0;
}