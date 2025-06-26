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

ll modInverse(ll N, ll mod) // N and mod must be co-primes, i.e., gcd (N, mod) = 1
{
    // If the mod is guaranteed to be prime, then the mod inverse is just:
    return modPow(N, mod - 2);
    // return 1 < N ? mod - modInverse(mod % N, N) * mod / N : 1;
}

/**
 * Fast Walsh-Hadamard Transform - XOR Convolution Implementation
 * Transforms polynomial coefficients for XOR-based operations
 * Works by recursively splitting array and applying butterfly operations
 * Time: O(n log n), Space: O(1) auxiliary
 */
template <typename T>
void FWHT(vector<T> &P, bool inverse = false)
{
    int n = P.size();

    for (int len = 1; 2 * len <= n; len *= 2)
    {
        for (int i = 0; i < n; i += 2 * len)
        {
            for (int j = 0; j < len; j++)
            {
                T u = P[i + j];
                T v = P[i + j + len];

                if (!inverse)
                {
                    P[i + j] = add64(u, v);
                    P[i + j + len] = sub64(u, v);
                }
                else
                {
                    P[i + j] = add64(u, v);
                    P[i + j + len] = sub64(u, v);
                }
            }
        }
    }

    if (inverse)
    {
        ll inv_n = modInverse(n, mod);
        for (T &x : P)
            x = mult64(x, inv_n);
    }
}

/**
 * XOR Convolution using FWHT
 * Multiplies two polynomials where multiplication is XOR operation
 * Result[i] = sum(a[j] * b[k]) for all j^k = i
 * Essential for problems involving XOR operations on subsets
 */
template <typename T>
vector<T> convolute_xor(const vector<T> &a, const vector<T> &b)
{
    if (a.empty() || b.empty())
        return {};

    int sz = max(a.size(), b.size());
    int n = 1;
    while (n < sz)
        n *= 2;

    vector<T> aHat(a.begin(), a.end()), bHat(b.begin(), b.end());
    aHat.resize(n);
    bHat.resize(n);

    FWHT(aHat);
    FWHT(bHat);

    for (int i = 0; i < n; i++)
        aHat[i] = mult64(aHat[i], bHat[i]);

    FWHT(aHat, true);

    return aHat;
}

/**
 * Polynomial Modular Power using XOR convolution
 * Computes P^power efficiently using binary exponentiation
 * Each multiplication is done via XOR convolution
 * Time: O(n log n * log power)
 */
template <typename T>
vector<T> PolyModPow_XOR(vector<T> P, ll power)
{
    vector<T> res{1};
    while (power)
    {
        if (power & 1)
            res = convolute_xor(res, P);
        P = convolute_xor(P, P);
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
    int t = 1;
    ll N;
    // cin >> t;
    while (t--)
    {
        cin >> N;
        vector<ll> Poly1(1 << N), Poly2(1 << N);
        for (int i{}; i < Poly1.size(); i++)
            cin >> Poly1[i];
        for (int i{}; i < Poly2.size(); i++)
            cin >> Poly2[i];
        vector<ll> res = convolute_xor(Poly1, Poly2);
        for (int i{}; i < res.size(); i++)
            cout << res[i] << " ";
    }

    return 0;
}