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

inline ll div64(const ll &a, const ll &b)
{
    return mult64(a, modPow(b, mod - 2));
}

inline ll normalize(ll n)
{
    n %= mod;
    (n < 0) && (n += mod);
    return n;
}

ll calc(const vector<ll> &y, ll N)
{
    // Σ y[i] * ((Π (N - j)) / (Π (i - j)))
    // L{i}(N) = L{i - 1}(N) * ((N - (i - 1)) / (N - i)) * ((i - k) / i)

    int k = y.size() - 1;
    ll ans = 0;

    // Precompute total product of all (N - j) terms
    ll PI_N_minus_j = 1;
    for (int j = 1; j <= k; j++)
        PI_N_minus_j = mult64(PI_N_minus_j, N - j);

    // Initialize running factorial values
    ll leftFact = 1;  // For (i - 1)!
    ll rightFact = 1; // For (k - i - 1)!

    // Precompute rightFact and rightCnt2s for i = 1 case (which is (k - 1)!)
    for (int i = 1; i <= k - 1; i++)
        rightFact = mult64(rightFact, i);

    // Calculate answer for each i
    for (int i = 1; i <= k; i++)
    {
        // Sign calculation: (-1)^(number of negative terms)
        ll isNeg = ((k - i) & 1) ? -1 : 1;

        // Denominator is (i - 1)! * (k - i)!
        ll denominator = mult64(leftFact, normalize(isNeg * rightFact));

        // Numerator is total product divided by (N - i)
        ll numerator = PI_N_minus_j;

        // Divide by (N - i)
        numerator = div64(numerator, N - i);
        ans = add64(ans, mult64(y[i], div64(numerator, denominator)));

        // For next iteration,
        // we need ((i + 1) - 1)! = (i)!
        // we need (k - (i + 1))! = (k - i - 1)!, Current rightFact is (k - i)!, so divide by (k - i)

        leftFact = mult64(leftFact, i);
        rightFact = mult64(rightFact, modPow(k - i, mod - 2));
    }

    return ans;
}

ll F(ll N, ll K)
{
    // How the problem define 0^0
    if (N == 0 && K == 0)
        return 0; // 1 if 0^0 is 1
    if (K == 0)
        return N; // Add 1 if 0^0 is 1
    vector<ll> y;
    ll sum = 0;
    y.push_back(sum);
    for (int i{1}; i <= K + 2; i++)
    {
        sum = add64(sum, modPow(i, K));
        y.push_back(sum);
    }
    if (N < y.size())
        return y[N];
    return calc(y, N);
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
    ll N, K;
    // cin >> t;
    while (t--)
    {
        cin >> N >> K;
        cout << F(N, K);
    }
    return 0;
}