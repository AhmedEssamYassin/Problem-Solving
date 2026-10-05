#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

template <typename T>
inline T mult64(const T &a, const T &b, T mod)
{
	return (__int128_t)a * b % mod;
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

// composite: bit x is set if x is NOT prime (12.5 MB for 1e8)
// blockCnt[w]: number of primes in [0, 64 * w) (6.25 MB for 1e8)
vector<int> primes;
vector<uint64_t> composite;
vector<uint32_t> blockCnt;

inline bool isPrime(ll x) { return !((composite[x >> 6] >> (x & 63)) & 1); }

// Number of primes <= x, for 0 <= x <= sieve limit
inline ll cntPrimes(ll x)
{
	ll w = x >> 6;
	int b = x & 63;
	uint64_t mask = (b == 63) ? ~0ULL : ((1ULL << (b + 1)) - 1);
	return blockCnt[w] + __builtin_popcountll(~composite[w] & mask);
}

void linearSieveOfEratosthenes(int N)
{
	int W = (N >> 6) + 1;
	composite.assign(W, 0);
	blockCnt.assign(W + 1, 0);
	composite[0] |= 3; // 0 and 1 are NOT primes
	primes.reserve(N / 15 + 100);
	for (long long i{2}; i <= N; i++)
	{
		if (isPrime(i))
			primes.push_back(i);
		for (size_t j{}; j < primes.size() && i * primes[j] <= N; j++)
		{
			ll v = i * primes[j];
			composite[v >> 6] |= 1ULL << (v & 63); // Crossing out all the multiples of prime numbers
			if (i % primes[j] == 0)
				break;
		}
	}
	for (ll v = N + 1; v < 64LL * W; v++) // Bits past N are not primes
		composite[v >> 6] |= 1ULL << (v & 63);
	for (int w = 0; w < W; w++)
		blockCnt[w + 1] = blockCnt[w] + __builtin_popcountll(~composite[w]);
}
static int autoCall = (linearSieveOfEratosthenes(1e8), 0);

// Calculating the exponent of a prime `p` in N! (Legendre's Formula)
int sumOfBin(ll N, int base)
{
	int res{};
	while (N != 0)
	{
		res += (N % base);
		N /= base;
	}
	return res;
}

int expFactor(ll N, int p)
{
	// ll exponent = (N - sumOfBin(N, p)) / (p - 1);
	ll exponent = 0;
	while ((N /= p) != 0)
		exponent += N;
	return exponent;
}

ll countDivisors(ll N, ll mod)
{
	ll cnt = 1;
	ll sqrtN = sqrt(N);

	// Handle primes <= sqrt(N) using Legendre's formula
	for (const int &p : primes)
	{
		if (p > sqrtN)
			break;
		cnt = mult64<ll>(cnt, expFactor(N, p) + 1, mod);
	}

	// Group primes with same N / p
	ll i = cntPrimes(sqrtN);
	while (i < (ll)primes.size())
	{
		ll L = i;
		ll Q = N / primes[L];
		if (Q == 0)
			break;
		ll R = cntPrimes(N / Q);
		cnt = mult64(cnt, modPow(Q + 1, R - L, mod), mod);
		i = R;
	}

	return cnt;
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
	ll N, M;
	cin >> t;
	while (t--)
	{
		cin >> N >> M;
		cout << countDivisors(N, M) << endl;
	}
	return 0;
}