#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

ll countLE(const vector<pair<ll, ll>> &segs, ll x)
{
	ll cnt = 0;
	for (const auto &[l, r] : segs)
	{
		if (x >= l)
			cnt += min(x, r) - l + 1;
	}
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
		vector<pair<ll, ll>> segs(n);
		ll L = LLONG_MAX, R = LLONG_MIN;
		for (int i{}; i < n; i++)
		{
			cin >> segs[i].first >> segs[i].second;
			L = min(L, segs[i].first);
			R = max(R, segs[i].second);
		}
		ll ans = R;
		while (L <= R)
		{
			ll mid = L + (R - L) / 2;
			if (countLE(segs, mid) >= k + 1)
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
