#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

const ll mod = 1e9 + 7;
inline ll mult64(const ll &a, const ll &b)
{
    return __int128_t(a) * b % mod;
}

inline ll add64(const ll &a, const ll &b)
{
    ll res = a + b;
    if (res >= mod)
        res -= mod;
    return res;
}

inline ll sub64(const ll &a, const ll &b)
{
    ll res = a - b;
    if (res < 0)
        res += mod;
    if (res >= mod)
        res -= mod;
    return res;
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

template <typename T>
T modInv(T N, T mod)
{
    return N <= 1 ? N : mod - (mod / N) * modInv(mod % N, mod) % mod;
}

vector<tuple<ll, ll, ll>> floorRange(ll N)
{
    vector<tuple<ll, ll, ll>> ranges;
    ll L = 1;
    while (L <= N)
    {
        ll R = N / (N / L);
        ranges.emplace_back(L, R, N / L);
        L = R + 1;
    }
    ranges.emplace_back(N + 1, N + 1, 0);
    reverse(ranges.begin(), ranges.end());
    return ranges;
}

void execOnBlocks(function<void(pair<ll, ll>, pair<ll, ll>, pair<ll, ll>)> func, ll N)
{
    ll sqrtN = sqrt(N);
    auto floors = floorRange(N);
    ll numRanges = floors.size() - 1;

    auto toOrd = [&](ll k)
    {
        return k <= sqrtN ? k : floors.size() - N / k;
    };

    func({1, 1}, {1, 1}, {1, numRanges});

    for (ll k = 2; k < floors.size(); k++)
    {
        ll z = floors.size() - k;

        for (ll x = 2;; x++)
        {
            ll yLo = max(toOrd(x), toOrd(z)) + 1;
            ll yHi = toOrd(N / (x * z));
            if (yHi < yLo)
                break;
            func({x, x}, {yLo, yHi}, {k, k});
            func({yLo, yHi}, {x, x}, {k, k});
        }

        func({1, 1}, {k, k}, {k, numRanges});
        func({k, k}, {1, 1}, {k, numRanges});

        ll x = k;
        for (ll y = 2; y < k; y++)
        {
            ll zLo = toOrd(x * y);
            ll zHi = toOrd(N / x);
            if (zHi < zLo)
                break;
            func({x, x}, {y, y}, {zLo, zHi});
            func({y, y}, {x, x}, {zLo, zHi});
        }

        ll zLo = toOrd(x * x);
        func({x, x}, {x, x}, {zLo, numRanges});
    }
}

vector<ll> dirichletMultiply(const vector<ll> &F, const vector<ll> &G, ll N)
{
    vector<ll> H(F.size() + 2, 0);

    auto propagate = [&](pair<ll, ll> x, pair<ll, ll> y, pair<ll, ll> z)
    {
        ll deltaF = sub64(F[x.second], F[x.first - 1]);
        ll deltaG = sub64(G[y.second], G[y.first - 1]);
        ll prod = mult64(deltaF, deltaG);
        H[z.first] = add64(H[z.first], prod);
        H[z.second + 1] = sub64(H[z.second + 1], prod);
    };

    execOnBlocks(propagate, N);

    for (size_t i = 1; i < H.size(); i++)
        H[i] = add64(H[i], H[i - 1]);

    H.pop_back();
    return H;
}

vector<ll> dirichletDivide(vector<ll> H, const vector<ll> &G, ll N)
{
    H.push_back(0);
    vector<ll> H_diff(H.size());
    H_diff[0] = 0;
    for (int i = 1; i < (int)H.size(); i++)
        H_diff[i] = sub64(H[i], H[i - 1]);
    H = H_diff;

    vector<ll> F(G.size(), -1);
    F[0] = 0;

    auto propagate = [&](pair<ll, ll> x, pair<ll, ll> y, pair<ll, ll> z)
    {
        ll deltaG = sub64(G[y.second], G[y.first - 1]);

        // If F[x.second] is not set yet
        if (F[x.second] == -1)
        {
            // Use modular division: H[z.first] / deltaG (mod p)
            ll quotient = mult64(H[z.first], modInv(deltaG, mod));
            F[x.second] = add64(F[x.first - 1], quotient);
        }

        // Now modify H using the computed F
        ll deltaF = sub64(F[x.second], F[x.first - 1]);
        ll delta = mult64(deltaF, deltaG);
        H[z.first] = sub64(H[z.first], delta);
        H[z.second + 1] = add64(H[z.second + 1], delta);
    };

    execOnBlocks(propagate, N);
    return F;
}

ll phiSum(ll N)
{
    auto floors = floorRange(N);
    vector<ll> H, G;

    for (auto &[L, R, nk] : floors)
    {
        ll s = mult64(nk, nk + 1);
        s = mult64(s, 500000004); // divide by 2 modulo (1e9 + 7)
        H.push_back(s);
        G.push_back(nk);
    }

    vector<ll> F = dirichletDivide(H, G, N);
    return F.back();
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
        cout << phiSum(N) << endl;
    }
    return 0;
}