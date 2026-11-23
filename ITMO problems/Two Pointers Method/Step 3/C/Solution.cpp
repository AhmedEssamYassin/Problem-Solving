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
		ll n, r;
		cin >> n >> r;
		vector<ll> d(n);
		for (int i{}; i < n; i++)
			cin >> d[i];

		ll ans = 0, L = 0;
		for (int R{}; R < n; R++)
		{
			while (L < R && d[R] - d[L] > r)
				L++;
			ans += L;
		}
		cout << ans;
	}
	return 0;
}
