#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define endl "\n"

bool isOk(const vector<pair<ll, ll>> &vc, ll K, ll numberOfCopies)
{
	ll cnt{};
	for (const auto &[freq, si] : vc)
		cnt += freq / numberOfCopies;
	return cnt >= K;
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
		vector<ll> s(N);
		map<ll, ll> mp;
		for (ll &si : s)
			cin >> si, mp[si]++;
		vector<pair<ll, ll>> vc; // {frequency of si, si}
		for (const auto &[si, freq] : mp)
			vc.push_back({freq, si});
		sort(vc.begin(), vc.end(), greater<>()); // Sorting in descending order on basis of first element (frequency)
		ll L = 1, R = N, numberOfCopies = 1;
		while (L <= R)
		{
			ll mid = ((L + R) >> 1);
			if (isOk(vc, K, mid))
				numberOfCopies = mid, L = mid + 1;
			else
				R = mid - 1;
		}
		vector<ll> ans;
		for (const auto &[freq, si] : vc)
		{
			ll t = freq / numberOfCopies;
			while (ans.size() < K && t--)
				ans.push_back(si);
		}
		for (const ll &x : ans)
			cout << x << " ";
	}
	return 0;
}