#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

/*
p                   | deg | g
132120577             19    5
998244353             23    3
469762049             26    3
1107296257            24    10
10000093151233        26    5
1000000523862017      26    3
1000000000949747713   26    2
*/
const ll NTT_MOD = 998244353;
const ll PRIMITIVE_ROOT = 3; // Primitive root for 998244353

#define double_size_t std::conditional_t<(NTT_MOD > (1LL << 31)), __int128_t, long long>

inline ll add64(const ll &a, const ll &b)
{
    double_size_t res = double_size_t(a) + b;
    if (res >= NTT_MOD)
        res -= NTT_MOD;
    return res;
}

inline ll sub64(const ll &a, const ll &b)
{
    double_size_t res = double_size_t(a) - b;
    if (res < 0)
        res += NTT_MOD;
    if (res >= NTT_MOD)
        res -= NTT_MOD;
    return res;
}

inline ll mult64(const ll &a, const ll &b)
{
    return double_size_t(a) * b % NTT_MOD;
}

ll modPow(ll N, ll power)
{
    if (N % NTT_MOD == 0 || N == 0)
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

// Find primitive root (generator) for modulus of form 2^a * b + 1
ll findPrimitiveRoot(ll mod)
{
    vector<ll> primeFactors;
    ll phi = mod - 1; // Euler's totient for prime modulus
    ll temp = phi;

    // Find all prime factors of phi
    for (ll i = 2; i * i <= temp; i++)
    {
        if (temp % i == 0)
        {
            primeFactors.push_back(i);
            while (temp % i == 0)
                temp /= i;
        }
    }
    if (temp > 1)
        primeFactors.push_back(temp);

    // Check each potential generator
    for (ll candidate = 2; candidate <= mod; candidate++)
    {
        bool isPrimitiveRoot = true;
        for (size_t i = 0; i < primeFactors.size() && isPrimitiveRoot; ++i)
        {
            // Check if candidate^(phi/p) % mod = 1 for any prime factor p
            isPrimitiveRoot &= modPow(candidate, phi / primeFactors[i]) != 1;
        }
        if (isPrimitiveRoot)
            return candidate;
    }
    return -1; // No primitive root found (shouldn't happen for valid NTT modulus)
}

// Number Theoretic Transform (NTT) - In-place transformation
template <typename T>
void NTT(vector<T> &coeff, bool inverse = false)
{
    T n = (T)coeff.size();             // arraySize
    T logSize = 31 - __builtin_clz(n); // log2(arraySize)

    // Precompute roots of unity for efficient NTT
    vector<ll> w(2, 1);
    for (T len = 2, s = 2; len < n; len <<= 1, s++)
    {
        w.resize(n);
        // Generate primitive (len)-th roots of unity
        ll root = modPow(PRIMITIVE_ROOT, NTT_MOD >> s);
        if (inverse)
            root = modPow(root, NTT_MOD - 2);
        ll wp[] = {1, root};
        for (T i = len; i < 2 * len; ++i)
            w[i] = mult64(w[i >> 1], wp[i & 1]);
    }

    // Bit-reversal permutation for Cooley-Tukey algorithm
    vector<T> rev(n);
    for (T i = 0; i < n; ++i)
    {
        rev[i] = (rev[i >> 1] | (i & 1) << logSize) / 2;
        if (i < rev[i])
            swap(coeff[i], coeff[rev[i]]);
    }
    // Cooley-Tukey FFT butterfly operations
    for (T len = 1; len < n; len *= 2)
    {
        for (T st = 0; st < n; st += 2 * len)
        {
            for (T j = 0; j < len; ++j)
            {
                T u = mult64(w[j + len], coeff[st + j + len]);
                T &v = coeff[st + j];
                coeff[st + j + len] = v - u + (u > v ? NTT_MOD : 0);
                v += (v + u >= NTT_MOD ? u - NTT_MOD : u);
            }
        }
    }
}

// Polynomial convolution using Number Theoretic Transform
template <typename T>
vector<T> convolute(vector<T> P1, vector<T> P2)
{
    if (P1.empty() || P2.empty())
        return {};

    T resultSize = (T)P1.size() + (T)P2.size() - 1;
    // T paddedSize = __bit_ceil(resultSize);           // Next power of 2
    T logSize = 32 - __builtin_clz(resultSize - 1);  // Ceiling of log2(resultSize)
    T paddedSize = 1 << logSize;                     // Next power of 2
    T inverseSize = modPow(paddedSize, NTT_MOD - 2); // Modular inverse

    // Prepare padded copies for NTT
    P1.resize(paddedSize);
    P2.resize(paddedSize);

    // Forward NTT on both polynomials
    NTT(P1, false);
    NTT(P2, false);

    // Point-wise multiplication in frequency domain
    for (T i = 0; i < paddedSize; ++i)
        P1[i] = mult64(mult64(P1[i], P2[i]), inverseSize);

    // Inverse NTT to get final result
    NTT(P1, true);

    // Return result trimmed to actual size
    return {P1.begin(), P1.begin() + resultSize};
}

template <typename T>
vector<T> PolyModPow(vector<T> P, ll power)
{
    vector<T> res{1};
    while (power)
    {
        if (power & 1)
            res = convolute(res, P);
        P = convolute(P, P);
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
    ll N, K, Q;
    // cin >> t;
    while (t--)
    {
        cin >> N >> K;
        vector<int> A(N);
        for (int i{}; i < N; i++)
            cin >> A[i];

        vector<int> B(N);
        // (1 + B[0]x)(1 + B[1]x)...(1 + B[N - 1]x)
        // Query coefficient of x^k => sum of products of B[i]'s taken k at a time
        function<vector<int>(int, int)> query = [&](int l, int r)
        {
            if (l == r)
                return vector<int>{1, B[l]};
            int mid = (l + r) / 2;
            return convolute(query(l, mid), query(mid + 1, r));
        };
        cin >> Q;
        vector<int> orig = A;
        while (Q--)
        {
            int type, q, i, L, R, d;
            cin >> type;
            if (type == 1)
            {
                cin >> q >> i >> d;
                i--;
                A[i] = d;
                for (int i{}; i < N; i++)
                    B[i] = sub64(q, A[i]);
            }
            else
            {
                cin >> q >> L >> R >> d;
                L--, R--;
                for (int i = L; i <= R; i++)
                    A[i] = add64(A[i], d);
                for (int i{}; i < N; i++)
                    B[i] = sub64(q, A[i]);
            }
            vector<int> ans = query(0, N - 1);
            cout << ans[K] << endl;
            A = orig;
        }
    }
    return 0;
}