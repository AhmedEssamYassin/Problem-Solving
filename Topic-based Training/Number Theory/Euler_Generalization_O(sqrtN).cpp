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
    T res = a + b;
    if (res >= mod)
        res -= mod;
    return res;
}

template <typename T>
T modPow(T N, T power, T mod)
{
    if (mod == 1)
        return 0; // N^k mod 1 = 0 for any N, k
    if (power == 0)
        return 1 % mod; // N^0 = 1 (if N ≠ 0), so handle this first
    if (N == 0 || N % mod == 0)
        return 0; // 0^k = 0 (k > 0), and (m·x)^k ≡ 0 mod m (k > 0)
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

template <typename T>
T modInverse(T N, T mod) // N and mod must be co-primes, i.e., gcd (N, mod) = 1
{
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
        ll base = 2;
        ll g = gcd(base, k);
        if (g == 1) // Euler Generalization
        {
            ll power = modPow<ll>(2, N, Phi(k));
            cout << add64<ll>(modPow(base, power, k), 1, k);
        }
        else
        {
            /*
            Computes x ≡ (a^b) mod k using Chinese Remainder Theorem (CRT) decomposition
            when k = g^m * k' (where g = gcd(a, k))

            The solution combines two congruences using CRT:
            1. x ≡ 0 mod g^m           (since a^b is divisible by g^m when b >= m)
            2. x ≡ a^(b mod ϕ(k')) mod k' (using Euler's theorem)

            The CRT solution is:
            x = [ (0 × k' × inv(k', g^m))
                + (a^(b mod ϕ(k')) × g^m × inv(g^m, k')) ] mod k

            Where:
            - inv(a,b) = modular inverse of a modulo b
            - ϕ(k')    = Euler's Totient function for k'
            - g^m      = maximal power of g dividing k

            Note: The first term vanishes (0 × ...), so we simplify to:
            x = [a^(b mod ϕ(k')) × g^m × inv(g^m, k')] mod k
            */
            ll k_dash = k, m = 0, g_power_m = 1;
            while (k_dash % g == 0)
                k_dash /= g, g_power_m *= g, m++;

            if (N >= m) // The first term vanishes
            {
                ll phi_k_dash = Phi(k_dash);
                ll res = modPow<ll>(base, modPow<ll>(2, N, phi_k_dash), k);
                res = mult64(res, g_power_m, k);
                res = mult64(res, modInverse(g_power_m, k_dash), k);
                cout << add64<ll>(res, 1, k);
            }
            else
            {
                ll phi_k_dash = Phi(k_dash);
                ll Term1 = modPow<ll>(base, (1LL << N), g_power_m);
                Term1 = mult64(Term1, k_dash, g_power_m);
                Term1 = mult64(Term1, modInverse(k_dash, g_power_m), g_power_m);
                ll Term2 = modPow<ll>(base, modPow<ll>(2, N, phi_k_dash), k);
                Term2 = mult64(Term2, g_power_m, k);
                Term2 = mult64(Term2, modInverse(g_power_m, k_dash), k);
                cout << add64<ll>(Term1, add64<ll>(Term2, 1, k), k);
            }
        }
    }
    return 0;
}