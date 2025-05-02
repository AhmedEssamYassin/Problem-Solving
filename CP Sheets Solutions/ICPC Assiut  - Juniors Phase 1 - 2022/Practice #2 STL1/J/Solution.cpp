#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll N, M;
	// cin >> t;
	while (t--)
	{
		cin >> N >> M;
		vector<pair<ll, ll>> vc;
		vector<ll> ans;
		for (int i{}; i < N; i++)
		{
			ll u, v;
			cin >> u >> v;
			vc.push_back({u, v});
		}
		sort(vc.begin(), vc.end());
		ll prev = 0;
		for (int i{1}; i < N; i++)
		{
			if (vc[i].first <= vc[prev].second) // Intersection
			{
				vc[prev].second = max(vc[i].second, vc[prev].second);
				// Marking current interval as included
				vc[i].first = 0;
				vc[i].second = 0;
			}
			else // No intersection, start a new interval
				prev = i;
		}

		for (const auto &[u, v] : vc)
		{
			// Indices are 0, 1, 2, ... ans.size() - 1
			// Even positions: 0, 2, 4, ... will represent Start of ranges
			// Odd positions: 1, 3, 5, ... will represent End of ranges
			if (u != 0 && v != 0)
			{
				ans.push_back(u);
				ans.push_back(v);
			}
		}

		ll q;
		cin >> q;
		while (q--)
		{
			ll x, y;
			cin >> x >> y;
			if (x > y)
				swap(x, y); // Make sure x <= y
			ll a = lower_bound(ans.begin(), ans.end(), x) - ans.begin();
			ll b = lower_bound(ans.begin(), ans.end(), y) - ans.begin();
			if ((b - a == 1 && x >= ans[a] && y <= ans[b]) || (b - a == 0 && b % 2 == 1))
				cout << "YES" << endl;
			else
				cout << "NO" << endl;
		}
	}
	return 0;
}