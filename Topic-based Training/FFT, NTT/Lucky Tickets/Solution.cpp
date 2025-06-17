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

inline ll add64(const ll &a, const ll &b, const ll &mod)
{
	double_size_t res = double_size_t(a) + b;
	if (res >= mod)
		res -= mod;
	return res;
}

inline ll sub64(const ll &a, const ll &b, const ll &mod)
{
	double_size_t res = double_size_t(a) - b;
	if (res < 0)
		res += mod;
	if (res >= mod)
		res -= mod;
	return res;
}

inline ll mult64(const ll &a, const ll &b, const ll &mod)
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
			res = mult64(res, N, mod);
		N = mult64(N, N, mod);
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
			isPrimitiveRoot &= modPow(candidate, phi / primeFactors[i], mod) != 1;
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
	T n = (T)coeff.size();			   // arraySize
	T logSize = 31 - __builtin_clz(n); // log2(arraySize)

	// Precompute roots of unity for efficient NTT
	vector<ll> w(2, 1);
	for (T len = 2, s = 2; len < n; len <<= 1, s++)
	{
		w.resize(n);
		// Generate primitive (len)-th roots of unity
		ll root = modPow(PRIMITIVE_ROOT, NTT_MOD >> s, NTT_MOD);
		if (inverse)
			root = modPow(root, NTT_MOD - 2, NTT_MOD);
		ll wp[] = {1, root};
		for (T i = len; i < 2 * len; ++i)
			w[i] = mult64(w[i >> 1], wp[i & 1], NTT_MOD);
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
				T u = mult64(w[j + len], coeff[st + j + len], NTT_MOD);
				T &v = coeff[st + j];
				coeff[st + j + len] = v - u + (u > v ? NTT_MOD : 0);
				v += (v + u >= NTT_MOD ? u - NTT_MOD : u);
			}
		}
	}
}

// Optimized polynomial trimming
template <typename T>
vector<T> &trimPolynomial(vector<T> &Poly)
{
	int lastNonZero = (int)Poly.size() - 1;
	while (lastNonZero >= 0 && !Poly[lastNonZero])
		lastNonZero--;
	Poly.resize(lastNonZero + 1);
	return Poly;
}

// Polynomial convolution using Number Theoretic Transform
template <typename T>
vector<T> convolute(vector<T> P1, vector<T> P2)
{
	if (P1.empty() || P2.empty())
		return {};

	T resultSize = (T)P1.size() + (T)P2.size() - 1;
	// T paddedSize = __bit_ceil(resultSize);           // Next power of 2
	T logSize = 32 - __builtin_clz(resultSize - 1);			  // Ceiling of log2(resultSize)
	T paddedSize = 1 << logSize;							  // Next power of 2
	T inverseSize = modPow(paddedSize, NTT_MOD - 2, NTT_MOD); // Modular inverse

	// Prepare padded copies for NTT
	P1.resize(paddedSize);
	P2.resize(paddedSize);

	// Forward NTT on both polynomials
	NTT(P1, false);
	NTT(P2, false);

	// Point-wise multiplication in frequency domain
	for (T i = 0; i < paddedSize; ++i)
		P1[i] = mult64(mult64(P1[i], P2[i], NTT_MOD), inverseSize, NTT_MOD);

	// Inverse NTT to get final result
	NTT(P1, true);

	// Return result trimmed to actual size
	return trimPolynomial(P1);
}

/*
ll CRT(ll a, ll m1, ll b, ll m2)
{
	__int128 m = m1 * m2;
	ll ans = a * m2 % m * modPow(m2, m1 - 2, m1) % m + m1 * b % m * modPow(m1, m2 - 2, m2) % m;
	return ans % m;
}

ll mod, root, desired_mod = 1000000007;
const ll mod1 = 167772161;
const ll mod2 = 469762049;
const ll mod3 = 754974721;
const ll root1 = 3;
const ll root2 = 3;
const ll root3 = 11;

ll CRT(ll a, ll b, ll c, ll m1, ll m2, ll m3)
{
	__int128 M = (__int128)m1 * m2 * m3;
	ll M1 = m2 * m3;
	ll M2 = m1 * m3;
	ll M3 = m2 * m1;

	ll M_1 = modPow(M1 % m1, m1 - 2, m1);
	ll M_2 = modPow(M2 % m2, m2 - 2, m2);
	ll M_3 = modPow(M3 % m3, m3 - 2, m3);

	__int128 ans = (__int128)a * M1 * M_1;
	ans += (__int128)b * M2 * M_2;
	ans += (__int128)c * M3 * M_3;

	return (ans % M) % desired_mod;
}
*/

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
	ll N, k;
	// cin >> t;
	while (t--)
	{
		cin >> N >> k;
		vector<int> d(10);
		for (int i{}; i < k; i++)
		{
			int di;
			cin >> di;
			d[di]++;
		}
		vector<int> ans = PolyModPow(d, N / 2);
		ll res = 0;
		for (int i{}; i < ans.size(); i++)
			res = add64(res, mult64(ans[i], ans[i], NTT_MOD), NTT_MOD);
		cout << res;
	}
	return 0;
}