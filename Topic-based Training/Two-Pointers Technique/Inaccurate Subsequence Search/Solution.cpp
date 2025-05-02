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
	ll N, m, k;
	cin >> t;
	while (t--)
	{
		cin >> N >> m >> k;
		vector<ll> a(N), b(m);
		map<ll, ll> mp;
		for (int i{}; i < N; i++)
			cin >> a[i];
		for (int i{}; i < m; i++)
			cin >> b[i], mp[b[i]]++;

		ll cnt{}, ans{};
		for (int i{}; i < m; i++)
			if (mp[a[i]]-- > 0)
				cnt++;

		for (int i = m, j = 0; i < N; i++, j++)
		{
			if (cnt >= k)
				ans++;
			if (mp[a[i]]-- > 0)
				cnt++;

			if (mp[a[j]]++ >= 0)
				cnt--;
		}
		if (cnt >= k)
			ans++;
		cout << ans << endl;
	}
	return 0;
}