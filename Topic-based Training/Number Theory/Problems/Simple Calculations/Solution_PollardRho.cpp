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
inline T __abs(T N)
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
                q = mult64(q, __abs(Xt - X), N);
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
            gcdVal = __gcd(__abs(Xs - Xt), N);
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
    if (N < 2 || N % 6 % 4 != 1)
        return (N | 1) == 3;
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

// GCC's implementation for I/O of 128-bit integers
using int128 = signed __int128;
using uint128 = unsigned __int128;

namespace int128_io
{

    inline auto char_to_digit(int chr)
    {
        return static_cast<int>(isalpha(chr) ? 10 + tolower(chr) - 'a' : chr - '0');
    }

    inline auto digit_to_char(int digit)
    {
        return static_cast<char>(digit > 9 ? 'a' + digit - 10 : '0' + digit);
    }

    template <class integer>
    inline auto to_int(const std::string &str, size_t *idx = nullptr, int base = 10)
    {
        size_t i = idx != nullptr ? *idx : 0;
        const auto n = str.size();
        const auto neg = str[i] == '-';
        integer num = 0;
        if (neg)
            ++i;
        while (i < n)
            num *= base, num += char_to_digit(str[i++]);
        if (idx != nullptr)
            *idx = i;
        return neg ? -num : num;
    }

    template <class integer>
    inline auto to_string(integer num, int base = 10)
    {
        const auto neg = num < 0;
        std::string str;
        if (neg)
            num = -num;
        do
            str += digit_to_char(num % base), num /= base;
        while (num > 0);
        if (neg)
            str += '-';
        std::reverse(str.begin(), str.end());
        return str;
    }

    inline auto next_str(std::istream &stream)
    {
        std::string str;
        stream >> str;
        return str;
    }

    template <class integer>
    inline auto &read(std::istream &stream, integer &num)
    {
        num = to_int<integer>(next_str(stream));
        return stream;
    }

    template <class integer>
    inline auto &write(std::ostream &stream, integer num) { return stream << to_string(num); }
}

inline auto &operator>>(istream &stream, int128 &num) { return int128_io::read(stream, num); }
inline auto &operator<<(ostream &stream, int128 num) { return int128_io::write(stream, num); }
inline auto &operator>>(istream &stream, uint128 &num) { return int128_io::read(stream, num); }
inline auto &operator<<(ostream &stream, uint128 num) { return int128_io::write(stream, num); }

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