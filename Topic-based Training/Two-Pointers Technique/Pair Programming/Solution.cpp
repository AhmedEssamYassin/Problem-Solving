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
	ll n, m, k;
	cin >> t;
	while (t--)
	{
		cin >> k >> n >> m;
		vector<ll> a(n), b(m);
		for (int i{}; i < n; i++)
			cin >> a[i];
		for (int i{}; i < m; i++)
			cin >> b[i];

		vector<ll> ans;
		for (int i{}, j{}; i < n || j < m; NULL)
		{
			if (i < n && a[i] <= k)
				k += (!a[i]), ans.push_back(a[i]), i++;
			else if (j < m && b[j] <= k)
				k += (!b[j]), ans.push_back(b[j]), j++;
			else
				break;
		}
		if (ans.size() < n + m)
			cout << -1 << endl;
		else
		{
			for (int i{}; i < n + m; i++)
				cout << ans[i] << " \n"[i == n + m - 1];
		}
	}
	return 0;
}