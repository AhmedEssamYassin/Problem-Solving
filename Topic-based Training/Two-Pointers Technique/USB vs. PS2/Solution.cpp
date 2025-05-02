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
	ll N;
	// cin >> t;
	while (t--)
	{
		ll a, b, c;
		cin >> a >> b >> c;
		ll m;
		cin >> m;
		vector<vector<ll>> vc(2); // 0: USB, 1: PS/2
		for (int i{}; i < m; i++)
		{
			ll price;
			string type;
			cin >> price >> type;
			vc[type == "USB" ? 0 : 1].push_back(price);
		}
		sort(vc[0].begin(), vc[0].end());
		sort(vc[1].begin(), vc[1].end());
		ll cnt{}, sum{}, i{}, j{};
		while (a-- > 0 && i < vc[0].size())
			cnt++, sum += vc[0][i++];

		while (b-- > 0 && j < vc[1].size())
			cnt++, sum += vc[1][j++];

		// Like merging sort
		while (c > 0)
		{
			if (i < vc[0].size() && j < vc[1].size())
			{
				if (vc[0][i] < vc[1][j])
					sum += vc[0][i++];
				else
					sum += vc[1][j++];
			}
			else if (i < vc[0].size())
				sum += vc[0][i++];
			else if (j < vc[1].size())
				sum += vc[1][j++];
			else
				break;
			cnt++;
			c--;
		}
		cout << cnt << " " << sum;
	}
	return 0;
}