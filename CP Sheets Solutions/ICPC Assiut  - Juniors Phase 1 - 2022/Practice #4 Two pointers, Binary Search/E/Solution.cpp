#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isOk(const vector<ll> &toughness, ll K, ll d)
{
	bool canMake = false;

	ll cnt{};
	for (int i{}; i < toughness.size(); i++)
	{
		if (d >= toughness[i])
			d--, cnt++, i++;
	}
	return (cnt >= K);
}

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
	// cin >> t;
	while (t--)
	{
		cin >> N >> K;
		vector<ll> toughness(N);
		for (auto &ti : toughness)
			cin >> ti;
		ll L{}, R = (1LL << 40), ans{-1};
		while (L <= R)
		{
			ll mid = ((L + R) >> 1);
			if (isOk(toughness, K, mid))
				ans = mid, R = mid - 1;
			else
				L = mid + 1;
		}
		cout << ans;
	}
	return 0;
}