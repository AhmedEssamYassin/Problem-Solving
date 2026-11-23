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
		array<vector<ll>, 4> clothes;
		for (int k{}; k < 4; k++)
		{
			int sz;
			cin >> sz;
			clothes[k].resize(sz);
			for (int i{}; i < sz; i++)
				cin >> clothes[k][i];
			sort(clothes[k].begin(), clothes[k].end());
		}

		array<int, 4> idx = {0, 0, 0, 0};
		array<ll, 4> ans = {0, 0, 0, 0};
		ll bestDiff = LLONG_MAX;

		while (idx[0] < (int)clothes[0].size() && idx[1] < (int)clothes[1].size() && idx[2] < (int)clothes[2].size() && idx[3] < (int)clothes[3].size())
		{
			int minK = 0;
			ll curMin = clothes[0][idx[0]], curMax = clothes[0][idx[0]];
			for (int k = 1; k < 4; k++)
			{
				ll val = clothes[k][idx[k]];
				if (val < curMin)
				{
					curMin = val;
					minK = k;
				}
				curMax = max(curMax, val);
			}
			if (curMax - curMin < bestDiff)
			{
				bestDiff = curMax - curMin;
				for (int k{}; k < 4; k++)
					ans[k] = clothes[k][idx[k]];
			}
			idx[minK]++;
		}

		cout << ans[0] << " " << ans[1] << " " << ans[2] << " " << ans[3];
	}
	return 0;
}
