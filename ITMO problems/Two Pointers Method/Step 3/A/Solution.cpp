#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

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
		ll n, p;
		cin >> n >> p;
		vector<ll> a(n);
		ll tot = 0;
		for (int i{}; i < n; i++)
			cin >> a[i], tot += a[i];

		ll fullLoops = p / tot;
		ll rem = p % tot;
		if (rem == 0)
		{
			cout << 1 << " " << fullLoops * n;
			return 0;
		}

		int bestLen = n + 1, bestStart = 1;
		ll curSum = 0;
		int L = 0;
		for (int R{}; R < 2 * n; R++)
		{
			curSum += a[R % n];
			while (L <= R && curSum >= rem)
			{
				if (L < n && R - L + 1 < bestLen)
				{
					bestLen = R - L + 1;
					bestStart = L + 1;
				}
				curSum -= a[L % n];
				L++;
			}
		}
		cout << bestStart << " " << fullLoops * n + bestLen;
	}
	return 0;
}
