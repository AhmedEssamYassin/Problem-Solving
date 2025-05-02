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
	ll N, d;
	// cin >> t;
	while (t--)
	{
		cin >> N >> d;
		vector<ll> a(N);
		for (int i{}; i < N; i++)
			cin >> a[i];
		ll ans{};
		ll j{};
		for (ll i{}; i < N; i++)
		{
			while (a[j] - a[i] <= d && j < N)
				j++;
			ll cnt = j - i - 1;
			ans += (cnt * (cnt - 1) / 2);
		}
		cout << ans;
	}
	return 0;
}