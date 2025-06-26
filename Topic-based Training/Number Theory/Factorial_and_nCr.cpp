#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll mod = (119 << 23) + 1; // = 998244353

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

ll modPow(ll N, ll power, ll mod)
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

vector<ll> factorial, invFactorial;
void preCompute(int N)
{
    factorial.assign(N + 1, 0);
    invFactorial.assign(N + 1, 0);
    factorial[0] = 1;
    invFactorial[0] = 1;
    for (int i = 1; i <= N; i++)
        factorial[i] = mult64(i, factorial[i - 1]);

    invFactorial[N] = modPow(factorial[N], mod - 2, mod);
    for (int i = N - 1; i >= 0; i--)
        invFactorial[i] = mult64(invFactorial[i + 1], (i + 1));
}
static int autoCall = (preCompute(1000000), 0);

ll nCr(ll n, ll r)
{
    if (r < 0 || r > n)
        return 0;
    if (n == r || r == 0)
        return 1;
    return mult64(factorial[n], mult64(invFactorial[r], invFactorial[n - r]));
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
    ll N, K, Q, X;
    // cin >> t;
    while (t--)
    {
        cin >> N >> K >> Q >> X;
        while (Q--)
        {
            ll i, L, R;
            cin >> i >> L >> R;
            cout << mult64(X, sub64(nCr(R + i, i + 1), nCr(L + i - 1, i + 1))) << endl;
        }
    }
    return 0;
}
