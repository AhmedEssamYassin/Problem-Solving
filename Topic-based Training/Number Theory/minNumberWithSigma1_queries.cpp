#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

vector<int> primes, SPPF, sum, ans; // SPPF[k]: largest power of k's smallest prime dividing k
void linearSieve(int N)
{
	SPPF.assign(N + 1, 0), sum.assign(N + 1, 0), ans.assign(N + 1, -1);
	sum[1] = 1;
	for (int i = 2; i <= N; i++)
	{
		if (!SPPF[i])
			primes.push_back(i), SPPF[i] = i, sum[i] = i + 1;
		for (const int &p : primes)
		{
			ll k = (ll)i * p;
			if (k > N)
				break;
			if (i % p == 0) // p is the smallest prime of i
			{
				SPPF[k] = SPPF[i] * p;
				sum[k] = SPPF[k] == k ? sum[i] * p + 1 : sum[i / SPPF[i]] * sum[SPPF[k]];
				break;
			}
			SPPF[k] = p, sum[k] = sum[i] * (p + 1);
		}
	}
	for (int i = N; i > 0; i--)
		if (sum[i] <= N)
			ans[sum[i]] = i;
}
static int autoCall = (linearSieve(10000000), 0);

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif
	int t = 1;
	cin >> t;
	while (t--)
	{
		int c;
		cin >> c;
		cout << ans[c] << endl;
	}
	return 0;
}
