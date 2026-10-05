#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Meissel-Lehmer algorithm: PI(n) for n up to ~1e14
#pragma GCC target("popcnt")
struct MeisselLehmer
{
	static const int N = 50000010, M = 100000, K = 100, W = N / 128 + 2, PL = 700000; // PL: primes kept, up to ~1e7
	int pr[PL], cnt[W];                                                               // primes; cnt[w] = number of odd primes below word w
	double ip[PL];                                                                    // ip[k] ~ 1 / pr[k], multiplying is much cheaper than a 64-bit division
	uint64_t bits[W];                                                                 // bit t <=> 2t + 1 is prime
	int memo[M][K];                                                                   // memo[n][k] = phi(n, k): numbers in [1, n] not divisible by any of the first k primes

	MeisselLehmer()
	{
		fill(bits, bits + W, ~0ULL), bits[0] ^= 1; // 1 is not prime
		for (ll i = 3; i * i < N; i += 2)
		{
			if (bits[i >> 7] >> (i >> 1 & 63) & 1)
				for (ll j = i * i; j < N; j += i << 1)
					bits[j >> 7] &= ~(1ULL << (j >> 1 & 63));
		}
		for (int w = 0, s = 0; w < W; s += __builtin_popcountll(bits[w++]))
			cnt[w] = s;
		pr[0] = 2;
		for (int i = 3, k = 1; k < PL && i < N; i += 2)
			if (bits[i >> 7] >> (i >> 1 & 63) & 1)
				pr[k++] = i;
		for (int k = 0; k < PL; k++)
			ip[k] = (1 + 1e-15) / pr[k];
		for (int n = 0; n < M; n++)
			for (int k = 0; k < K; k++)
				memo[n][k] = k ? memo[n][k - 1] - memo[(int)(n * ip[k - 1])][k - 1] : n;
	}

	int pi(ll x) const // x < N
	{
		ll t = x - 1 >> 1;
		return x < 2 ? 0 : 1 + cnt[t >> 6] + __builtin_popcountll(bits[t >> 6] & ~0ULL >> (63 - (t & 63)));
	}

	ll phi(ll n, int k)
	{
		int s = n < M ? min(k, K - 1) : 1; // start from the memo (or from phi(n, 1)) and unroll the rest
		ll r = n < M ? memo[n][s] : n + 1 >> 1;
		for (int i = s + 1; i <= k && pr[i - 1] <= n; i++)
			r -= phi(n * ip[i - 1], i - 1); // n * ip[k] = floor(n / pr[k]) for n < 1e15
		return r;
	}

	ll countPrimes(ll n)
	{
		if (n < N)
			return pi(n);
		ll s = sqrt(n), a = countPrimes(sqrt(s)), b = countPrimes(s);
		ll res = phi(n, a) + (b + a - 2) * (b - a + 1) / 2;
		for (int i = a; i < b; i++)
		{
			ll p = pr[i], w = n / p;
			res -= countPrimes(w);
			if (p * p <= w)
				for (int j = i, e = countPrimes(sqrt(w)); j < e; j++)
					res += j - pi(w * ip[j]);
		}
		return res;
	}
} ML;

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
		ll N;
		cin >> N;
		ll cnt{};
		for (const ll &p : ML.pr)
		{
			if (p * p * p > N)
				break;
			cnt++;
		}
		for (const ll &p : ML.pr)
		{
			if (p * p > N)
				break;
			cnt += ML.countPrimes(N / p) - 1; // Excluding p
		}
		ll sqrtN = sqrtl(N);
		ll val = ML.countPrimes(sqrtN);
		ll sub = val * (val - 1) / 2;
		cout << cnt - sub;
	}
	return 0;
}