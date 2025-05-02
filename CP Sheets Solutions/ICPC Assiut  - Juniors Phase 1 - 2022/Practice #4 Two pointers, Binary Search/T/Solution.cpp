#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

struct segment
{
	int left, right;
	bool operator<(const segment &other) const
	{
		return right < other.right;
	}
};

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("Output.txt", "w", stdout);
#endif //! ONLINE_JUDGE
	int t = 1;
	ll N, k, a;
	// cin >> t;
	while (t--)
	{
		cin >> N >> k >> a;
		set<segment> ranges{{1, int(N)}};

		// The ships cannot intersect and even touch each other.
		auto countShips = [&](int l, int r)
		{
			return (r - l + 1 + 1) / (a + 1);
		};

		ll cnt = countShips(1, N);
		int ans = -1;
		int m;
		cin >> m;
		for (int i{1}; i <= m; i++)
		{
			int x;
			cin >> x;
			segment key = {x, x};
			auto it = ranges.lower_bound(key);
			if (it != ranges.end() && it->left <= x && x <= it->right)
			{
				int L = it->left, R = it->right;
				ranges.erase(it);
				cnt -= countShips(L, R);
				if (L <= x - 1)
				{
					ranges.insert({L, x - 1});
					cnt += countShips(L, x - 1);
				}
				if (x + 1 <= R)
				{
					ranges.insert({x + 1, R});
					cnt += countShips(x + 1, R);
				}
				if (cnt < k && !~ans)
					ans = i;
			}
		}
		cout << ans;
	}
	return 0;
}