#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

using i64 = int64_t;
using u32 = uint32_t;
using u64 = uint64_t;
using u128 = __uint128_t;
template <typename T>
T inverse(T a, T m)
{
    T u = 0, v = 1;
    while (a != 0)
    {
        T t = m / a;
        m -= t * a;
        swap(a, m);
        u -= t * v;
        swap(u, v);
    }
    assert(m == 1);
    return u;
}

template <typename T>
class Modular
{
public:
    using Type = typename decay<decltype(T::value)>::type;

    constexpr Modular() : value() {}
    template <typename U>
    Modular(const U &x)
    {
        value = normalize(x);
    }

    template <typename U>
    static Type normalize(const U &x)
    {
        Type v;
        if (-mod() <= x && x < mod())
            v = static_cast<Type>(x);
        else
            v = static_cast<Type>(x % mod());
        if (v < 0)
            v += mod();
        return v;
    }

    const Type &operator()() const { return value; }
    template <typename U>
    explicit operator U() const { return static_cast<U>(value); }
    constexpr static Type mod() { return T::value; }

    Modular &operator+=(const Modular &other)
    {
        value += other.value;
        value -= (value >= mod()) * mod();
        return *this;
    }
    Modular &operator-=(const Modular &other)
    {
        value -= other.value;
        value += (value < 0) * mod();
        return *this;
    }
    template <typename U>
    Modular &operator+=(const U &other) { return *this += Modular(other); }
    template <typename U>
    Modular &operator-=(const U &other) { return *this -= Modular(other); }
    Modular &operator++() { return *this += 1; }
    Modular &operator--() { return *this -= 1; }
    Modular operator++(int)
    {
        Modular result(*this);
        *this += 1;
        return result;
    }
    Modular operator--(int)
    {
        Modular result(*this);
        *this -= 1;
        return result;
    }
    Modular operator-() const { return Modular(-value); }

    template <typename U = T>
    typename enable_if<is_same<typename Modular<U>::Type, int>::value, Modular>::type &operator*=(const Modular &rhs)
    {
        value = normalize(static_cast<int64_t>(value) * static_cast<int64_t>(rhs.value));
        return *this;
    }
    template <typename U = T>
    typename enable_if<is_same<typename Modular<U>::Type, int64_t>::value, Modular>::type &operator*=(const Modular &rhs)
    {
        int64_t q = int64_t(static_cast<long double>(value) * rhs.value / mod());
        value = normalize(value * rhs.value - q * mod());
        return *this;
    }
    template <typename U = T>
    typename enable_if<!is_integral<typename Modular<U>::Type>::value, Modular>::type &operator*=(const Modular &rhs)
    {
        value = normalize(value * rhs.value);
        return *this;
    }

    Modular &operator/=(const Modular &other) { return *this *= Modular(inverse(other.value, mod())); }

    friend const Type &abs(const Modular &x) { return x.value; }

    template <typename U>
    friend bool operator==(const Modular<U> &lhs, const Modular<U> &rhs);

    template <typename U>
    friend bool operator<(const Modular<U> &lhs, const Modular<U> &rhs);

    template <typename V, typename U>
    friend V &operator>>(V &stream, Modular<U> &number);

private:
    Type value;
};

template <typename T>
bool operator==(const Modular<T> &lhs, const Modular<T> &rhs) { return lhs.value == rhs.value; }
template <typename T, typename U>
bool operator==(const Modular<T> &lhs, U rhs) { return lhs == Modular<T>(rhs); }
template <typename T, typename U>
bool operator==(U lhs, const Modular<T> &rhs) { return Modular<T>(lhs) == rhs; }

template <typename T>
bool operator!=(const Modular<T> &lhs, const Modular<T> &rhs) { return !(lhs == rhs); }
template <typename T, typename U>
bool operator!=(const Modular<T> &lhs, U rhs) { return !(lhs == rhs); }
template <typename T, typename U>
bool operator!=(U lhs, const Modular<T> &rhs) { return !(lhs == rhs); }

template <typename T>
bool operator<(const Modular<T> &lhs, const Modular<T> &rhs) { return lhs.value < rhs.value; }

template <typename T>
Modular<T> operator+(const Modular<T> &lhs, const Modular<T> &rhs) { return Modular<T>(lhs) += rhs; }
template <typename T, typename U>
Modular<T> operator+(const Modular<T> &lhs, U rhs) { return Modular<T>(lhs) += rhs; }
template <typename T, typename U>
Modular<T> operator+(U lhs, const Modular<T> &rhs) { return Modular<T>(lhs) += rhs; }

template <typename T>
Modular<T> operator-(const Modular<T> &lhs, const Modular<T> &rhs) { return Modular<T>(lhs) -= rhs; }
template <typename T, typename U>
Modular<T> operator-(const Modular<T> &lhs, U rhs) { return Modular<T>(lhs) -= rhs; }
template <typename T, typename U>
Modular<T> operator-(U lhs, const Modular<T> &rhs) { return Modular<T>(lhs) -= rhs; }

