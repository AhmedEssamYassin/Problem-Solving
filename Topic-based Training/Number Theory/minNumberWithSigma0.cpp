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
		int n;
		cin >> n;
		vector<int> primes({2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53});
		ll res = 4e18, INF = 4e18;
		function<void(int, ll, ll, int)> dfs = [&](int i, ll cnt, ll cur, int lim) -> void
		{
			if (cur >= res)
				return;
			if (cnt == n)
			{
				res = min(res, cur);
				return;
			}
			ll mul = 1;
			for (int j = 1; j <= lim; j++)
			{
				if (INF / primes[i] < cur * mul)
					break;
				mul *= primes[i];
				if (n % (cnt * (j + 1)) == 0)
					dfs(i + 1, cnt * (j + 1), cur * mul, j);
			}
		};
		dfs(0, 1, 1, 60);
		cout << res;
	}
	return 0;
}
