#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll mod = 1e9 + 7;
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
 * AND Transform (Superset Sum Transform)
 * Computes sum over all supersets - for each bit position,
 * adds values from positions with that bit to positions without that bit
 * Used for superset convolution: c[i] = sum(a[j] * b[k]) where j&k = i
 */
template <typename T>
void AND_Transform(vector<T> &P, bool inverse = false)
{
    int n = P.size();
    int LOG = __builtin_ctz(n);

    for (int bit = 0; bit < LOG; bit++)
    {
        for (int i = 0; i < n; i++)
        {
            if (!((i >> bit) & 1))
            {
                if (!inverse)
                    P[i] = add64(P[i], P[i ^ (1 << bit)]);
                else
                    P[i] = sub64(P[i], P[i ^ (1 << bit)]);
            }
        }
    }
}

/**
 * AND Convolution using Superset Sum Transform
 * Multiplies two polynomials where combination is AND operation
 * Result[i] = sum(a[j] * b[k]) for all j&k = i
 * Critical for problems involving intersection of sets
 */
template <typename T>
vector<T> convolute_and(const vector<T> &a, const vector<T> &b)
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

    AND_Transform(aHat);
    AND_Transform(bHat);

    for (int i = 0; i < n; i++)
        aHat[i] = mult64(aHat[i], bHat[i]);

    AND_Transform(aHat, true);

    return aHat;
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
    ll N, K;
    // cin >> t;
    while (t--)
    {
        cin >> N;
        vector<ll> coeff(1 << 20, 0);
        // Create polynomial: coeff[i] = 1 if value i exists in input, 0 otherwise
        for (int i = 0; i < N; i++)
        {
            ll x;
            cin >> x;
            coeff[x] = 1;
        }

        coeff[0] = 1; // Empty subset contribution

        // Apply AND transform
        AND_Transform(coeff);

        // Convert to 2^count for each position
        for (int i = 0; i < coeff.size(); i++)
        {
            if (coeff[i] > 0)
                coeff[i] = modPow(2, coeff[i]) - 1; // Exclude the empty subset
        }

        // Apply inverse AND transform
        AND_Transform(coeff, true);

        // Count non-zero positions
        ll cnt = 0;
        for (int i = 0; i < coeff.size(); i++)
            cnt += (coeff[i] != 0);

        cout << cnt << endl;
    }

    return 0;
}