template <typename T>
Modular<T> operator*(const Modular<T> &lhs, const Modular<T> &rhs) { return Modular<T>(lhs) *= rhs; }
template <typename T, typename U>
Modular<T> operator*(const Modular<T> &lhs, U rhs) { return Modular<T>(lhs) *= rhs; }
template <typename T, typename U>
Modular<T> operator*(U lhs, const Modular<T> &rhs) { return Modular<T>(lhs) *= rhs; }

template <typename T>
Modular<T> operator/(const Modular<T> &lhs, const Modular<T> &rhs) { return Modular<T>(lhs) /= rhs; }
template <typename T, typename U>
Modular<T> operator/(const Modular<T> &lhs, U rhs) { return Modular<T>(lhs) /= rhs; }
template <typename T, typename U>
Modular<T> operator/(U lhs, const Modular<T> &rhs) { return Modular<T>(lhs) /= rhs; }

template <typename T, typename U>
Modular<T> power(const Modular<T> &a, const U &b)
{
    assert(b >= 0);
    Modular<T> x = a, res = 1;
    U p = b;
    while (p > 0)
    {
        if (p & 1)
            res *= x;
        x *= x;
        p >>= 1;
    }
    return res;
}

template <typename T>
bool IsZero(const Modular<T> &number)
{
    return number() == 0;
}

template <typename T>
string to_string(const Modular<T> &number)
{
    return std::to_string(number());
}

// U == std::ostream? but done this way because of fastoutput
template <typename U, typename T>
U &operator<<(U &stream, const Modular<T> &number)
{
    return stream << number();
}

// U == std::istream? but done this way because of fastinput
template <typename U, typename T>
U &operator>>(U &stream, Modular<T> &number)
{
    typename common_type<typename Modular<T>::Type, int64_t>::type x;
    stream >> x;
    number.value = Modular<T>::normalize(x);
    return stream;
}

// using ModType = int; // Important for is_same<> to work

// struct VarMod
// {
//     static ModType value;
// };
// ModType VarMod::value;
// ModType &mod = VarMod::value;
// using Mint = Modular<VarMod>;

constexpr int mod = 998244353;
using Mint = Modular<std::integral_constant<decay<decltype(mod)>::type, mod>>;
Mint operator""_m(unsigned long long literal)
{
    return Mint(literal);
}

vector<int> primes, mu;
void linearSieveOfEratosthenes(ll N)
{
    mu.assign(N + 1, 0);
    bitset<2000001> isPrime;
    isPrime.set();
    isPrime[0] = isPrime[1] = 0;
    mu[1] = 1;
    for (ll i = 2; i <= N; i++)
    {
        if (isPrime[i])
        {
            primes.push_back(i);
            mu[i] = -1;
        }
        for (ll j = 0; j < (ll)primes.size() && i * primes[j] <= N; j++)
        {
            ll multiple = i * primes[j];
            isPrime[multiple] = 0;
            if (i % primes[j] == 0)
            {
                mu[multiple] = 0;
                break;
            }
            mu[multiple] = -mu[i];
        }
    }
}

static int autoCall = (linearSieveOfEratosthenes(2e6), 0);

/**
 * Fast Mobius Transform for GCD Convolution (Prime SOS DP)
 * Time: O(N log log N), Space: O(1) in-place
 */
template <typename T>
void mobiusTransform(vector<T> &polyVec, bool inverse = false)
{
    int vecSize = polyVec.size();
    if (!inverse)
    {
        // Fast Zeta Transform (Sum over multiples)
        for (const int &p : primes)
        {
            if (p >= vecSize)
                break;
            // Iterate downwards to accumulate multiples correctly
            for (int i = (vecSize - 1) / p; i >= 1; i--)
                polyVec[i] += polyVec[i * p];
        }
    }
    else
    {
        // Fast Mobius Transform (Exact Inversion)
        for (const int &p : primes)
        {
            if (p >= vecSize)
                break;
            // Iterate upwards to subtract multiples sequentially
            for (int i = 1; i * p < vecSize; i++)
                polyVec[i] -= polyVec[i * p];
        }
    }
}

/**
 * GCD Convolution using Mobius Transform
 * Result[i] = sum(a[j] * b[k]) for all gcd(j, k) = i
 */
template <typename T>
vector<T> convoluteGCD(const vector<T> &polyA, const vector<T> &polyB)
{
    if (polyA.empty() || polyB.empty())
        return {};

    int maxSize = max(polyA.size(), polyB.size());

    vector<T> transA(polyA.begin(), polyA.end()), transB(polyB.begin(), polyB.end());
    transA.resize(maxSize);
    transB.resize(maxSize);

    mobiusTransform(transA);
    mobiusTransform(transB);

    for (int idx = 0; idx < maxSize; idx++)
        transA[idx] = (transA[idx] * transB[idx]);

    mobiusTransform(transA, true);

    return transA;
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
    // cin >> t;
    while (t--)
    {
        cin >> N;
        // GCD works with 1-based Polynomials (Since gcd = 0 is undefined)
        vector<Mint> Poly1(N + 1), Poly2(N + 1);
        for (int i{1}; i <= N; i++)
            cin >> Poly1[i];
        for (int i{1}; i <= N; i++)
            cin >> Poly2[i];
        vector<Mint> res = convoluteGCD(Poly1, Poly2);
        for (int i{1}; i < res.size(); i++)
            cout << res[i] << " ";
    }

    return 0;
}