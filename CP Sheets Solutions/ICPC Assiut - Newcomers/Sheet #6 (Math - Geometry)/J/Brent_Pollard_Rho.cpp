#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <typename T>
T mult64(T a, T b, T mod)
{
	return (__int128_t)a * b % mod;
}

template <typename T>
inline T F(T x, T c, T mod) // Pollard-rho function
{
	return (mult64(x, x, mod) + c) % mod;
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
	static uint64_t rng = 0xdeafbeefff;
	uint64_t a = rng * 6364136223846793005ull + 1442695040888963407ull;
	uint64_t b = a * 6364136223846793005ull + 1442695040888963407ull;
	rng = (a + b) ^ (a * b);

	T X0 = 1 + a % (N - 1);
	T C = 1 + b % (N - 1);
	T X = X0; // X1
	T gcd_val = 1;
	T q = 1;
	T Xs, Xt;
	T m = 128;
	T L = 1;
	while (gcd_val == 1)
	{
		Xt = X;
		for (size_t i = 1; i < L; i++)
			X = F(X, C, N);

		int k = 0;
		while (k < L && gcd_val == 1)
		{
			Xs = X;
			for (size_t i = 0; i < m && i < L - k; i++)
			{
				X = F(X, C, N);
				q = mult64(q, absVal(Xt - X), N);
			}
			gcd_val = __gcd(q, N);
			k += m;
		}
		L += L;
	}
	if (gcd_val == N) // Failure
	{
		do
		{
			Xs = F(Xs, C, N);
			gcd_val = __gcd(absVal(Xs - Xt), N);
		} while (gcd_val == 1);
	}
	return gcd_val;
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
void Factor(T N, map<T, int> &primeFactors)
{
	if (N == 1)
		return;

	if (!isPrime(N))
	{
		T Y = Pollard_Brent(N);
		Factor(Y, primeFactors);
		Factor(N / Y, primeFactors);
	}
	else
	{
		primeFactors[N]++;
		return;
	}
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
	ll N;
	// cin >> t;
	while (t--)
	{
		cin >> N;
		map<ll, int> primeFactors;
		Factor(N, primeFactors);

		bool first = true;
		for (const auto &[prime, power] : primeFactors)
		{
			if (!first)
				cout << "*";
			cout << "(" << prime << "^" << power << ")";
			first = false;
		}
	}
	return 0;
}