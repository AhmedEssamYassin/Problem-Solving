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
	ll N, k;
	cin >> t;
	while (t--)
	{
		cin >> N >> k;
		ll ans{};
		vector<ll> mods;
		for (int i{}; i < N; i++)
		{
			ll x;
			cin >> x;
			ans += (x / k);
			mods.push_back(x % k);
		}
		sort(mods.begin(), mods.end());
		for (int i{}, j = N - 1; i < j; i++, j--)
		{
			while (mods[i] + mods[j] < k && i < j)
				i++;
			if (i == j)
				break;
			ans++;
		}
		cout << ans << endl;
	}
	return 0;
}