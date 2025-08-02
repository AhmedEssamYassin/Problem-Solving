#include <bits/stdc++.h>
using namespace std;
#define i128 __int128_t
#define ll long long int
#define endl "\n"

template <typename T>
inline T mult64(const T &a, const T &b, T mod)
{
    return (__int128_t)a * b % mod;
}

template <typename T>
inline T mult128(T a, T b, T mod)
{
    T result = 0; // Initialize result

    for (a %= mod; b > 0; a <<= 1, b >>= 1)
    {
        if (a >= mod)
            a -= mod;
        if (b & 1) // b is ODD
        {
            result += a;
            if (result >= mod)
                result -= mod;
        }
    }
    return result;
}

template <typename T>
inline T F(T x, T c, T mod) // Pollard-rho function
{
    return (mult128(x, x, mod) + c) % mod;
}

template <typename T>
inline T __abs(T N)
{
    if (N < 0)
        return -N;

    return N;
}

template <typename T>
T rho(T N, T x0 = 2, T c = 1)
{
    if (N % 2 == 0)
        return 2;
    T x = x0;
    T y = x0;
    T g = 1;
    while (g == 1)
    {
        x = F(x, c, N);
        y = F(y, c, N);
        y = F(y, c, N);
        g = __gcd(__abs(x - y), N); // Must use __gcd() with __int128_t
    }
    return g == N ? rho(N, x0 + 1, c + 1) : g;
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
            p = mult128(p, p, N);
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
        T Y = rho(N);
        primeFactorize(Y, primeFactors);
        primeFactorize(N / Y, primeFactors);
    }
    else
    {
        primeFactors[N]++;
        return;
    }
}

// As a rule of thumb, if you inevitably generate all factors, use sqrt(N) factorization.
template <typename T>
void getAllFactors(T N, vector<T> &factors)
{
    factors = {1};
    map<T, T> freq;
    primeFactorize(N, freq);

    for (auto &[p, cnt] : freq)
    {
        vector<T> temp;
        T pw = 1;
        for (int i = 0; i <= cnt; i++, pw *= p)
        {
            for (const T &f : factors)
                temp.push_back(f * pw);
        }
        factors.swap(temp);
    }
    sort(factors.begin(), factors.end());
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
    i128 N;
    // cin >> t;
    while (t--)
    {
        cin >> N;
        cout << countDivisors(N);
    }
    return 0;
}
