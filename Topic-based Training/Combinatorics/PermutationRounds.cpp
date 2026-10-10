#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

constexpr int mod = 1e9 + 7;
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

vector<int> primes, LPF;
bitset<1000001> isPrime;

void linearSieveOfEratosthenes(int N)
{
	isPrime.set();
	LPF.resize(N + 1);
	isPrime[0] = isPrime[1] = 0;
	for (long long i{2}; i <= N; i++)
	{
		if (isPrime[i])
		{
			primes.push_back(i);
			LPF[i] = i;
		}
		for (long long j{}; j < (int)primes.size() && i * primes[j] <= N && primes[j] <= LPF[i]; j++)
		{
			isPrime[i * primes[j]] = 0;
			LPF[i * primes[j]] = primes[j];
		}
	}
}
static int autoCall = (linearSieveOfEratosthenes(1000000), 0);

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
		int n;
		cin >> n;
		vector<int> P(n + 1);
		for (int i = 1; i <= n; i++)
			cin >> P[i];

		vector<bool> vis(n + 1, false);
		vector<int> maxExp(n + 1, 0);

		for (int i = 1; i <= n; i++)
		{
			if (vis[i])
				continue;

			int curr = i;
			int len = 0;
			while (!vis[curr])
			{
				vis[curr] = true;
				curr = P[curr];
				len++;
			}

			int tmp = len;
			while (tmp > 1)
			{
				int pr = LPF[tmp];
				int cnt = 0;
				while (tmp % pr == 0)
				{
					cnt++;
					tmp /= pr;
				}
				maxExp[pr] = max(maxExp[pr], cnt);
			}
		}

		ll ans = 1;
		for (const int &p : primes)
		{
			if (p > n)
				break;
			ans = mult64<ll>(ans, modPow(p, maxExp[p], mod), mod);
		}

		cout << ans;
	}
	return 0;
}