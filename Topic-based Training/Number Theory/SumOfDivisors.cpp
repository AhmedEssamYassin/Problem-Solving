#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define u128 __uint128_t
#define endl "\n"

// Use type u64
namespace Montgomery64
{
    using u64 = uint64_t;
    using u128 = __uint128_t;

    inline u64 mult64(u64 a, u64 b, u64 mod) { return (u128)a * b % mod; }

    static inline u64 inv64_2k(u64 n0)
    {
        u64 x = 1;
        for (int i = 6; i > 0; i--)
            x *= 2 - n0 * x;
        return x;
    }

    inline u64 montModInv(u64 n) { return 0 - inv64_2k(n); }
    inline u64 montMult(u64 a, u64 b, u64 n, u64 n0Prime)
    {
        u128 t = (u128)a * b;
        u64 m = (u64)t * n0Prime;
        u128 res = (t >> 64) + (((u128)m * n) >> 64) + ((u64)t != 0);
        if (res >= n)
            res -= n;
        return res;
    }
}
using namespace Montgomery64;

template <typename T>
inline T absVal(T N) { return N < 0 ? -N : N; }

template <typename T>
inline T F(T x, T c, T mod, T inv) // Pollard-Rho function
{
    x = montMult(x, x, mod, inv);
    x = x >= mod - c ? x - mod + c : x + c;
    return x;
}

template <typename T>
T pollardBrent(T N)
{
    if (!(N & 1))
        return 2;

    // Random Number Linear Congruential Generator MMIX from D.E. Knuth
    static u128 rng = 0xdeafbeefff;
    uint64_t a = rng * 6364136223846793005ULL + 1442695040888963407ULL;
    uint64_t b = a * 6364136223846793005ULL + 1442695040888963407ULL;
    rng = (a + b) ^ (a * b);

    T X0 = 1 + a % (N - 1);
    T C = 1 + b % (N - 1);
    T X = X0; // X1
    T gcdVal = 1;
    T q = 1;
    T Xs, Xt;
    T m = 128;
    u64 inv = montModInv(N);
    T L = 1;
    while (gcdVal == 1)
    {
        Xt = X;
        for (size_t i = 1; i < L; i++)
            X = F(X, C, N, inv);

        uint64_t k = 0;
        while (k < L && gcdVal == 1)
        {
            Xs = X;
            for (size_t i = 0; i < m && i + k < L; i++)
            {
                X = F(X, C, N, inv);
                q = montMult(q, Xt > X ? Xt - X : X - Xt, N, inv);
            }
            gcdVal = __gcd(q, N);
            k += m;
        }
        L += L;
    }
    if (gcdVal == N) // Failure
    {
        do
        {
            Xs = F(Xs, C, N, inv);
            gcdVal = __gcd(Xs > Xt ? Xs - Xt : Xt - Xs, N);
        } while (gcdVal == 1);
    }
    return gcdVal;
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
void primeFactorize(T N, vector<T> &primeFactors)
{
    if (N == 1)
        return;

    if (isPrime(N))
    {
        primeFactors.push_back(N);
        return;
    }
    T Y = pollardBrent(N);
    primeFactorize(Y, primeFactors);
    primeFactorize(N / Y, primeFactors);
}

// σ function (Sum of divisors)
template <typename T>
T sumDivisors(T N)
{
    if (N == 1)
        return 1;

    u64 sigma{1};
    vector<u64> pf;
    primeFactorize(N, pf);
    sort(pf.begin(), pf.end());
    for (size_t i = 0; i < pf.size();)
    {
        ll p = pf[i];
        ll sum{1}, term{1};
        while (i < pf.size() && pf[i] == p)
        {
            term *= p;
            sum += term;
            i++;
        }
        sigma *= sum;
    }

    return sigma;
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
    u64 N;
    cin >> t;
    while (t--)
    {
        cin >> N;
        cout << sumDivisors(N) - N << endl;
    }
    return 0;
}
