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
    constexpr uint64_t MASK = 0x28208A20A08A28ACULL;
    constexpr uint32_t WHEEL30 = 0x208A2882;
    if (N < 64)
        return (MASK >> N) & 1;
    if (!((WHEEL30 >> (uint32_t)(N % 30)) & 1))
        return false;
    T d = N - 1;
    int s{};
    while (!(d & 1))
        d >>= 1, ++s;
    for (const T &a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022})
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
void primeFactorize(T N, map<T, T> &primeFactors)
{
    for (ll p = 2; p * p <= N; p++)
    {
        while (N % p == 0)
            primeFactors[p]++, N /= p;
    }
    if (N > 1)
        primeFactors[N]++;
}

// Euler Totient Function
template <typename T>
T Phi(T N)
{
    if (isPrime(N))
        return (N - 1);
    map<T, T> pf;
    primeFactorize(N, pf);
    T ans = N;
    for (auto &[p, exp] : pf)
        ans -= ans / p;
    return ans;
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
    ll N;
    cin >> t;
    while (t--)
    {
        cin >> N;
        cout << Phi(N) << endl;
    }
    return 0;
}
