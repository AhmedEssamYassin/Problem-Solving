#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define u128 __uint128_t
#define endl "\n"

namespace bigNumber
{
	using u128 = __uint128_t;

	// ---------- 64-bit limb helpers (fast) ----------
	static inline void mult64(uint64_t a, uint64_t b, uint64_t &lo, uint64_t &hi)
	{
		__uint128_t p = (__uint128_t)a * b;
		lo = (uint64_t)p;
		hi = (uint64_t)(p >> 64);
	}
	static inline uint64_t add64(uint64_t a, uint64_t b, uint64_t &carry)
	{
		__uint128_t s = (__uint128_t)a + b + carry;
		carry = (uint64_t)(s >> 64);
		return (uint64_t)s;
	}
	// Fallback generic (slow): kept for setup (e.g., computing R^2 mod N once)
	u128 mult128(u128 a, u128 b, u128 mod)
	{
		u128 result = 0;
		for (a %= mod; b > 0; a <<= 1, b >>= 1)
		{
			a >= mod ? a -= mod : 0;
			if (b & 1)
				result += a, result >= mod ? result -= mod : 0;
		}
		return result;
	}
	// ---------- Montgomery (CIOS, 2x64-bit limbs) ----------
	// Compute n0' = -N^{-1} mod 2^64 (low limb only)
	static inline uint64_t inv64_2k(uint64_t n0)
	{
		uint64_t x = 1; // initial approx
		for (int i = 6; i > 0; i--)
			x = (__uint128_t)x * (2 - (__uint128_t)n0 * x);
		return x; // x ≡ n0^{-1}
	}

	inline pair<u128, u128> montModInv(u128 N)
	{
		return {0, (u128)(0 - inv64_2k(N))};
	}

	// Fast Montgomery multiplication: returns a*b*R^{-1} mod N, where R=2^128
	inline u128 montMult(u128 a, u128 b, u128 N, u128 N_n0prime)
	{
		uint64_t n0 = (uint64_t)N, n1 = (uint64_t)(N >> 64);
		uint64_t a0 = (uint64_t)a, a1 = (uint64_t)(a >> 64);
		uint64_t b0 = (uint64_t)b, b1 = (uint64_t)(b >> 64);
		uint64_t n0p = (uint64_t)N_n0prime;
		uint64_t t0 = 0, t1 = 0, t2 = 0;

		auto round_step = [&](uint64_t ai)
		{
			uint64_t lo, hi, carry;
			// t += ai * b
			mult64(ai, b0, lo, hi);
			carry = 0;
			t0 = add64(t0, lo, carry);
			t1 = add64(t1, hi, carry);
			t2 = add64(t2, 0, carry);
			mult64(ai, b1, lo, hi);
			carry = 0;
			t1 = add64(t1, lo, carry);
			t2 = add64(t2, hi, carry);
			// m = t0 * n0' (mod 2^64)
			uint64_t m = (uint64_t)((__uint128_t)t0 * n0p);
			// t += m * N
			mult64(m, n0, lo, hi);
			carry = 0;
			t0 = add64(t0, lo, carry);
			t1 = add64(t1, hi, carry);
			t2 = add64(t2, 0, carry);
			mult64(m, n1, lo, hi);
			carry = 0;
			t1 = add64(t1, lo, carry);
			t2 = add64(t2, hi, carry);
			// shift by one limb
			t0 = t1;
			t1 = t2;
			t2 = 0;
		};

		round_step(a0);
		round_step(a1);

		__uint128_t res = (((__uint128_t)t1 << 64) | t0);
		if (res >= N)
			res -= N;
		return (u128)res;
	}
}
using namespace bigNumber;

template <typename T>
inline T F(T x, T c, T mod, T inv) // Pollard-rho function
{
	x = montMult(x, x, mod, inv);
	x = x >= mod - c ? x - mod + c : x + c;
	return x;
}

template <typename T>
inline T absVal(T N)
{
	if (N < 0)
		return -N;

	return N;
}

template <typename T>
T Pollard_Brent(T N)
{
	if (!(N & 1))
		return 2;

	// Random Number Linear Congruential Generator MMIX from D.E. Knuth
	static u128 rng = 0xdeafbeefff;
	uint64_t a = rng * 6364136223846793005ull + 1442695040888963407ull;
	uint64_t b = a * 6364136223846793005ull + 1442695040888963407ull;
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
		for (size_t i = 1; i < L; i++)
			X = F(X, C, N, inv);

		int k = 0;
		while (k < L && gcdVal == 1)
		{
			Xs = X;
			for (size_t i = 0; i < m && i < L - k; i++)
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
void primeFactorize(T N, map<T, T> &primeFactors)
{
	if (N == 1)
		return;

	if (!isPrime(N))
	{
		T Y = Pollard_Brent(N);
		primeFactorize(Y, primeFactors);
		primeFactorize(N / Y, primeFactors);
	}
	else
	{
		primeFactors[N]++;
		return;
	}
}

template <typename T>
void getAllFactors(T N, vector<T> &factors)
{
	factors = {1};
	map<T, T> freq;
	primeFactorize(N, freq);

	for (auto &[p, cnt] : freq)
	{
		vector<T> temp;
		T pw = 1;
		for (int i = 0; i <= cnt; i++, pw *= p)
		{
			for (const T &f : factors)
				temp.push_back(f * pw);
		}
		factors.swap(temp);
	}
	sort(factors.begin(), factors.end());
}

template <typename T>
T countDivisors(T N)
{
	map<T, T> primeFactors;
	primeFactorize(N, primeFactors);
	T ans{1};
	for (const auto &[p, exp] : primeFactors)
		ans *= (exp + 1);
	return ans;
}

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
	return stream;
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
	u128 N, a, b, c;
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