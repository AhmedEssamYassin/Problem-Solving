#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define u128 __uint128_t
#define endl "\n"

// Fast I/O for 128-bit integers
using int128 = signed __int128;
using uint128 = unsigned __int128;
ostream &operator<<(ostream &stream, int128 n)
{
	if (__builtin_expect(n == 0, 0))
		return stream.put('0');
	char buf[41];
	char *end = buf + 41;
	char *p = end;
	bool neg = (n < 0);
	uint128 u = neg ? -(uint128)n : (uint128)n;
	do
	{
		*--p = (char)('0' + (unsigned)(u % 10));
		u /= 10;
	} while (u);
	if (neg)
		*--p = '-';
	return stream.write(p, end - p);
}

ostream &operator<<(ostream &stream, uint128 n)
{
	if (__builtin_expect(n == 0, 0))
		return stream.put('0');
	char buf[40];
	char *end = buf + 40;
	char *p = end;
	do
	{
		*--p = (char)('0' + (unsigned)(n % 10));
		n /= 10;
	} while (n);
	return stream.write(p, end - p);
}

istream &operator>>(istream &stream, int128 &n)
{
	n = 0;
	char c;
	if (!(stream >> c))
		return stream;
	bool neg = (c == '-');
	if (neg && !stream.get(c))
		return stream;
	while (isdigit((unsigned char)c))
	{
		n = n * 10 + (c - '0');
		if (!stream.get(c))
			break;
	}
	if (stream)
		stream.putback(c);
	if (neg)
		n = -n;
	return stream;
}

istream &operator>>(istream &stream, uint128 &n)
{
	n = 0;
	char c;
	if (!(stream >> c))
		return stream;
	while (isdigit((unsigned char)c))
	{
		n = n * 10 + (c - '0');
		if (!stream.get(c))
			break;
	}
	if (stream)
		stream.putback(c);
	return stream;
}

// Use type u128
namespace Montgomery128
{
	using u128 = __uint128_t;
	// ---------- 64-bit limb helpers (fast) ----------
	static inline void mult64(uint64_t a, uint64_t b, uint64_t &lo, uint64_t &hi)
	{
		u128 r = (u128)a * b;
		lo = (uint64_t)r;
		hi = (uint64_t)(r >> 64);
	}
	static inline uint64_t add64(uint64_t a, uint64_t b, uint64_t &carry)
	{
		u128 r = (u128)a + b + carry;
		carry = (uint64_t)(r >> 64);
		return (uint64_t)r;
	}
	// Fallback generic (slow): kept for setup (e.g., computing R^2 mod N once)
	u128 mult128(u128 a, u128 b, u128 mod)
	{
		if ((a | b) >> 64 == 0)
			return a * b % mod;
		u128 result = 0;
		for (a %= mod; b; b >>= 1)
		{
			result += a & -(u128)(b & 1);
			result -= mod & -(result >= mod);
			a += a;
			a -= mod & -(a >= mod);
		}
		return result;
	}
	// ---------- Montgomery (CIOS, 2x64-bit limbs) ----------
	// Compute n0' = -N^{-1} mod 2^64 (low limb only)
	static inline uint64_t inv64_2k(uint64_t n0)
	{
		uint64_t x = 1; // initial approx
		for (int i = 6; i > 0; --i)
			x = (__uint128_t)x * (2 - (__uint128_t)n0 * x);
		return x; // x ≡ n0^{-1}
	}

	inline pair<u128, u128> montModInv(u128 N) { return {0, (u128)(0 - inv64_2k(N))}; }

