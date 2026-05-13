#include <bits/stdc++.h>
using namespace std;
#define i128 __int128_t
#define ll long long int
#define endl "\n"

// Use type u64
namespace Montgomery64
{
	using u64 = uint64_t;
	using u128 = __uint128_t;

	inline u64 mult64(u64 a, u64 b, u64 mod) { return (u128)a * b % mod; }

	static inline u64 inv64_2k(u64 n0)
	{
		u64 x = 1;
		for (int i = 6; i > 0; i--)
			x *= 2 - n0 * x;
		return x;
	}

	inline u64 montModInv(u64 n) { return 0 - inv64_2k(n); }
	inline u64 montMult(u64 a, u64 b, u64 n, u64 n0Prime)
	{
		u128 t = (u128)a * b;
		u64 m = (u64)t * n0Prime;
		u128 res = (t >> 64) + (((u128)m * n) >> 64) + ((u64)t != 0);
		if (res >= n)
			res -= n;
		return res;
	}
}
using namespace Montgomery64;

template <typename T>
inline T absVal(T N) { return N < 0 ? -N : N; }

template <typename T>
inline T F(T x, T c, T mod, T inv) // Pollard-Rho function
{
	x = montMult(x, x, mod, inv);
	x = x >= mod - c ? x - mod + c : x + c;
	return x;
}

template <typename T>
T pollardBrent(T N)
{
	if (!(N & 1))
		return 2;

	// Random Number Linear Congruential Generator MMIX from D.E. Knuth
	static u128 rng = 0xdeafbeefff;
	uint64_t a = rng * 6364136223846793005ULL + 1442695040888963407ULL;
	uint64_t b = a * 6364136223846793005ULL + 1442695040888963407ULL;
	rng = (a + b) ^ (a * b);

	T X0 = 1 + a % (N - 1);
	T C = 1 + b % (N - 1);
	T X = X0; // X1
	T gcdVal = 1;
	T q = 1;
	T Xs, Xt;
	T m = 128;
	u64 inv = montModInv(N);
	T L = 1;
	while (gcdVal == 1)
	{
		Xt = X;
		for (size_t i = 1; i < L; i++)
			X = F(X, C, N, inv);

		uint64_t k = 0;
		while (k < L && gcdVal == 1)
		{
			Xs = X;
			for (size_t i = 0; i < m && i + k < L; i++)
			{
				X = F(X, C, N, inv);
				q = montMult(q, Xt > X ? Xt - X : X - Xt, N, inv);
			}
			gcdVal = __gcd(q, N);
			k += m;
		}
		L += L;
	}
	if (gcdVal == N) // Failure
	{
		do
		{
			Xs = F(Xs, C, N, inv);
			gcdVal = __gcd(Xs > Xt ? Xs - Xt : Xt - Xs, N);
		} while (gcdVal == 1);
	}
	return gcdVal;
}

template <typename T>
T modPow(T N, T power, T mod)
{
	if (N % mod == 0 || N == 0)
		return 0;
	if (N == 1 || power == 0)
		return 1;
	T res{1};
	while (power)
	{
		if (power & 1)
			res = mult64(res, N, mod);
		N = mult64(N, N, mod);
		power >>= 1;
	}
	return res;
}

template <typename T>
bool isPrime(T N)
{
	constexpr uint64_t MASK = 0x28208A20A08A28ACULL;
	constexpr uint32_t WHEEL30 = 0x208A2882;
	if (N < 64)
		return (MASK >> N) & 1;
	if (!((WHEEL30 >> (uint32_t)(N % 30)) & 1))
		return false;

	T d = N - 1;
	int s{};
	while (!(d & 1))
		d >>= 1, ++s;
	for (const T &a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022})
	{
		T p = modPow(a % N, d, N), i = s;
		while (p != 1 && p != N - 1 && a % N && i--)
			p = mult64(p, p, N);
		if (p != N - 1 && i != s)
			return false;
	}
	return true;
}

template <typename T>
void primeFactorize(T N, vector<T> &primeFactors)
{
	if (N == 1)
		return;

	if (isPrime(N))
	{
		primeFactors.push_back(N);
		return;
	}
	T Y = pollardBrent(N);
	primeFactorize(Y, primeFactors);
	primeFactorize(N / Y, primeFactors);
}

// Euler Totient Function
map<int, int> mp;
template <typename T>
T phi(T N)
{
	if (mp.count(N))
		return mp[N];
	if (isPrime(N))
		return mp[N] = N - 1;
	vector<T> pf;
	primeFactorize(N, pf);
	sort(pf.begin(), pf.end());
	T ans = N;
	for (int i = 0; i < pf.size();)
	{
		T p = pf[i];
		while (p == pf[i])
			i++;
		ans -= (ans / p);
	}
	return mp[N] = ans;
}

u64 normalize(u64 x, u64 m)
{
	if (x < m)
		return x;
	return (m + x % m);
}

template <typename T>
T Exp(T N, T power, T mod)
{
	T res{1};
	while (power)
	{
		if (power & 1)
			res = normalize(res * N, mod);

		N = normalize(N * N, mod);
		power >>= 1;
	}
	return res;
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
		u64 n, m;
		cin >> n >> m;
		vector<u64> w(n + 1);
		for (int i = 1; i <= n; i++)
			cin >> w[i];
		function<u64(u64, u64, u64)> solve = [&](u64 l, u64 r, u64 m) -> u64
		{
			if (l == r + 1 || m == 1)
				return 1;

			u64 power = solve(l + 1, r, phi(m));
			u64 res = Exp(w[l], power, m);
			return res; // Don't take mod here
		};
		int q;
		cin >> q;
		while (q--)
		{
			int l, r;
			cin >> l >> r;
			cout << solve(l, r, m) % m << endl;
		}
	}
	return 0;
}