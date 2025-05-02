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
	ll N, K;
	cin >> t;
	while (t--)
	{
		ll n, m, a, b, c;
		cin >> n >> m;
		vector<int> line(n);
		for (int i{}; i < n; i++)
			cin >> line[i];

		sort(line.begin(), line.end());
		for (int i{}; i < m; i++)
		{
			cin >> a >> b >> c;
			auto ans = upper_bound(line.begin(), line.end(), b - 2 * sqrtl(a * c));

			if (ans != line.end() and *ans < b + 2 * sqrt(a * c))
				cout << "YES\n"
					 << *ans << endl;
			else
				cout << "NO\n";
		}
		cout << endl;
	}
	return 0;
}