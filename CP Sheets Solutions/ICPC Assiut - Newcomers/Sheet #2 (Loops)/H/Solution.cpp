#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl "\n"

/*
vector<int> primes, LPF;
bitset<1000001> isPrime;

void linearSieveOfEratosthenes(int N)
{
	isPrime.set(); // Initially Assuming all numbers to be primes
	LPF.resize(N + 1);
	isPrime[0] = isPrime[1] = 0; // 0 and 1 are NOT primes
	for (long long i{2}; i <= N; i++)
	{
		if (isPrime[i])
		{
			primes.push_back(i);
			LPF[i] = i; // The least prime factor of a prime number is itself
		}
		for (long long j{}; j < (int)primes.size() && i * primes[j] <= N && primes[j] <= LPF[i]; j++)
		{
			isPrime[i * primes[j]] = 0; // Crossing out all the multiples of prime numbers
			LPF[i * primes[j]] = primes[j];
		}
	}
}
static int autoCall = (linearSieveOfEratosthenes(1e6), 0);
*/

template <typename T>
T mult64(const T &a, const T &b, T mod)
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

template <typename T>
bool isPrime(T N)
{
	if (N < 2 || N % 6 % 4 != 1)
		return (N | 1) == 3;
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

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
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
		if (isPrime(N))
			cout << "YES\n";
		else
			cout << "NO\n";
	}
	return 0;
}