	// Fast Montgomery multiplication: returns a*b*R^{-1} mod N, where R=2^128
	inline u128 montMult(u128 a, u128 b, u128 N, u128 N_n0prime)
	{
		uint64_t n0 = (uint64_t)N, n1 = (uint64_t)(N >> 64);
		uint64_t a0 = (uint64_t)a, a1 = (uint64_t)(a >> 64);
		uint64_t b0 = (uint64_t)b, b1 = (uint64_t)(b >> 64);
		uint64_t n0p = (uint64_t)N_n0prime;
		uint64_t t0 = 0, t1 = 0, t2 = 0, t3 = 0;

		auto roundStep = [&](uint64_t ai) __attribute__((always_inline))
		{
			uint64_t lo, hi, carry = 0;
			// t += ai * b
			mult64(ai, b0, lo, hi);
			t0 = add64(t0, lo, carry);
			t1 = add64(t1, hi, carry);
			t2 = add64(t2, 0, carry);
			mult64(ai, b1, lo, hi);
			carry = 0;
			t1 = add64(t1, lo, carry);
			t2 = add64(t2, hi, carry);
			t3 = add64(t3, 0, carry);
			// m = t0 * n0' (mod 2^64)
			uint64_t m = (uint64_t)((__uint128_t)t0 * n0p);
			// t += m * N
			mult64(m, n0, lo, hi);
			carry = (t0 != 0); // Mathematically exact replacement for t0 = add64(t0, lo, carry);
			t1 = add64(t1, hi, carry);
			t2 = add64(t2, 0, carry);
			t3 = add64(t3, 0, carry);

			mult64(m, n1, lo, hi);
			carry = 0;
			t1 = add64(t1, lo, carry);
			t2 = add64(t2, hi, carry);
			t3 = add64(t3, 0, carry);

			// shift by one limb
			t0 = t1;
			t1 = t2;
			t2 = t3;
			t3 = 0;
		};

		roundStep(a0);
		roundStep(a1);

		__uint128_t res = (((__uint128_t)t1 << 64) | t0);
		if (t2 || res >= N)
			res -= N;
		return (u128)res;
	}
}
using namespace Montgomery128;

template <typename T>
inline T absVal(T N) { return N < 0 ? -N : N; }

constexpr inline uint32_t bitWidth128(u128 n)
{
	if (!n)
		return 0;
	uint64_t hi = (uint64_t)(n >> 64);
	uint64_t lo = (uint64_t)n;
	return hi ? 128 - countl_zero(hi) : 64 - countl_zero(lo);
}

constexpr inline int ctz128(u128 x)
{
	if (!x)
		return 128;
	uint64_t hi = (uint64_t)(x >> 64);
	uint64_t lo = (uint64_t)x;
	return lo ? countr_zero(lo) : 64 + countr_zero(hi);
}

template <typename T>
T GCD(T a, T b)
{
	if (!a || !b)
		return a | b;
	int shift = ctz128(a | b);
	a >>= ctz128(a);
	do
	{
		b >>= ctz128(b);
		if (a > b)
			swap(a, b);
		b -= a;
	} while (b);
	return a << shift;
}

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
	u128 inv = montModInv(N).second;
	T L = 1;
	while (gcdVal == 1)
	{
		Xt = X;
		for (size_t i = 1; i < L; ++i)
			X = F(X, C, N, inv);

		uint64_t k = 0;
		while (k < L && gcdVal == 1)
		{
			Xs = X;
			for (size_t i = 0; i < m && i + k < L; ++i)
			{
				X = F(X, C, N, inv);
				q = montMult(q, Xt > X ? Xt - X : X - Xt, N, inv);
			}
			gcdVal = GCD(q, N);
			k += m;
		}
		L += L;
	}
	if (gcdVal == N) // Failure
	{
		do
		{
			Xs = F(Xs, C, N, inv);
			gcdVal = GCD(Xs > Xt ? Xs - Xt : Xt - Xs, N);
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
			res = mult128(res, N, mod);
		N = mult128(N, N, mod);
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
			p = mult128(p, p, N);
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

template <typename T>
T countDivisors(T N)
{
	vector<T> primeFactors;
	primeFactorize(N, primeFactors);
	sort(primeFactors.begin(), primeFactors.end());

	T ans = 1;
	for (size_t i = 0, sz = primeFactors.size(); i < sz;)
	{
		size_t j = i;
		while (j < sz && primeFactors[i] == primeFactors[j])
			++j;
		ans *= (j - i + 1);
		i = j;
	}
	return ans;
}

template <typename T>
T binExp(T N, T power)
{
	if (N == 1 || power == 0)
		return 1;

	T res{1};
	while (power)
	{
		if (power & 1) // ODD
			res *= N;

		N *= N;
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
	u128 a, b, c;
	// cin >> t;
	while (t--)
	{
		cin >> a >> b >> c;
		u128 x = a + c;
		u128 y = binExp(x, b - 1);
		u128 N = binExp(x, b) * (y - x + 2) / 2;
		cout << countDivisors(N) << endl;
	}
	return 0;
}