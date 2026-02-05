#include <bits/stdc++.h>
using namespace std;
#define ll long long int
// #define endl "\n"

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
		auto Ask = [&](int l, int r)
		{
			cout << "? " << l << " " << r;
			cout << endl;
			ll s;
			cin >> s;
			return s;
		};
		int n, q;
		cin >> n >> q;
		vector<ll> memo(4 * n + 1, -1);
		while (q--)
		{
			int k;
			cin >> k;
			function<int(int, int, int, int)> f = [&](int l, int r, int node, int k)
			{
				if (l == r)
					return l;
				int mid = ((l + r) >> 1);
				ll sumL = (~memo[node] ? memo[node] : memo[node] = Ask(l, mid));
				ll z = (mid - l + 1) - sumL;
				if (z >= k)
					return memo[node]++, f(l, mid, 2 * node + 1, k);
				else
					return f(mid + 1, r, 2 * node + 2, k - z);
			};
			int x = f(1, n, 0, k);
			cout << "! " << x << endl;
		}
	}
	return 0;
}