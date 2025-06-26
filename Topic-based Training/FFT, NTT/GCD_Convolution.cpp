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

/**
 * Mobius Transform for GCD Convolution
 * Forward: sumOverMultiples, Inverse: mobiusInversion
 * Result[i] = sum(a[j] * b[k]) where gcd(j,k) = i
 */
template <typename T>
void mobiusTransform(vector<T> &polyVec, bool inverse = false)
{
    int vecSize = polyVec.size();

    if (!inverse)
    {
        for (int divIdx = 1; divIdx < vecSize; divIdx++)
        {
            for (int mulIdx = 2 * divIdx; mulIdx < vecSize; mulIdx += divIdx)
                polyVec[divIdx] = add64(polyVec[divIdx], polyVec[mulIdx]);
        }
    }
    else
    {
        vector<int> mu(vecSize);
        mu[1] = 1;
        for (int baseIdx = 1; baseIdx < vecSize; baseIdx++)
        {
            for (int mulIdx = 2 * baseIdx; mulIdx < vecSize; mulIdx += baseIdx)
                mu[mulIdx] -= mu[baseIdx];
        }

        vector<T> tempVec(vecSize);
        for (int divIdx = 1; divIdx < vecSize; divIdx++)
        {
            for (int mulIdx = divIdx; mulIdx < vecSize; mulIdx += divIdx)
            {
                if (mu[mulIdx / divIdx] > 0)
                    tempVec[divIdx] = add64(tempVec[divIdx], mult64(polyVec[mulIdx], mu[mulIdx / divIdx]));
                else if (mu[mulIdx / divIdx] < 0)
                    tempVec[divIdx] = sub64(tempVec[divIdx], mult64(polyVec[mulIdx], -mu[mulIdx / divIdx]));
            }
        }
        polyVec = tempVec;
    }
}

/**
 * GCD Convolution using Mobius Transform
 * Result[i] = sum(a[j] * b[k]) for all gcd(j,k) = i
 */
template <typename T>
vector<T> convoluteGcd(const vector<T> &polyA, const vector<T> &polyB)
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
        transA[idx] = mult64(transA[idx], transB[idx]);

    mobiusTransform(transA, true);

    return transA;
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
        // GCD works with 1-based Polynomials (Since gcd = 0 is undefined)
        vector<ll> Poly1(N + 1), Poly2(N + 1);
        for (int i{1}; i <= N; i++)
            cin >> Poly1[i];
        for (int i{1}; i <= N; i++)
            cin >> Poly2[i];
        vector<ll> res = convoluteGcd(Poly1, Poly2);
        for (int i{1}; i < res.size(); i++)
            cout << res[i] << " ";
    }

    return 0;
}