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
bool checkComposite(T N, T a, T d, int s)
{
    T X = modPow(a, d, N);
    if (X == 1 || X == N - 1)
        return false; // Not composite

    for (int r = 1; r < s; r++)
    {
        X = mult64(X, X, N);
        if (X == 1 || X == N - 1)
            return false; // Not composite
    }
    return true; // Composite
}

template <typename T>
bool Miller_Rabin(T N, int K = 5) // k is the number of trials (bases). If k increases the accuracy increases
{
    T d = N - 1;
    int s{};
    while (~s & 1)
        d >>= 1, ++s;

    for (const T &a : {11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47})
    {
        if (N == a)
            return true;
        if (checkComposite(N, a, d, s))
            return false;
    }
    return true;
}

template <typename T>
bool isPrime(T N)
{
    if (N < 2)
        return false;

    if (N <= 3)
        return true;
    if (N == 5 || N == 7)
        return true;

    if (!(N & 1) || N % 3 == 0 || N % 5 == 0 || N % 7 == 0)
        return false;

    return Miller_Rabin(N);
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
T Phi(T N)
{
    if (isPrime(N))
        return (N - 1);
    map<T, T> pf;
    primeFactorize(N, pf);
    T ans = 1;
    for (auto &[p, exp] : pf)
        ans *= (binPow(p, exp) - binPow(p, exp - 1));
    return ans;
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
        cin >> N;
        cout << Phi(N) << endl;
    }
    return 0;
}
