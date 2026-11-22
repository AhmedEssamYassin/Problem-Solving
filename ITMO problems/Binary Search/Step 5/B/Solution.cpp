#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

ll countLE(ll n, ll x)
{
	ll cnt = 0;
	for (ll i = 1; i <= n; i++)
		cnt += min(n, x / i);
	return cnt;
}

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
		ll n, k;
		cin >> n >> k;
		ll L = 1, R = n * n, ans = R;
		while (L <= R)
		{
			ll mid = L + (R - L) / 2;
			if (countLE(n, mid) >= k)
			{
				ans = mid;
				R = mid - 1;
			}
			else
				L = mid + 1;
		}
		cout << ans;
	}
	return 0;
}
