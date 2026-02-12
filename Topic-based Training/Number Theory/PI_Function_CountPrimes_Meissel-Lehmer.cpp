#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

// Meissel-Lehmer algorithm
struct MeisselLehmer
{
	static const int MAX_N = 20000010;
	static const int MAX_PRIMES = 1300000;
	static const int PHI_N = 100000;
	static const int PHI_K = 100;

	int idx;
	int primes[MAX_PRIMES];
	int primeCount[MAX_N];
	int phiMemo[PHI_N][PHI_K];
	bitset<MAX_N> isComposite;

	MeisselLehmer() { sieve(); }

	void sieve()
	{
		idx = 0;
		isComposite[1] = 1;
		for (int i = 4; i < MAX_N; i += 2)
			isComposite[i] = 1;
		for (int i = 3; i * i < MAX_N; i += 2)
		{
			if (!isComposite[i])
			{
				for (int j = i * i; j < MAX_N; j += (i << 1))
					isComposite[j] = 1;
			}
		}
		for (int i = 1; i < MAX_N; i++)
		{
			if (!isComposite[i])
				primes[idx++] = i;
			primeCount[i] = idx;
		}
		for (int n = 0; n < PHI_N; n++)
			phiMemo[n][0] = n;
		for (int k = 1; k < PHI_K; k++)
		{
			for (int n = 0; n < PHI_N; n++)
				phiMemo[n][k] = phiMemo[n][k - 1] - phiMemo[n / primes[k - 1]][k - 1];
		}
	}

	ll phi(ll n, int k)
	{
		if (n < PHI_N && k < PHI_K)
			return phiMemo[n][k];
		if (k == 1)
			return ((++n) >> 1);
		if (primes[k - 1] >= n)
			return 1;
		return phi(n, k - 1) - phi(n / primes[k - 1], k - 1);
	}

	ll countPrimes(ll n)
	{
		if (n < MAX_N)
			return primeCount[n];
		int sqrtN = sqrt(n);
		int cbrtPi = countPrimes(cbrt(n));
		int sqrtSqrtPi = countPrimes(sqrt(sqrtN));
		int sqrtPi = countPrimes(sqrtN);
		ll res = phi(n, sqrtSqrtPi) + ((1LL * (sqrtPi + sqrtSqrtPi - 2) * (sqrtPi - sqrtSqrtPi + 1)) >> 1);
		for (int i = sqrtSqrtPi; i < sqrtPi; i++)
		{
			ll w = n / primes[i];
			int wSqrtPi = countPrimes(sqrt(w));
			res -= countPrimes(w);
			if (i <= cbrtPi)
			{
				for (int j = i; j < wSqrtPi; j++)
				{
					res += j;
					res -= countPrimes(w / primes[j]);
				}
			}
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
		for (const ll &p : ML.primes)
		{
			if (p * p * p > N)
				break;
			cnt++;
		}
		for (const ll &p : ML.primes)
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