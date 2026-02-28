#include <bits/stdc++.h>
using namespace std;
#define ll long long
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

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
	ll N;
	cin >> N;
	linearSieveOfEratosthenes(N);
	for (int i{}; i < primes.size(); i++)
		cout << primes[i] << " ";
	return 0;
}