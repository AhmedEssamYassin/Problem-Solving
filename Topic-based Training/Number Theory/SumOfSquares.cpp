#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define u64 uint64_t
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

template <typename T>
bool isPerfectSquare(T n, T &r)
{
	if (n == 0 || n == 1)
		return r = n, true;

	static constexpr unsigned long long MASK =
	    0x0202021202030213ULL;

	if (((MASK >> (n & 63)) & 1) == 0)
		return false;

	r = iSqrt(n);
	return r * r == n;
}

using G = pair<ll, ll>;
G gm(G x, G y) { return {x.first * y.first - x.second * y.second, x.first * y.second + x.second * y.first}; }
G gp(G x, u64 e)
{
	G r{1, 0};
	for (; e; e >>= 1, x = gm(x, x))
		if (e & 1)
			r = gm(r, x);
	return r;
}
G cor(u64 p) // Cornacchia/Hermite-Serret
{
	if (p == 2)
		return {1, 1};
	u64 x, a = p, b;
	for (u64 g = 2;; g++)
	{
		x = modPow<u64>(g, (p - 1) / 4, p);
		if (mult64(x, x, p) == p - 1)
			break;
	}
	for (b = x; (u128)b * b > p;)
		tie(a, b) = make_pair(b, a % b);
	return {b, iSqrt(p - b * b)};
}
optional<G> two(u64 n) // Fermat two-squares
{
	if (!n)
		return G{0, 0};
	map<u64, u64> f;
	primeFactorize(n, f);
	G r{1, 0};
	for (auto &[p, e] : f)
	{
		if (p % 4 == 3)
		{
			if (e & 1)
				return nullopt;
			r = gm(r, gp({(ll)p, 0}, e / 2));
		}
		else
			r = gm(r, gp(cor(p), e));
	}
	return G{llabs(r.first), llabs(r.second)};
}
array<ll, 3> three(ll n) // Legendre three-squares
{
	for (ll i = iSqrt(n); i >= 0; i--)
		if (auto g = two(n - i * i))
			return {i, g->first, g->second};
	return {};
}
vector<ll> minSquares(ll x) // Lagrange/Legendre case split
{
	if (!x)
		return {0};
	ll m = 1, t = x, r;
	while (t % 4 == 0)
		m *= 2, t /= 4;
	vector<ll> v;
	r = iSqrt(t);
	if (r * r == t)
		v = {r};
	else if (auto g = two(t))
		v = {g->first, g->second};
	else if (t % 8 != 7)
	{
		auto a = three(t);
		v = {a[0], a[1], a[2]};
	}
	else
	{
		auto a = three(t - 1);
		v = {1, a[0], a[1], a[2]};
	}
	for (ll &y : v)
		y *= m;
	return v;
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
		ll n;
		cin >> n;
		auto vc = minSquares(n);
		while (vc.size() < 4)
			vc.push_back(0);
		for (const auto &x : vc)
			cout << x << " ";
		cout << endl;
	}
	return 0;
}