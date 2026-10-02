#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

typedef unsigned long long ull;
typedef uint32_t u32;
const ll mod = 1e9 + 7;
inline ull sub(ull a, ull b) { return a >= b ? a - b : a + mod - b; }

// Returns sum_{k = 1..N} phi(k) under modulo. Stateless, safe to call repeatedly.
ll sumPhi(ll n)
{
    if (n < 100)
    {
        ll r = 0;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= i; j++)
                r += __gcd(i, j) == 1;
        return r;
    }
    ll sq = sqrtl(n);
    vector<int> P;
    {
        vector<uint8_t> c(sq + 1);
        for (ll i = 2; i <= sq; i++)
        {
            if (!c[i])
            {
                P.push_back(i);
                for (ll j = i * i; j <= sq; j += i)
                    c[j] = 1;
            }
        }
    }
    P.push_back(INT_MAX);                                          // sentinel
    int K = upper_bound(P.begin(), P.end(), cbrt(sq)) - P.begin(); // sieve P[0..K]; "rough" = all prime factors > P[K] > cbrt(sq)
    ll m = (ll)P[K] * sq;
    vector<double> inv(sq + 1);
    for (ll i = 1; i <= sq; i++)
        inv[i] = (1 + 1e-15) / i;
    auto dv = [&](ll x, ll y)
    { return (ll)(x * inv[y]); };                              // floor(x / y) without a hardware divide
    vector<ll> lc(sq + 1), hc(sq + 1), lm(sq + 1), hm(sq + 1); // l * [i]: value at i, h * [i]: value at n / i; c = rough count, m = rough sum of mu
    for (ll i = 1; i <= sq; i++)
        lc[i] = i, hc[i] = n / i;
    auto step = [&](vector<ll> &H, vector<ll> &L, ll p) // f(x) -= f(x / p), in place, for all x in {i} and {n / i}
    {
        ll t = dv(sq, p), tn = dv(n, p);
        for (ll i = 1; i <= t; i++)
            H[i] -= H[i * p];
        for (ll i = t + 1; i <= sq; i++)
            H[i] -= L[dv(tn, i)];
        for (ll i2 = t, i = sq; i2; i2--)
            for (ll v = L[i2]; i >= i2 * p; i--)
                L[i] -= v;
    };
    for (int j = 0; j <= K; j++)
        step(hc, lc, P[j]);

    // rough numbers <= m with mu + 1 != 0 are only 1, pq, p^2, p^2 q: add (mu + 1) for them, the counts above supply the "-1"
    lm[1] = 2;
    for (int j = K + 1;; j++)
    {
        ll p = P[j], p2 = p * p, q;
        if (p2 > m)
            break;
        ll t = dv(sq, p), tn = dv(n, p), tm = dv(m, p);
        int j2 = j + 1;
        for (; (q = P[j2]) <= t; j2++)
            lm[p * q] += 2;
        for (; (q = P[j2]) <= tm; j2++)
            hm[dv(tn, q)] += 2;
        t = dv(t, p), tn = dv(tn, p), tm = dv(tm, p);
        (p2 <= sq ? lm[p2] : hm[tn])++;
        for (j2 = K + 1; (q = P[j2]) <= t; j2++)
            lm[p2 * q]++;
        for (; (q = P[j2]) <= tm; j2++)
            hm[dv(tn, q)]++;
    }
    for (ll i = 1; i <= sq; i++)
        lm[i] += lm[i - 1];
    hm[sq] += lm[sq];
    for (ll i = sq; i > 0; i--)
        hm[i - 1] += hm[i];
    for (ll i = 1; i <= sq; i++)
        lm[i] -= lc[i], hm[i] -= hc[i];

    // n / i > m: hyperbola on (mu * 1) over rough numbers = [x == 1]
    vector<pair<ll, ll>> R; // rough x in [2, sq] with mu(x)
    for (ll i = 2; i <= sq; i++)
        if (lc[i] != lc[i - 1])
            R.push_back({i, lm[i] - lm[i - 1]});
    R.push_back({sq + 1, 0});
    for (ll i = n / m; i > 0; i--)
    {
        ll X = dv(n, i), B = sqrtl(X), s = 1 - hc[i] + lc[B] * lm[B], t;
        int j = 0;
        for (; (t = i * R[j].first) <= sq; j++)
            s -= hm[t] + hc[t] * R[j].second;
        for (; R[j].first <= B; j++)
            t = dv(X, R[j].first), s -= lm[t] + lc[t] * R[j].second;
        hm[i] = s;
    }
    for (int j = K; j >= 0; j--) // add the small primes back: rough Mertens -> Mertens
        step(hm, lm, P[j]);

    auto S = [&](ll x)
    { x %= mod; return x * (x + 1) / 2 % mod; };
    ll ans = -S(sq) * lm[sq]; // sum phi = sum_{a * b <= n} mu(a) * b
    for (ll i = 1; i <= sq; i++)
        ans = (ans + i * hm[i] + (lm[i] - lm[i - 1]) * S(dv(n, i))) % mod;
    return (ans % mod + mod) % mod;
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
    // cin >> t;
    while (t--)
    {
        ll N;
        cin >> N;
        cout << sumPhi(N);
    }
    return 0;
}