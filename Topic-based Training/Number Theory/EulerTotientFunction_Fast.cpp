#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define u128 __uint128_t
#define endl "\n"

// Use mult64() if the number fits in 64 bits, otherwise change all to mult128() (slower)
template <typename T>
inline T mult64(const T &a, const T &b, T mod)
{
    return (__int128_t)a * b % mod;
}
namespace bigNumber
{
    using u128 = __uint128_t;

    // ---------- 64-bit limb helpers (fast) ----------
    static inline void mult64(uint64_t a, uint64_t b, uint64_t &lo, uint64_t &hi)
    {
        __uint128_t p = (__uint128_t)a * b;
        lo = (uint64_t)p;
        hi = (uint64_t)(p >> 64);
    }
    static inline uint64_t add64(uint64_t a, uint64_t b, uint64_t &carry)
    {
        __uint128_t s = (__uint128_t)a + b + carry;
        carry = (uint64_t)(s >> 64);
        return (uint64_t)s;
    }
    // Fallback generic (slow): kept for setup (e.g., computing R^2 mod N once)
    u128 mult128(u128 a, u128 b, u128 mod)
    {
        u128 result = 0;
        for (a %= mod; b > 0; a <<= 1, b >>= 1)
        {
            a >= mod ? a -= mod : 0;
            if (b & 1)
                result += a, result >= mod ? result -= mod : 0;
        }
        return result;
    }
    // ---------- Montgomery (CIOS, 2x64-bit limbs) ----------
    // Compute n0' = -N^{-1} mod 2^64 (low limb only)
    static inline uint64_t inv64_2k(uint64_t n0)
    {
        uint64_t x = 1; // initial approx
        for (int i = 6; i > 0; i--)
            x = (__uint128_t)x * (2 - (__uint128_t)n0 * x);
        return x; // x ≡ n0^{-1}
    }

    inline pair<u128, u128> montModInv(u128 N)
    {
        return {0, (u128)(0 - inv64_2k(N))};
    }

    // Fast Montgomery multiplication: returns a*b*R^{-1} mod N, where R=2^128
    inline u128 montMult(u128 a, u128 b, u128 N, u128 N_n0prime)
    {
        uint64_t n0 = (uint64_t)N, n1 = (uint64_t)(N >> 64);
        uint64_t a0 = (uint64_t)a, a1 = (uint64_t)(a >> 64);
        uint64_t b0 = (uint64_t)b, b1 = (uint64_t)(b >> 64);
        uint64_t n0p = (uint64_t)N_n0prime;
        uint64_t t0 = 0, t1 = 0, t2 = 0;

        auto round_step = [&](uint64_t ai)
        {
            uint64_t lo, hi, carry;
            // t += ai * b
            mult64(ai, b0, lo, hi);
            carry = 0;
            t0 = add64(t0, lo, carry);
            t1 = add64(t1, hi, carry);
            t2 = add64(t2, 0, carry);
            mult64(ai, b1, lo, hi);
            carry = 0;
            t1 = add64(t1, lo, carry);
            t2 = add64(t2, hi, carry);
            // m = t0 * n0' (mod 2^64)
            uint64_t m = (uint64_t)((__uint128_t)t0 * n0p);
            // t += m * N
            mult64(m, n0, lo, hi);
            carry = 0;
            t0 = add64(t0, lo, carry);
            t1 = add64(t1, hi, carry);
            t2 = add64(t2, 0, carry);
            mult64(m, n1, lo, hi);
            carry = 0;
            t1 = add64(t1, lo, carry);
            t2 = add64(t2, hi, carry);
            // shift by one limb
            t0 = t1;
            t1 = t2;
            t2 = 0;
        };

        round_step(a0);
        round_step(a1);

        __uint128_t res = (((__uint128_t)t1 << 64) | t0);
        if (res >= N)
            res -= N;
        return (u128)res;
    }
}
using namespace bigNumber;

template <typename T>
inline T F(T x, T c, T mod, T inv) // Pollard-rho function
{
    x = montMult(x, x, mod, inv);
    x = x >= mod - c ? x - mod + c : x + c;
    return x;
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
    static u128 rng = 0xdeafbeefff;
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
    u128 inv = montModInv(N).second;
    T L = 1;
    while (gcdVal == 1)
    {
        Xt = X;
        for (size_t i = 1; i < L; i++)
            X = F(X, C, N, inv);

        int k = 0;
        while (k < L && gcdVal == 1)
        {
            Xs = X;
            for (size_t i = 0; i < m && i < L - k; i++)
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
T Phi(T N)
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

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
    int t = 1;
    u128 N;
    cin >> t;
    while (t--)
    {
        cin >> N;
        cout << Phi(N) << endl;
    }
    return 0;
}
