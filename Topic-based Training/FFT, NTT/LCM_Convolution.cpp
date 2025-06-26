#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll mod = 998244353;
#define double_size_t std::conditional_t<(mod > (1LL << 31)), __int128_t, long long>

inline ll add64(const ll &a, const ll &b)
{
    double_size_t res = double_size_t(a) + b;
    if (res >= mod)
        res -= mod;
    return res;
}

inline ll sub64(const ll &a, const ll &b)
{
    double_size_t res = double_size_t(a) - b;
    if (res < 0)
        res += mod;
    if (res >= mod)
        res -= mod;
    return res;
}

inline ll mult64(const ll &a, const ll &b)
{
    return double_size_t(a) * b % mod;
}

ll modPow(ll N, ll power)
{
    if (N % mod == 0 || N == 0)
        return 0;
    if (N == 1 || power == 0)
        return 1;

    ll res{1};
    while (power)
    {
        if (power & 1)
            res = mult64(res, N);
        N = mult64(N, N);
        power >>= 1;
    }
    return res;
}

ll modInverse(ll a)
{
    return modPow(a, mod - 2);
}

vector<ll> Prime, LPF;
bitset<1000001> isPrime;

void linearSieveOfEratosthenes(int N)
{
    isPrime.set(); // Initially Assuming all numbers to be primes
    LPF.resize(N + 1);
    isPrime[0] = isPrime[1] = 0; // 0 and 1 are NOT primes
    for (int i{2}; i <= N; i++)
    {
        if (isPrime[i])
        {
            Prime.push_back(i);
            LPF[i] = i; // The least prime factor of a prime number is itself
        }
        for (int j{}; j < (int)Prime.size() and i * Prime[j] <= N and Prime[j] <= LPF[i]; j++)
        {
            isPrime[i * Prime[j]] = 0; // Crossing out all the multiples of prime numbers
            LPF[i * Prime[j]] = Prime[j];
        }
    }
}
static int autoCall = (linearSieveOfEratosthenes(1000000), 0);

/**
 * Subset Sum over Divisors Transform for LCM Convolution
 * This is the correct transform for LCM convolution
 */
template <typename T>
void sosOverDivisors(vector<T> &f, bool inverse = false)
{
    int n = f.size();

    if (!inverse)
    {
        // Forward SOS over divisors
        for (const ll &p : Prime)
        {
            for (ll i = 1; i * p < n; i++)
                f[i * p] = add64(f[i * p], f[i]);
        }
    }
    else
    {
        // Inverse SOS over divisors
        for (const ll &p : Prime)
        {
            for (ll i = (n - 1) / p; i >= 1; i--)
                f[i * p] = sub64(f[i * p], f[i]);
        }
    }
}

/**
 * LCM Convolution
 * Result[i] = sum(a[j] * b[k]) for all lcm(j,k) = i
 */
template <typename T>
vector<T> convoluteLcm(vector<T> a, vector<T> b)
{
    if (a.empty() || b.empty())
        return {};

    int n = max(a.size(), b.size());
    a.resize(n);
    b.resize(n);

    // Apply SOS over divisors transform
    sosOverDivisors(a, false);
    sosOverDivisors(b, false);

    // Point-wise multiplication
    for (int i = 0; i < n; i++)
        a[i] = mult64(a[i], b[i]);

    // Apply inverse SOS over divisors transform
    sosOverDivisors(a, true);

    return a;
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
    // cin >> t;
    while (t--)
    {
        cin >> N;
        // LCM works with 1-based Polynomials (Since lcm = 0 is undefined)
        vector<ll> Poly1(N + 1), Poly2(N + 1);
        for (int i{1}; i <= N; i++)
            cin >> Poly1[i];
        for (int i{1}; i <= N; i++)
            cin >> Poly2[i];
        vector<ll> res = convoluteLcm(Poly1, Poly2);
        for (int i{1}; i < res.size(); i++)
            cout << res[i] << " ";
    }

    return 0;
}