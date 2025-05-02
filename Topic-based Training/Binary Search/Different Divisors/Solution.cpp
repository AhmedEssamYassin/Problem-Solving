#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl "\n"

vector<ll> Prime, LPF;
bitset<100001> isPrime;

void Linear_Sieve_Of_Eratosthenes(int N)
{
	isPrime.set(); // Initially Assuming all numbers to be primes
	LPF.resize(N + 1);
	isPrime[0] = isPrime[1] = 0; // 0 and 1 are NOT primes
	for (int i{2}; i <= N; i++)
	{
		if (isPrime[i])
		{
			Prime.push_back(i);
			LPF[i] = i; // The least prime factor of a prime number is itself
		}
		for (int j{}; j < (int)Prime.size() and i * Prime[j] <= N and Prime[j] <= LPF[i]; j++)
		{
			isPrime[i * Prime[j]] = 0; // Crossing out all the multiples of prime numbers
			LPF[i * Prime[j]] = Prime[j];
		}
	}
}

static int autoCall = (Linear_Sieve_Of_Eratosthenes(100000), 0);

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	int N;
	cin >> t;
	while (t--)
	{
		cin >> N;
		int P0, P1;

		P0 = *upper_bound(Prime.begin(), Prime.end(), N);
		P1 = *lower_bound(Prime.begin(), Prime.end(), P0 + N);

		cout << 1ll * P0 * P1 << "\n";
	}
	return 0;
}