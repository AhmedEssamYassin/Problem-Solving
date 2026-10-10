#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

constexpr ll INF = 2e18;
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
	T X = X0;
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
void primeFactorize(T N, map<T, T> &primeFactors)
{
	if (N == 1)
		return;

	if (isPrime(N))
	{
		primeFactors[N]++;
		return;
	}
	T Y = pollardBrent(N);
	primeFactorize(Y, primeFactors);
	primeFactorize(N / Y, primeFactors);
}

template <typename T>
void getAllFactors(T N, vector<T> &f)
{
	map<T, T> fr;
	primeFactorize(N, fr);
	f = {1};
	for (auto &[p, c] : fr)
		for (size_t i = 0, s = f.size() * c; i < s; i++)
			f.push_back(f[i] * p);
}

constexpr u64 iSqrt(u64 n) // O(log(log(n)))
{
	if (n < 2)
		return n;
	u64 xk = (u64)1 << ((bit_width(n) + 1) >> 1);
	u64 nxt = (xk + n / xk) >> 1;
	while (nxt < xk)
	{
		xk = nxt;
		nxt = (xk + n / xk) >> 1;
	}
	return xk;
}

struct T
{
	ll d, p, pk;
};

vector<T> candidates(u64 c, const vector<u64> &divs)
{
	vector<T> vc;
	for (const ll &d : divs)
	{
		if (d <= 1)
			continue;
		if (isPrime(d - 1))
			vc.push_back({d, d - 1, d - 1});
		ll r = iSqrt(d);
		for (ll p = max(2LL, r - 1); p <= r; p++)
			if (p * p + p + 1 == d && isPrime(p))
				vc.push_back({d, p, p * p});
	}
	int L = cbrtl(c) + 2;
	vector<bool> comp(L + 1);
	for (ll p = 2; p <= L; p++)
	{
		if (comp[p])
			continue;
		for (ll j = p * p; j <= L; j += p)
			comp[j] = 1;
		ll cur = p * p, s = 1 + p + cur;
		while (INF / p >= cur)
		{
			cur *= p, s += cur;
			if ((u64)s > c)
				break;
			if (c % s == 0)
				vc.push_back({s, p, cur});
		}
	}
	sort(vc.begin(), vc.end(), [](auto &a, auto &b)
	     { return a.p < b.p; });
	return vc;
}

ll minWithSigma(u64 c)
{
	vector<u64> divs;
	getAllFactors(c, divs);
	auto vc = candidates(c, divs);
	map<u64, u64> fr;
	primeFactorize<u64>(c, fr);
	vector<u64> q;
	vector<int> ex, st;
	int m = 1;
	for (auto &[a, e] : fr)
		q.push_back(a), ex.push_back(e), st.push_back(m), m *= e + 1;
	int K = q.size(), n = vc.size();
	vector<ll> dp(m, INF);
	dp[0] = 1;
	vector<pair<int, ll>> up;
	vector<int> dg(K), cu(K);
	for (int l = 0, r = 0; l < n; l = r)
	{
		while (r < n && vc[r].p == vc[l].p)
			r++;
		up.clear();
		for (int j = l; j < r; j++)
		{
			u64 d = vc[j].d;
			int di = 0, id = 0, k;
			for (k = 0; k < K; k++)
			{
				dg[k] = cu[k] = 0;
				while (d % q[k] == 0)
					d /= q[k], dg[k]++;
				di += dg[k] * st[k];
			}
			do // odometer over the divisors of c / d
			{
				if (dp[id] != INF && INF / vc[j].pk >= dp[id])
					up.push_back({id + di, dp[id] * vc[j].pk});
				for (k = 0; k < K && cu[k] == ex[k] - dg[k]; k++)
					id -= cu[k] * st[k], cu[k] = 0;
				if (k < K)
					cu[k]++, id += st[k];
			} while (k < K);
		}
		for (auto &[i, v] : up)
			dp[i] = min(dp[i], v);
	}
	return dp[m - 1] == INF ? -1 : dp[m - 1];
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
	cin >> t;
	while (t--)
	{
		u64 c;
		cin >> c;
		cout << minWithSigma(c) << endl;
	}
	return 0;
}
