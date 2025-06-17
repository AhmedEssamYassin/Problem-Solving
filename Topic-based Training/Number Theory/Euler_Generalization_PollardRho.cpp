#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define u128 __uint128_t
#define endl "\n"

namespace bigNumber
{
    using u128 = __uint128_t;

    u128 mult128(u128 a, u128 b, u128 mod)
    {
        u128 result = 0;
        for (a %= mod; b > 0; a <<= 1, b >>= 1)
        {
            a >= mod ? a -= mod : 0;
            if (b & 1)
            {
                result += a, result >= mod ? result -= mod : 0;
            }
        }
        return result;
    }

    static u128 mult(u128 x, u128 y, u128 &high)
    {
        const size_t shift = sizeof(x) * 4;
        u128 a = x >> shift, b = (x << shift) >> shift;
        u128 c = y >> shift, d = (y << shift) >> shift;
        u128 ac = a * c;
        u128 ad = a * d;
        u128 bc = b * c;
        u128 bd = b * d;
        u128 carry = ((ad << shift) >> shift) + ((bc << shift) >> shift) + (bd >> shift);
        high = ac + (ad >> shift) + (bc >> shift) + (carry >> shift);
        return (ad << shift) + (bc << shift) + bd;
    }
    u128 montMult(u128 a, u128 b, u128 N, u128 N_neg_inv)
    {
        u128 Th, Tl, m, mNh, mNl;
        Tl = mult(a, b, Th);
        m = Tl * N_neg_inv;
        mNl = mult(m, N, mNh);
        bool lc = Tl + mNl < Tl;
        u128 th = Th + mNh + lc;
        bool hc = (th < Th) || (th == Th && lc);

        if (hc > 0 || th >= N)
            th = th - N;

        return th;
    }

    inline pair<u128, u128> montModInv(u128 m)
    {
        const size_t shift = sizeof(m) * 8 - 1;
        u128 a = u128(1) << shift;
        u128 u = 1;
        u128 v = 0;

        while (a)
        {
            a >>= 1;
            if (u & 1)
            {
                u = ((u ^ m) >> 1) + (u & m);
                v = (v >> 1) + (u128(1) << shift);
            }
            else
            {
                u >>= 1;
                v >>= 1;
            }
        }

        return {u, v};
    }
}
using namespace bigNumber;

template <typename T>
inline T add64(const T &a, const T &b, const T &mod)
{
    T res = (a + b);
    if (res >= mod)
        res -= mod;
    return res;
}

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
            res = mult128(res, N, mod);
        N = mult128(N, N, mod);
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
        X = mult128(X, X, N);
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
void Factor(T N, map<T, T> &primeFactors)
{
    if (N == 1)
        return;

    if (!isPrime(N))
    {
        T Y = Pollard_Brent(N);
        Factor(Y, primeFactors);
        Factor(N / Y, primeFactors);
    }
    else
    {
        primeFactors[N]++;
        return;
    }
}

template <typename T>
void primeFactorize(T N, map<T, T> &primeFactors) // Use a vector if generating ALL factors
{
    Factor(N, primeFactors);
    // Sort if you use a vector and order matters!
    // sort(primeFactors.begin(), primeFactors.end());
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
    u128 N, k;
    // cin >> t;
    while (t--)
    {
        cin >> N >> k;
        if (N == 0)
            return cout << add64(2 % k, u128(1), k), 0;
        u128 base = 2;
        u128 g = __gcd(base, k);
        if (g == 1) // Euler Generalization
        {
            u128 power = modPow(u128(2), N, Phi(k));
            cout << add64(modPow(base, power, k), u128(1), k);
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
            u128 k_dash = k, m = 0, g_power_m = 1;
            while (k_dash % g == 0)
                k_dash /= g, g_power_m *= g, m++;

            if (N >= m) // The first term vanishes
            {
                u128 phi_k_dash = Phi(k_dash);
                u128 res = modPow(base, modPow(u128(2), N, phi_k_dash), k);
                res = mult128(res, g_power_m, k);
                res = mult128(res, modInverse(g_power_m, k_dash), k);
                cout << add64(res, u128(1), k);
            }
            else
            {
                u128 phi_k_dash = Phi(k_dash);
                u128 Term1 = modPow(base, (u128(1) << N), g_power_m);
                Term1 = mult128(Term1, k_dash, g_power_m);
                Term1 = mult128(Term1, modInverse(k_dash, g_power_m), g_power_m);
                u128 Term2 = modPow(base, modPow(u128(2), N, phi_k_dash), k);
                Term2 = mult128(Term2, g_power_m, k);
                Term2 = mult128(Term2, modInverse(g_power_m, k_dash), k);
                cout << add64(Term1, add64(Term2, u128(1), k), k);
            }
        }
    }
    return 0;
}