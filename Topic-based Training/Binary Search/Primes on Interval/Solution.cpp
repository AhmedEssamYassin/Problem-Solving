#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

int A, B, K;
vector<int> cntPrimes(1000001, 0);
bitset<1000001> isPrime;
void SieveOfEratosthenes(int N)
{
	isPrime.set();
	isPrime[0] = 0;
	isPrime[1] = 0;
	for (ll i{2}; i * i <= N; i++)
	{
		if (isPrime[i])
		{
			for (ll j = 2 * i; j <= N; j += i)
				isPrime[j] = 0;
		}
	}
}

static int autoCall = (SieveOfEratosthenes(1000000), 0);
bool isOk(int X)
{
	for (int i{A}; i + X - 1 <= B; i++)
	{
		int r = i + X - 1;
		if (cntPrimes[r] - cntPrimes[i - 1] < K)
			return false;
	}
	return true;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll N;
	for (int i{1}; i < 1000007; i++)
		cntPrimes[i] = cntPrimes[i - 1] + isPrime[i];
	// cin >> t;
	while (t--)
	{
		cin >> A >> B >> K;

		// from 1 to B - A + 1 and have K primes
		int L{1}, R = B - A + 1, ans{INT_MAX};
		while (L <= R)
		{
			int mid = L + (R - L) / 2;
			if (isOk(mid))
			{
				ans = min(ans, mid);
				R = mid - 1;
			}
			else
				L = mid + 1;
		}
		if (ans == INT_MAX)
			cout << -1;
		else
			cout << ans;
	}
	return 0;
}