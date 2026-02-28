#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

vector<int> primes, LPF;
bitset<10000001> isPrime;

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
static int autoCall = (linearSieveOfEratosthenes(1e7), 0);

ll G[10000001];

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
	int t = 1;
	ll N, K;
	cin >> t;
	G[1] = 1;
	for (int i = 2, val = 2; i < 10000001; i++)
	{
		if (i & 1)
		{
			if (isPrime[i])
				G[i] = val++;
			else
				G[i] = G[LPF[i]];
		}
	}
	while (t--)
	{
		cin >> N;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];
		ll XOR{};
		for (const ll &x : vc)
		{
			if (x & 1)
				XOR ^= G[x];
		}

		if (XOR)
			cout << "Alice\n";
		else
			cout << "Bob\n";
	}
	return 0;
}