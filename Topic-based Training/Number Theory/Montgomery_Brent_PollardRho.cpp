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

// As a rule of thumb, if you inevitably generate all factors, use sqrt(N) factorization.
template <typename T>
void getAllFactors(T N, vector<T> &factors)
{
    vector<T> primeFactors;
    primeFactorize(N, primeFactors);
    factors.push_back(1);
    for (const T &p : primeFactors)
    {
        ll len = factors.size();
        for (ll i{}; i < len; i++)
            factors.push_back(p * factors[i]);

        set<ll> distinctDivisors(factors.begin(), factors.end());
        factors.clear();
        for (const ll &it : distinctDivisors)
            factors.push_back(it);
    }
}

template <typename T>
T countDivisors(T N)
{
    map<T, T> primeFactors;
    primeFactorize(N, primeFactors);
    T ans{1};
    for (const auto &[p, exp] : primeFactors)
        ans *= (exp + 1);
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
        cout << countDivisors(N) << endl;
    }
    return 0;
}
