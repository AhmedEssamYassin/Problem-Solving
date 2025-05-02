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
		cin >> N;
		vector<ll> vc(N);
		for (int i{}; i < N; i++)
			cin >> vc[i];

		vector<ll> pref(N, 0);
		pref[0] = vc[0];
		for (int i{1}; i < N; i++)
			pref[i] = pref[i - 1] + vc[i];
		ll tot = pref[N - 1];
		if (tot % 3 != 0)
			return cout << 0, 0;

		vector<ll> cnt(N, 0), acc(N, 0);
		for (int i{}; i < N; i++)
		{
			ll cur = (tot - (i ? pref[i - 1] : 0));
			cnt[i] = (cur == tot / 3);
		}
		acc[N - 1] = cnt[N - 1];
		for (int i = N - 2; i >= 0; i--)
			acc[i] = acc[i + 1] + cnt[i];
		ll ans{};
		for (int i{}; i < N - 2; i++)
		{
			ll Pref = pref[i];
			if (Pref == tot / 3)
				ans += acc[i + 2];
		}
		cout << ans;
	}
	return 0;
}