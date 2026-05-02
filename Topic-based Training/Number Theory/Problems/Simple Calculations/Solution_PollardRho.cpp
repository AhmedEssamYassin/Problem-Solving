#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define i128 __int128_t
#define endl "\n"

template <typename T>
inline T add64(const T &a, const T &b, const T &mod)
{
    T res = (a + b);
    if (res >= mod)
        res -= mod;
    return res;
}

template <typename T>
inline T mult64(const T &a, const T &b, T mod)
{
    return (__int128_t)a * b % mod;
}

template <typename T>
inline T F(T x, T c, T mod) // Pollard-rho function
{
    return (mult64(x, x, mod) + c) % mod;
}

template <typename T>
inline T absVal(T N)
{
    if (N < 0)
        return -N;

    return N;
}

template <typename T>
T Pollard_Brent(T N)
{
    if (!(N & 1))
        return 2;

    // Random Number Linear Congruential Generator MMIX from D.E. Knuth
    static i128 rng = 0xdeafbeefff;
    uint64_t a = rng * 6364136223846793005ull + 1442695040888963407ull;
    uint64_t b = a * 6364136223846793005ull + 1442695040888963407ull;
    rng = (a + b) ^ (a * b);

    T X0 = 1 + a % (N - 1);
    T C = 1 + b % (N - 1);
    T X = X0; // X1
    T gcdVal = 1;
    T q = 1;
    T Xs, Xt;
    T m = 128;
    T L = 1;
    while (gcdVal == 1)
    {
        Xt = X;
        for (size_t i = 1; i < L; i++)
            X = F(X, C, N);

        int k = 0;
        while (k < L && gcdVal == 1)
        {
            Xs = X;
            for (size_t i = 0; i < m && i < L - k; i++)
            {
                X = F(X, C, N);
                q = mult64(q, absVal(Xt - X), N);
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
            Xs = F(Xs, C, N);
            gcdVal = __gcd(absVal(Xs - Xt), N);
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
void primeFactorize(T N, map<T, T> &primeFactors)
{
    if (N == 1)
        return;

    if (!isPrime(N))
    {
        T Y = Pollard_Brent(N);
        primeFactorize(Y, primeFactors);
        primeFactorize(N / Y, primeFactors);
    }
    else
    {
        primeFactors[N]++;
        return;
    }
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

// Fast I/O for 128-bit integers
using int128 = signed __int128;
using uint128 = unsigned __int128;
ostream &operator<<(ostream &stream, int128 n)
{
    if (__builtin_expect(n == 0, 0))
        return stream.put('0');
    char buf[41];
    char *end = buf + 41;
    char *p = end;
    bool neg = (n < 0);
    uint128 u = neg ? -(uint128)n : (uint128)n;
    do
    {
        *--p = (char)('0' + (unsigned)(u % 10));
        u /= 10;
    } while (u);
    if (neg)
        *--p = '-';
    return stream.write(p, end - p);
}

ostream &operator<<(ostream &stream, uint128 n)
{
    if (__builtin_expect(n == 0, 0))
        return stream.put('0');
    char buf[40];
    char *end = buf + 40;
    char *p = end;
    do
    {
        *--p = (char)('0' + (unsigned)(n % 10));
        n /= 10;
    } while (n);
    return stream.write(p, end - p);
}

istream &operator>>(istream &stream, int128 &n)
{
    n = 0;
    char c;
    if (!(stream >> c))
        return stream;
    bool neg = (c == '-');
    if (neg && !stream.get(c))
        return stream;
    while (isdigit((unsigned char)c))
    {
        n = n * 10 + (c - '0');
        if (!stream.get(c))
            break;
    }
    if (stream)
        stream.putback(c);
    if (neg)
        n = -n;
    return stream;
}

istream &operator>>(istream &stream, uint128 &n)
{
    n = 0;
    char c;
    if (!(stream >> c))
        return stream;
    while (isdigit((unsigned char)c))
    {
        n = n * 10 + (c - '0');
        if (!stream.get(c))
            break;
    }
    if (stream)
        stream.putback(c);
    return stream;
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
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif
    freopen("calc.in", "r", stdin);
    freopen("calc.out", "w", stdout);
    int t = 1;
    ll N, k;
    // cin >> t;
    while (t--)
    {
        cin >> N >> k;
        if (N == 0)
            return cout << add64(2 % k, 1LL, k), 0;
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