#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

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
		/*
		If a number N is a square of a prime, i.e., N = P²
		N will have ONLY 3 distinct divisors: 1, P, P²
		So we need to check if that number N satisfy two conditions:
		1. N is a perfect square
		2. sqrt(N) is a prime number
		*/
		cin >> N;
		ll Sqrt_N = sqrt(N);
		if (Sqrt_N * Sqrt_N == N && isPrime[Sqrt_N])
			cout << "YES\n";
		else
			cout << "NO\n";
	}
	return 0;
}
