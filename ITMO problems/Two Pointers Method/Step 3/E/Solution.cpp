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
		ll n, s;
		cin >> n >> s;
		vector<ll> w(n), c(n);
		for (int i{}; i < n; i++)
			cin >> w[i];
		for (int i{}; i < n; i++)
			cin >> c[i];

		ll curW = 0, curC = 0, bestC = 0;
		int L = 0;
		for (int R{}; R < n; R++)
		{
			curW += w[R];
			curC += c[R];
			while (curW > s)
			{
				curW -= w[L];
				curC -= c[L];
				L++;
			}
			bestC = max(bestC, curC);
		}
		cout << bestC;
	}
	return 0;
}
